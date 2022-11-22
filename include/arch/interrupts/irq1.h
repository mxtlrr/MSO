/* IRQ1 ---> Keyboard */
#pragma once

#include "arch/idt.h" // on the safe side!
#include "libc/stdio.h"
#include "libc/stdio_debugcon.h"

// Initialize the keyboard interrupt
void init_kbd();


// Function that gets called whenever there's
// a keyboard int.
void kbd_handle();
