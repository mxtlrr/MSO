#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "libc/stdio.h"
#include "mem/mmap.h"
#include "mem/mm.h"
#include "multiboot.h"

extern void load_gdt();

void kmain(multiboot_info_t* mbd, uint32_t magic){
  printf("Hello world!\n\nMemory map:\n");

  // Now we have the memory map!
  block_t* b = getmmap(mbd, magic);
  printmmap(mbd, magic);

  // test malloc
  uint32_t m = malloc(b, 4); // allocate 4 bytes
  printf("\n\n4 bytes have been allocated to location: 0x%x", m);

  // okay, now let's free this memory
  free(m);

  // yuh!
  load_gdt();
  printf("GDT has been enabled!!");
}
