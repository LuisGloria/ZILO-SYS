#pragma once
#include <stdint.h>
#include <stddef.h>

typedef struct vfs_file vfs_file_t;

// file handle and interface ig
typedef struct vfs_ops
{
    int (*read)(vfs_file_t* file, void* buf, size_t size, size_t offset);
    int (*write)(vfs_file_t* file, const void* buf, size_t size, size_t offset);
    int (*close)(vfs_file_t* file);
} vfs_ops_t;


typedef void (*vfs_list_callback_t)(const char* name);

void vfs_list(vfs_list_callback_t cb);

struct vfs_file
{
    const char* name;
    void* internal;

    const vfs_ops_t* ops;
};

// VFS API
vfs_file_t* vfs_open(const char* path);
int vfs_read(vfs_file_t* file, void* buf, size_t size, size_t offset);
int vfs_write(vfs_file_t* file, const void* buf, size_t size, size_t offset);
int vfs_close(vfs_file_t* file);

// registration
void vfs_register_file(vfs_file_t* file);
