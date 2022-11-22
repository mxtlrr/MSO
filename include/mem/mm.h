#pragma once

#include <stddef.h>
#include <stdint.h>

// TODO: Rewrite the memory allocator.
// This time, without depending on the grub shit.

void memcpy(void* dest, void* source, uint32_t n);
