#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "libc/stdio.h"
#include "multiboot.h"
#include "mem/mm.h"

/* architecture specific stuff */
#include "arch/gdt.h"
#include "arch/idt.h"

/* interrupts */
#include "arch/interrupts/irq1.h"

/* utilites */
#include "libc/fsutil.h"

uint32_t placement_address;

void kmain(multiboot_info_t* mbd, uint32_t magic){
  fs_node_t* fs_root = 0; // root of fs

  printf("Hello world!\n\n");

  // Load desc. tables.
  load_gdt();
  init_idt();

  set_colors(0x0, 0x7);
  init_kbd();

  kassert(mbd->mods_count > 0); // has the module been loaded?
  uint32_t initrd_loc = *((uint32_t*)mbd->mods_addr);
  uint32_t initrd_end = *(uint32_t*)(mbd->mods_addr+4);
  placement_address = initrd_end;
  fs_root = init_initrd(initrd_loc);

  // start up our heap
  init_heap(0x10000, 0x10000);

  kassert(fs_root != NULL);

  FILE* test = fopen("file.txt", "r", fs_root);
  if(test == NULL) printf("Woops! File doesn't exist..\n");
  else {
    printf("mode -> %d\nfile name -> %s\n", test->mode, test->f);
  }
  
  for(;;) asm("hlt");
}
