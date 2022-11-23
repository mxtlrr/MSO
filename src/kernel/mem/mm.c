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

void memcpy(void* destination, void* source, uint32_t num) {
	if (num <= 8) {
		uint8_t* valPtr = (uint8_t*)source;
		for (uint8_t* ptr = (uint8_t*)destination; ptr < (uint8_t*)((uint32_t)destination + num); ptr++) {
			*ptr = *valPtr;
			valPtr++;
		}

		return;
	}

	uint32_t proceedingBytes = num % 8;
	uint32_t newnum = num - proceedingBytes;
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
