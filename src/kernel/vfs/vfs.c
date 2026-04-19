#include "vfs.h"
#include <string.h>

#define MAX_FILES 64

static vfs_file_t* g_files[MAX_FILES];
static int g_file_count = 0;

void vfs_register_file(vfs_file_t* file)
{
    if (g_file_count < MAX_FILES)
        g_files[g_file_count++] = file;
}

vfs_file_t* vfs_open(const char* path)
{
    for (int i = 0; i < g_file_count; i++)
    {
        if (strcmp(g_files[i]->name, path) == 0)
            return g_files[i];
    }

    return NULL;
}

int vfs_read(vfs_file_t* file, void* buf, size_t size, size_t offset)
{
    if (!file || !file->ops || !file->ops->read)
        return -1;

    return file->ops->read(file, buf, size, offset);
}

int vfs_write(vfs_file_t* file, const void* buf, size_t size, size_t offset)
{
    if (!file || !file->ops || !file->ops->write)
        return -1;

    return file->ops->write(file, buf, size, offset);
}

int vfs_close(vfs_file_t* file)
{
    if (!file || !file->ops || !file->ops->close)
        return -1;

    return file->ops->close(file);
}

void vfs_list(vfs_list_callback_t cb)
{
    for (int i = 0; i < g_file_count; i++)
    {
        if (g_files[i] && cb)
            cb(g_files[i]->name);
    }
}
