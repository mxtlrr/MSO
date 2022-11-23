#include "mem/mm.h"

struct m_seg_hdr* FirstfreeMemorySegment;


// initalize the heap, where the starting position is heap_addr and ends at
// heap_len + heap_addr
void init_heap(uint32_t heap_addr, uint32_t heap_len) {
	FirstfreeMemorySegment = (struct m_seg_hdr*)heap_addr;
	FirstfreeMemorySegment->mem_len = heap_len - sizeof(struct m_seg_hdr);
	FirstfreeMemorySegment->next_seg = 0;
	FirstfreeMemorySegment->prev_seg = 0;
	FirstfreeMemorySegment->n_free_seg = 0;
	FirstfreeMemorySegment->prev_f_seg = 0;
	FirstfreeMemorySegment->free = true;
}

// allocate size in bytes, then
// set to 0.
void* calloc(uint32_t size) {
	void* mallocVal = malloc(size);
	memset(mallocVal, 0, size);
	return mallocVal;
}

// allocate size in bytes and return the address to
// the beginning of the allocated block.
void* malloc(uint32_t size) {
	uint32_t remainder = size % 8;
	size -= remainder;
	if (remainder != 0) size += 8;

	struct m_seg_hdr* cur_mem_seg = FirstfreeMemorySegment;

	while (true) {
		if (cur_mem_seg->mem_len >= size) {
			if (cur_mem_seg->mem_len > size + sizeof(struct m_seg_hdr)) {
				struct m_seg_hdr* newSegmentHeader = (struct m_seg_hdr*)((uint32_t)cur_mem_seg + sizeof(struct m_seg_hdr) + size);
				
				newSegmentHeader->free = true;
				newSegmentHeader->mem_len = ((uint32_t)cur_mem_seg->mem_len) - (sizeof(struct m_seg_hdr) + size);
				newSegmentHeader->n_free_seg = cur_mem_seg->n_free_seg;
				newSegmentHeader->next_seg = cur_mem_seg->next_seg;
				newSegmentHeader->prev_seg = cur_mem_seg;
				newSegmentHeader->prev_f_seg = cur_mem_seg->prev_f_seg;

				cur_mem_seg->n_free_seg = newSegmentHeader;
				cur_mem_seg->next_seg = newSegmentHeader;
				cur_mem_seg->mem_len = size; 
			}
			if (cur_mem_seg == FirstfreeMemorySegment) FirstfreeMemorySegment = cur_mem_seg->n_free_seg;
			cur_mem_seg->free = false;
			
			if (cur_mem_seg->prev_f_seg != 0) cur_mem_seg->prev_f_seg->n_free_seg = cur_mem_seg->n_free_seg;
			if (cur_mem_seg->n_free_seg != 0) cur_mem_seg->n_free_seg->prev_f_seg = cur_mem_seg->prev_f_seg;
			if (cur_mem_seg->prev_seg != 0) cur_mem_seg->prev_seg->n_free_seg = cur_mem_seg->n_free_seg;
			if (cur_mem_seg->next_seg != 0) cur_mem_seg->next_seg->prev_f_seg = cur_mem_seg->prev_f_seg;

			return cur_mem_seg + 1;
		}
		if (cur_mem_seg->n_free_seg == 0) return 0;
		cur_mem_seg = cur_mem_seg->n_free_seg;
	}
	return 0;
}

// free a memory block (passed in by address)
void free(void* address) {
	struct m_seg_hdr* cur_mem_seg = ((struct m_seg_hdr*)address) - 1;
	cur_mem_seg->free = true;

	if (cur_mem_seg < FirstfreeMemorySegment) FirstfreeMemorySegment = cur_mem_seg;

	if (cur_mem_seg->n_free_seg != 0) {
		if (cur_mem_seg->n_free_seg->prev_f_seg < cur_mem_seg)
			cur_mem_seg->n_free_seg->prev_f_seg = cur_mem_seg;
	}
	if (cur_mem_seg->prev_f_seg != 0) {
		if (cur_mem_seg->prev_f_seg->n_free_seg > cur_mem_seg)
			cur_mem_seg->prev_f_seg->n_free_seg = cur_mem_seg;
	}
	if (cur_mem_seg->next_seg != 0) {
		cur_mem_seg->next_seg->prev_seg = cur_mem_seg;
		if (cur_mem_seg->next_seg->free) add_free_seg(cur_mem_seg, cur_mem_seg->next_seg);
	}
	if (cur_mem_seg->prev_seg != 0) {
		cur_mem_seg->prev_seg->next_seg = cur_mem_seg;
		if (cur_mem_seg->prev_seg->free) add_free_seg(cur_mem_seg, cur_mem_seg->prev_seg);
	}
}

// add a free segment.
void add_free_seg(struct m_seg_hdr* a, struct m_seg_hdr* b) {
	if (a == 0) return;
	if (b == 0) return;
	if (a < b) {
		a->mem_len += b->mem_len + sizeof(struct m_seg_hdr);
		a->next_seg = b->next_seg;
		a->n_free_seg = b->n_free_seg;
		b->next_seg->prev_seg = a;
		b->next_seg->prev_f_seg = a;
		b->n_free_seg->prev_f_seg = a;
	}	else {
		b->mem_len += a->mem_len + sizeof(struct m_seg_hdr);
		b->next_seg = a->next_seg;
		b->n_free_seg = a->n_free_seg;
		a->next_seg->prev_seg = b;
		a->next_seg->prev_f_seg = b;
		a->n_free_seg->prev_f_seg = b;
	}
}

void memcpy(void* destination, void* source, uint32_t num) {
	if (num <= 8) {
		uint8_t* valPtr = (uint8_t*)source;
		for (uint8_t* ptr = (uint8_t*)destination; ptr < (uint8_t*)((uint32_t)destination + num); ptr++) {
			*ptr = *valPtr;
			valPtr++;
		}
		return;
	}

	uint32_t newnum = num - (num % 8);
	uint32_t* srcptr = (uint32_t*)source;
	for (uint32_t* destptr = (uint32_t*)destination; destptr < (uint32_t*)((uint32_t)destination + newnum); destptr++) {
		*destptr = *srcptr;
		srcptr++;
	}

	uint8_t* srcptr8 = (uint8_t*)((uint32_t)source + newnum);
	for (uint8_t* destptr8 = (uint8_t*)((uint32_t)destination + newnum); destptr8 < (uint8_t*)((uint32_t)destination + num); destptr8++){
		*destptr8 = *srcptr8;
		srcptr8++;
	}
}
