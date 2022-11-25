#pragma once

#include <stdint.h>
#include "mem/mm.h"
#include "fs/fs.h"

typedef struct {
  uint32_t nfiles; // The number of files in the ramdisk.
} initrd_header_t;

typedef struct {
  uint8_t magic;     // Magic number, for error checking.
  char name[64];  // Filename.
  uint32_t offset;   // Offset in the initrd that the file starts.
  uint32_t length;   // Length of the file.
} initrd_file_header_t;

fs_node_t* init_initrd(uint32_t location);
fs_node_t *initrd_finddir(fs_node_t *node, char *name);
struct dirent *initrd_readdir(fs_node_t* node, uint32_t index);
uint32_t initrd_read(fs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer);
