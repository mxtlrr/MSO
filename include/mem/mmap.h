#pragma once

#include <stdint.h>
#include <stddef.h>
#include "multiboot.h"

#define MAGIC_NUM 0x2badb002
#define true 1
#define false 0

typedef struct {
  uint32_t begin_dir;
  uint32_t end_dir;
  uint32_t size;    // end_dir - begin_dir
  _Bool    free;
} block_t;

block_t* getmmap(multiboot_info_t* mbd, uint32_t magic);
