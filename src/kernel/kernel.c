#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "libc/stdio.h"
#include "mem/mmap.h"
#include "multiboot.h"

void kmain(multiboot_info_t* mbd, uint32_t magic){
  terminal_initialize();
  printf("Hello world!\n");

  // Print information
  printf("\nMultiboot header -> 0x%x\n", magic);

  block_t* b = getmmap(mbd, magic);
  for(multiboot_uint32_t i = 0; i != mbd->mmap_length; i++){
    printf("Block #%d\n", i);
  }
}
