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

/* filesystem */
#include "fs/fs.h"

void kmain(multiboot_info_t* mbd, uint32_t magic){
  fs_node_t* fs_root = 0; // root of fs

  printf("Hello world!\n\n");

  // Load gdt
  load_gdt();
  printf("GDT has been enabled!!\n");

  init_idt();
  printf("IDT has been enabled!!\n");

  // start up our heap
  init_heap(0x10000, 0x10000);

  set_colors(0x0, 0x7);
  init_kbd();

  kassert(mbd->mods_count > 0); // has the module been loaded?
  uint32_t initrd_loc = *((uint32_t*)mbd->mods_addr);
  uint32_t initrd_end = *(uint32_t*)(mbd->mods_addr+4);

  printf("Initrd location -> 0x%x. Ends at 0x%x.\n", initrd_loc, initrd_end);
  fs_root = init_initrd(initrd_loc);

  for(;;) asm("hlt");
}
