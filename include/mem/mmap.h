#pragma once

#include <stdint.h>
#include <stddef.h>
#include "multiboot.h"

#define MAGIC_NUM 0x2badb002

typedef struct {
  uint32_t start_addr;
  uint32_t end_addr;
  uint32_t size;
  uint32_t type;
} block_t;

block_t* getmmap(multiboot_info_t* mbd, uint32_t magic);
void   printmmap(multiboot_info_t* mbd, uint32_t magic);

