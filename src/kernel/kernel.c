#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "libc/stdio.h"

void kmain(){
  terminal_initialize();
  terminal_writestring("Hello world!\nNew line!");
}
