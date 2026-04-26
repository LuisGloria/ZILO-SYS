#pragma once
#include <stdint.h>

#define MAX_DATASETS 16
#define DATASET_NAME_LEN 16
#define DSET_MAGIC 0x3153445A // "ZDS1"
#define DSET_MAX_FILES 16
#define DSET_FILENAME_LEN 16

typedef struct {
    uint32_t magic;
    uint32_t file_count;
} dset_header_t;

typedef struct {
    char name[DSET_FILENAME_LEN];
    uint32_t offset; // in bytes from dataset start
    uint32_t size;
} dset_file_t;

int dset_write_file(const char* dset,
                    const char* fname,
                    const uint8_t* data,
                    uint32_t size);

int dset_read_file(const char* dset,
                   const char* fname,
                   uint8_t* buffer);

typedef struct {
    char name[DATASET_NAME_LEN];
    uint32_t start_lba;
    uint32_t size; // in sectors
} dataset_t;

void dataset_init();

int dataset_count();
dataset_t* dataset_get(int index);



int dataset_create(const char* name, uint32_t size);
int dataset_delete(const char* name);
int dataset_write(const char* name, const uint8_t* data, uint32_t size);
int dataset_read(const char* name, uint8_t* buffer);
void dataset_list();
