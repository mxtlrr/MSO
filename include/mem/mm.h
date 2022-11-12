#pragma once

#include <stddef.h>
#include <stdint.h>

/* Probably the worst EVER design, for a memory allocator.
 * but it should work. */

#include "mem/mmap.h" // ;)
#include "libc/stdio.h"

// Find a hole in the memory map, that fits N, return location
uint32_t find_hole(block_t* b, uint32_t n);
uint32_t malloc(block_t* b, uint32_t size);
