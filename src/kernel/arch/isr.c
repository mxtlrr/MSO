#include "arch/isr.h"

void isr_handler(registers_t regs){
  set_colors(0x0, 0x4); // black bg, red fg
  printf("MSO has recieved an exception! INT_NO: 0x%x", regs.int_no);
  asm("cli; hlt"); // stop interrupts, halt computer entirely
}
