#include "ramfs.h"
#include "memory.h"
#include "string.h"
#include "stdint.h"
#include "../heap.h"

typedef struct
{
    char* data;
    size_t size;
} ramfs_inode_t;

static int ramfs_read(vfs_file_t* file, void* buf, size_t size, size_t offset)
{
    ramfs_inode_t* inode = (ramfs_inode_t*)file->internal;

    if (offset >= inode->size)
        return 0;

    size_t to_read = size;
    if (offset + to_read > inode->size)
        to_read = inode->size - offset;

    memcpy(buf, inode->data + offset, to_read);
    return to_read;
}

static int ramfs_write(vfs_file_t* file, const void* buf, size_t size, size_t offset)
{
    ramfs_inode_t* inode = (ramfs_inode_t*)file->internal;

    if (offset + size > inode->size)
        return -1;

    memcpy(inode->data + offset, buf, size);
    return size;
}

static int ramfs_close(vfs_file_t* file)
{
    (void)file;
    return 0;
}

static vfs_ops_t ramfs_ops =
{
    .read = ramfs_read,
    .write = ramfs_write,
    .close = ramfs_close
};

vfs_file_t* ramfs_create_file(const char* name, const char* content)
{
    ramfs_inode_t* inode = (ramfs_inode_t*)kmalloc(sizeof(ramfs_inode_t));
    inode->size = strlen(content);

    inode->data = kmalloc(inode->size + 1);
    memcpy(inode->data, content, inode->size);
    inode->data[inode->size] = 0;

    vfs_file_t* file = (vfs_file_t*)kmalloc(sizeof(vfs_file_t));

    file->name = name;
    file->internal = inode;
    file->ops = &ramfs_ops;

    return file;
}

void ramfs_init()
{
    // test files
    vfs_register_file(
        ramfs_create_file("hello.txt", "RAMFS works :3\n")
    );

    vfs_register_file(
        ramfs_create_file("readme.txt", "VFS :3.\n")
    );
}
