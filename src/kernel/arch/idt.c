#include "arch/idt.h"
#include "arch/pic.h"
#include "libc/stdio.h"

__attribute__((aligned(0x10))) 
static idt_entry_t idt[256];
static idtr_t idtr;

static bool vectors[256];

__attribute__((noreturn))
void exception_handler(){
  set_colors(4, 15); // white on red
  printf("Exception has occurred. Halting...");
  __asm__("cli; hlt"); // Halt computer
}

void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags){
  idt_entry_t* descriptor = &idt[vector];

  descriptor->isr_low   = (uint32_t)isr & 0xffff;
  descriptor->kernel_cs = KERNEL_CS;
  descriptor->attrib    = flags;
  descriptor->isr_high  = (uint32_t)isr >> 16;
  descriptor->reserved  = 0;
}


void idt_init(){
  idtr.base = (uintptr_t)&idt[0];
  idtr.limit = (uint16_t)sizeof(idt_entry_t) * 256 - 1;

  for(uint8_t vector = 0; vector < 32; vector++){
    idt_set_descriptor(vector, isr_stub_table[vector], 0x8e);
    vectors[vector] = true;
  }

  remap_pic(); // remap pic

  // load idt
  __asm__ volatile("lidt %0" :: "m"(idtr));
  __asm__ volatile("sti"); // set interrupt flag! we have started interrupts
}

void irq_handler(registers_t regs){
  printf("interrupt");
  outb(0xa0, 0x20);
  outb(0x20, 0x20);
}
