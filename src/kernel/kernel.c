#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "libc/stdio.h"
#include "multiboot.h"

void kmain(multiboot_info_t* mbd, uint32_t magic){
  terminal_initialize();
  printf("Hello world!\n");

  // Print information
  printf("\nMultiboot header -> 0x%x\n", magic);
}
