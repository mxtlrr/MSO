#pragma once

#include <stdint.h>
#include "mem/mm.h"
#include "fs/fs.h"

typedef char  byte;
typedef short word;

typedef struct {
  uint32_t nfiles;
} initrd_hdr_t;

typedef struct {
  word magic;   // 0x3377
  char fname[64];
  uint32_t offset;
  uint32_t filesize;
  byte ending;  // always 0x00
} initrd_file_header_t;

fs_node_t* init_initrd(uint32_t location);
fs_node_t *initrd_finddir(fs_node_t *node, char *name);
struct dirent *initrd_readdir(fs_node_t* node, uint32_t index);
uint32_t initrd_read(fs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer);
