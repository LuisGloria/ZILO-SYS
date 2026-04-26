#include "dataset.h"
#include "../text.h"
#include "../drivers/block/ata.h"
#include "../string.h"
#include "../memory.h"

#define DATASET_TABLE_LBA 1
#define SECTOR_SIZE 512

static dataset_t datasets[MAX_DATASETS];


// Trim input
static void trim(char* str)
{
    for (int i = 0; str[i]; i++)
    {
        if (str[i] == '\n' || str[i] == '\r' || str[i] == ' ')
        {
            str[i] = 0;
            return;
        }
    }
}

static dataset_t* dataset_find(const char* name)
{
    char clean_name[DATASET_NAME_LEN];
    memset(clean_name, 0, DATASET_NAME_LEN);
    strncpy(clean_name, name, DATASET_NAME_LEN - 1);

    trim(clean_name);

    text_print("Searching for: ");
    text_print(clean_name);
    text_print("\n");

    for (int i = 0; i < MAX_DATASETS; i++)
    {
        if (datasets[i].name[0])
        {
            text_print("Checking: ");
            text_print(datasets[i].name);
            text_print("\n");

            if (strncmp(datasets[i].name, clean_name, DATASET_NAME_LEN) == 0)
            {
                return &datasets[i];
            }
        }
    }

    return NULL;
}

int dataset_count()
{
    int c = 0;

    for (int i = 0; i < MAX_DATASETS; i++)
    {
        if (datasets[i].name[0])
            c++;
    }

    return c;
}

dataset_t* dataset_get(int index)
{
    int c = 0;

    for (int i = 0; i < MAX_DATASETS; i++)
    {
        if (datasets[i].name[0])
        {
            if (c == index)
                return &datasets[i];

            c++;
        }
    }

    return NULL;
}

int dset_write_file(const char* name,
                    const char* fname,
                    const uint8_t* data,
                    uint32_t size)
{
    dataset_t* d = dataset_find(name);
    if (!d)
    {
        text_print("Dataset not found\n");
        return -1;
    }

    uint8_t sector[SECTOR_SIZE];

    ata_read28(d->start_lba, 1, sector);
    dset_header_t* hdr = (dset_header_t*)sector;

    if (hdr->magic != DSET_MAGIC)
    {
        // init dataset
        hdr->magic = DSET_MAGIC;
        hdr->file_count = 0;
        ata_write28(d->start_lba, 1, sector);

        // clear file table
        memset(sector, 0, SECTOR_SIZE);
        ata_write28(d->start_lba + 1, 1, sector);
    }

    ata_read28(d->start_lba + 1, 1, sector);
    dset_file_t* table = (dset_file_t*)sector;

    // find file or free slot
    int index = -1;

    for (int i = 0; i < DSET_MAX_FILES; i++)
    {
        if (strncmp(table[i].name, fname, DSET_FILENAME_LEN) == 0)
        {
            index = i;
            break;
        }

        if (table[i].name[0] == 0 && index == -1)
            index = i;
    }

    if (index == -1)
    {
        text_print("No file slots\n");
        return -1;
    }

    uint32_t offset = SECTOR_SIZE * 2;

    for (int i = 0; i < DSET_MAX_FILES; i++)
    {
        if (table[i].name[0])
        {
            uint32_t end = table[i].offset + table[i].size;
            if (end > offset)
                offset = end;
        }
    }

    uint32_t sectors = (size + 511) / 512;
    ata_write28(d->start_lba + (offset / SECTOR_SIZE), sectors, (uint8_t*)data);

    // update entry
    memset(table[index].name, 0, DSET_FILENAME_LEN);
    strncpy(table[index].name, fname, DSET_FILENAME_LEN - 1);
    table[index].offset = offset;
    table[index].size = size;

    int is_new = (table[index].name[0] == 0);
    if (is_new)
        hdr->file_count++;

    // save table + header
    ata_write28(d->start_lba + 1, 1, (uint8_t*)table);

    ata_read28(d->start_lba, 1, sector);
    ((dset_header_t*)sector)->file_count = hdr->file_count;
    ata_write28(d->start_lba, 1, sector);

    text_print("File saved\n");
    return 0;
}



int dataset_write(const char* name, const uint8_t* data, uint32_t size)
{
    dataset_t* d = dataset_find(name);

    if (!d)
    {
        text_print("Dataset not found\n");
        return -1;
    }

    uint32_t sectors = (size + 511) / 512;

    if (sectors > d->size)
    {
        text_print("Dataset too small\n");
        return -1;
    }

    return ata_write28(d->start_lba, sectors, (uint8_t*)data);
}


int dataset_read(const char* name, uint8_t* buffer)
{
    dataset_t* d = dataset_find(name);

    if (!d)
    {
        text_print("Dataset not found\n");
        return -1;
    }

    return ata_read28(d->start_lba, d->size, buffer);
}


static void dataset_save()
{
    ata_write28(DATASET_TABLE_LBA, 1, (uint8_t*)datasets);
}


static void dataset_load()
{
    ata_read28(DATASET_TABLE_LBA, 1, (uint8_t*)datasets);
}


void dataset_init()
{
    dataset_load();
}


int dataset_create(const char* name, uint32_t size)
{
    char clean_name[DATASET_NAME_LEN];
    memset(clean_name, 0, DATASET_NAME_LEN);
    strncpy(clean_name, name, DATASET_NAME_LEN - 1);

    trim(clean_name);

    for (int i = 0; i < MAX_DATASETS; i++)
    {
        if (datasets[i].name[0] == 0)
        {
            memset(datasets[i].name, 0, DATASET_NAME_LEN);
            strncpy(datasets[i].name, clean_name, DATASET_NAME_LEN - 1);

            uint32_t last_end = 10;

            for (int j = 0; j < MAX_DATASETS; j++)
            {
                if (datasets[j].name[0])
                {
                    uint32_t end = datasets[j].start_lba + datasets[j].size;
                    if (end > last_end)
                        last_end = end;
                }
            }

            datasets[i].start_lba = last_end;
            datasets[i].size = size;

            dataset_save();

            text_print("Dataset created\n");
            return 0;
        }
    }

    text_print("No free dataset slots\n");
    return -1;
}


int dataset_delete(const char* name)
{
    char clean_name[DATASET_NAME_LEN];
    memset(clean_name, 0, DATASET_NAME_LEN);
    strncpy(clean_name, name, DATASET_NAME_LEN - 1);

    trim(clean_name);

    for (int i = 0; i < MAX_DATASETS; i++)
    {
        if (datasets[i].name[0] &&
            strncmp(datasets[i].name, clean_name, DATASET_NAME_LEN) == 0)
        {
            memset(&datasets[i], 0, sizeof(dataset_t));

            dataset_save();

            text_print("Dataset deleted\n");
            return 0;
        }
    }

    text_print("Dataset not found\n");
    return -1;
}

int dset_read_file(const char* name,
                   const char* fname,
                   uint8_t* buffer)
{
    dataset_t* d = dataset_find(name);
    if (!d)
    {
        text_print("Dataset not found\n");
        return -1;
    }

    uint8_t sector[SECTOR_SIZE];

    ata_read28(d->start_lba + 1, 1, sector);
    dset_file_t* table = (dset_file_t*)sector;

    for (int i = 0; i < DSET_MAX_FILES; i++)
    {
        if (strncmp(table[i].name, fname, DSET_FILENAME_LEN) == 0)
        {
            uint32_t sectors = (table[i].size + 511) / 512;

            return ata_read28(
                d->start_lba + (table[i].offset / SECTOR_SIZE),
                              sectors,
                              buffer
            );
        }
    }

    text_print("File not found\n");
    return -1;
}

void dataset_list()
{
    for (int i = 0; i < MAX_DATASETS; i++)
    {
        if (datasets[i].name[0])
        {
            text_print("Name: ");
            text_print(datasets[i].name);

            text_print(" LBA: ");
            text_print_dec(datasets[i].start_lba);

            text_print(" Size: ");
            text_print_dec(datasets[i].size);

            text_print("\n");
        }
    }
}
