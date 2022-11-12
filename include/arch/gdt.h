/* helper functions for the gdt. */
#pragma once

#include <stdint.h>
#include <stddef.h>


// defined in asm/gdt.s
extern void load_gdt();
