#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "libc/stdio.h"

/* memory */
#include "mem/mmap.h"
#include "mem/mm.h"
#include "multiboot.h"

extern void load_gdt();

void kmain(){
  printf("Hello world!\n\nMemory map:\n");

  // Load gdt
  load_gdt();
  printf("GDT has been enabled!!");
}
