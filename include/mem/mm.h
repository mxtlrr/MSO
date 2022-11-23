#pragma once

#include <stddef.h>
#include <stdint.h>

// TODO: Rewrite the memory allocator.
// This time, without depending on the grub shit.

struct m_seg_hdr {
	uint32_t mem_len;
	struct m_seg_hdr* next_seg;
	struct m_seg_hdr* prev_seg;
	struct m_seg_hdr* n_free_seg;
	struct m_seg_hdr* prev_f_seg;
	int free;
};

void* malloc(uint32_t size);
void free(void* address);
void init_heap(uint32_t heap_addr, uint32_t heap_len);
void calloc(uint32_t size);
void* realloc(void* address, uint32_t newSize);
void add_free_seg(struct m_seg_hdr* a, struct m_seg_hdr* b);
void free(void* address);

void memcpy(void* dest, void* source, uint32_t n);
