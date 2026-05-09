#pragma once
#include "vfs.h"

void ramfs_init();
vfs_file_t* ramfs_create_file(const char* name, const char* content);
