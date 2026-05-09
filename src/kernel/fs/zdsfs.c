#include "zdsfs.h"

#include "../text.h"
#include "../drivers/block/ata.h"
#include "../string.h"
#include "../memory.h"

static zdsfs_superblock_t superblock;
static dataset_t vtoc[ZDSFS_MAX_DATASETS];

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

static void zdsfs_save_vtoc()
{
    ata_write28(
        superblock.vtoc_start,
        ZDSFS_VTOC_SECTORS,
        (uint8_t*)vtoc
    );
}

static void zdsfs_load_vtoc()
{
    ata_read28(
        superblock.vtoc_start,
        ZDSFS_VTOC_SECTORS,
        (uint8_t*)vtoc
    );
}

static dataset_t* zdsfs_find(const char* name)
{
    char clean[ZDSFS_NAME_LEN];

    memset(clean, 0, sizeof(clean));
    strncpy(clean, name, ZDSFS_NAME_LEN - 1);
    trim(clean);

    for (int i = 0; i < ZDSFS_MAX_DATASETS; i++)
    {
        if (vtoc[i].name[0] &&
            strncmp(vtoc[i].name, clean, ZDSFS_NAME_LEN) == 0)
        {
            return &vtoc[i];
        }
    }

    return NULL;
}

void zdsfs_format()
{
    text_print("[ZDSFS] Formatting disk...\n");

    memset(&superblock, 0, sizeof(superblock));
    memset(vtoc, 0, sizeof(vtoc));

    superblock.magic = ZDSFS_MAGIC;
    superblock.version = ZDSFS_VERSION;

    superblock.vtoc_start = ZDSFS_VTOC_LBA;
    superblock.vtoc_size = ZDSFS_VTOC_SECTORS;
    superblock.total_datasets = 0;

    ata_write28(
        ZDSFS_SUPERBLOCK_LBA,
        1,
        (uint8_t*)&superblock
    );

    zdsfs_save_vtoc();

    text_print("[{0x08}ZDSFS{0x0F}]       Format complete\n");
}

void zdsfs_init()
{
    ata_read28(
        ZDSFS_SUPERBLOCK_LBA,
        1,
        (uint8_t*)&superblock
    );

    if (superblock.magic != ZDSFS_MAGIC)
    {
        zdsfs_format();
    }

    zdsfs_load_vtoc();

    text_print("[{0x08}ZDSFS{0x0F}]       Ready\n");
}

int zdsfs_create(const char* name)
{
    uint32_t size_lba = ZDSFS_DEFAULT_SIZE;

    if (zdsfs_find(name))
    {
        text_print("[{0x08}ZDSFS{0x0F}]       Dataset already exists\n");
        return -1;
    }

    for (int i = 0; i < ZDSFS_MAX_DATASETS; i++)
    {
        if (vtoc[i].name[0] == 0)
        {
            uint32_t last_end = ZDSFS_DATA_START_LBA;

            for (int j = 0; j < ZDSFS_MAX_DATASETS; j++)
            {
                if (vtoc[j].name[0])
                {
                    uint32_t end =
                    vtoc[j].start_lba +
                    vtoc[j].size_lba;

                    if (end > last_end)
                        last_end = end;
                }
            }

            memset(&vtoc[i], 0, sizeof(dataset_t));

            strncpy(
                vtoc[i].name,
                name,
                ZDSFS_NAME_LEN - 1
            );

            vtoc[i].format_id = '1';
            vtoc[i].start_lba = last_end;
            vtoc[i].size_lba = size_lba;
            vtoc[i].file_size = 0;
            vtoc[i].flags = 0;

            superblock.total_datasets++;

            ata_write28(
                ZDSFS_SUPERBLOCK_LBA,
                1,
                (uint8_t*)&superblock
            );

            zdsfs_save_vtoc();

            text_print("[{0x08}ZDSFS{0x0F}]       Dataset created\n");
            return 0;
        }
    }

    text_print("[{0x08}ZDSFS{0x0F}]       No free dataset slots\n");
    return -1;
}

int zdsfs_write(
    const char* name,
    const uint8_t* data,
    uint32_t size_bytes
)
{
    dataset_t* d = zdsfs_find(name);

    if (!d)
    {
        text_print("[{0x08}ZDSFS{0x0F}]       Dataset not found\n");
        return -1;
    }

    uint32_t sectors = (size_bytes + 511) / 512;

    if (sectors > d->size_lba)
    {
        text_print("[{0x08}ZDSFS{0x0F}]       Dataset too small\n");
        return -1;
    }

    ata_write28(
        d->start_lba,
        sectors,
        (uint8_t*)data
    );

    d->file_size = size_bytes;

    zdsfs_save_vtoc();

    text_print("[{0x08}ZDSFS{0x0F}]       Dataset written\n");
    return 0;
}

int zdsfs_read(
    const char* name,
    uint8_t* buffer
)
{
    dataset_t* d = zdsfs_find(name);

    if (!d)
    {
        text_print("[{0x08}ZDSFS{0x0F}]       Dataset not found\n");
        return -1;
    }

    uint32_t sectors = (d->file_size + 511) / 512;

    return ata_read28(
        d->start_lba,
        sectors,
        buffer
    );
}

int zdsfs_delete(const char* name)
{
    dataset_t* d = zdsfs_find(name);

    if (!d)
    {
        text_print("[{0x08}ZDSFS{0x0F}]       Dataset not found\n");
        return -1;
    }

    memset(d, 0, sizeof(dataset_t));

    superblock.total_datasets--;

    ata_write28(
        ZDSFS_SUPERBLOCK_LBA,
        1,
        (uint8_t*)&superblock
    );

    zdsfs_save_vtoc();

    text_print("[{0x08}ZDSFS{0x0F}]       Dataset deleted\n");
    return 0;
}

void zdsfs_list()
{
    for (int i = 0; i < ZDSFS_MAX_DATASETS; i++)
    {
        if (vtoc[i].name[0])
        {
            text_print("Name: ");
            text_print(vtoc[i].name);

            text_print(" LBA: ");
            text_print_dec(vtoc[i].start_lba);

            text_print(" Size: ");
            text_print_dec(vtoc[i].size_lba);

            text_print("\n");
        }
    }
}

int zdsfs_count()
{
    int c = 0;

    for (int i = 0; i < ZDSFS_MAX_DATASETS; i++)
        if (vtoc[i].name[0])
            c++;

    return c;
}

zdsfs_dscb_t* zdsfs_get(int index)
{
    int c = 0;

    for (int i = 0; i < ZDSFS_MAX_DATASETS; i++)
    {
        if (vtoc[i].name[0])
        {
            if (c == index)
                return &vtoc[i];

            c++;
        }
    }

    return NULL;
}
