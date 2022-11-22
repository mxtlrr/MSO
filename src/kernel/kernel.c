#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "libc/stdio.h"
#include "multiboot.h"

/* architecture specific stuff */
#include "arch/gdt.h"
#include "arch/idt.h"

/* interrupts */
#include "arch/interrupts/irq1.h"

void kmain(multiboot_info_t* mbd, uint32_t magic){
  printf("Hello world!\n\n");

  // Load gdt
  load_gdt();
  printf("GDT has been enabled!!\n");

  init_idt();
  printf("IDT has been enabled!!\n");

  init_kbd();

  for(;;) asm("hlt");
}
