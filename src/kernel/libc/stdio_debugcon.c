#include "libc/stdio_debugcon.h"

void serial_putc(char c){
  outb(0xe9, c);
}

void serial_puts(char* fmt){
  for(int i = 0; fmt[i]; i++) serial_putc(fmt[i]);
}
