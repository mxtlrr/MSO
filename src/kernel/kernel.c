#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "libc/stdio.h"
#include "mem/mmap.h"
#include "multiboot.h"

void kmain(multiboot_info_t* mbd, uint32_t magic){
  printf("Hello world!\n");

  // Now we have the memory map!
  block_t* b = getmmap(mbd, magic);
}
