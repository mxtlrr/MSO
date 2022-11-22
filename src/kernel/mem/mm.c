#include "mem/mm.h"

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
