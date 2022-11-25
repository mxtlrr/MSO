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
#include "fs/initrd.h"

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

  // why shouldn't we just look through the fs
  struct dirent* node = 0;
  for(int i = 0; (node = readdir_fs(fs_root, i)) != 0; i++){
    printf("File found! Name: \"%s\"\n", node->name);
    fs_node_t *fsnode = finddir_fs(fs_root, node->name);

    // copy the file contents into a buffer
    char buf[256];
    uint32_t sz = read_fs(fsnode, 0, 256, buf);

    printf("contents are:\n\"%s\"..\n", buf);
    i++;
  }

  for(;;) asm("hlt");
}
