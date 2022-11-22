#include "arch/irq.h"

isr_t irqs[256];

void reg_int_handler(uint8_t n, isr_t handler){
  irqs[n] = handler;
}

void irq_handler(registers_t r){
  // Sending EOI!

  if(r.int_no >= 40) outb(0xa0, 0x20);
  outb(0x20, 0x20);

  // call the handler if it exists
  if(irqs[r.int_no] != 0){
    isr_t h = irqs[r.int_no];
    h(r);
  } else {
    // NOTE: This spams that IRQ: 0x20 doesn't exist
    /* set_colors(0x0, 0x4);
    printf("That interrupt (int no -> 0x%x) doesn't exist\n", r.int_no); */
  }
}
