#include "libc/string.h"

char* itoa(unsigned int num, int base){
	static char repr[]= "0123456789ABCDEF";
	static char buffer[50]; 
	char *ptr; 

	ptr = &buffer[49]; 
	*ptr = '\0'; 

	do {
		*--ptr = repr[num%base]; 
		num /= base; 
	} while(num != 0); 

	return(ptr); 
}

void* memset(void *dest, int val, size_t len){
  unsigned char *ptr = dest;
  while (len-- > 0)
    *ptr++ = val;
  return dest;
}
