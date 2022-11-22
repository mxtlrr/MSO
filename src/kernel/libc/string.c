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

char* strcpy(char* source, char* dest){
	while(*source != '\0') *dest++ = *source++;
	return dest;
}

int strcmp(const char *p1, const char *p2){
  const unsigned char *s1 = (const unsigned char *) p1;
  const unsigned char *s2 = (const unsigned char *) p2;
  unsigned char c1, c2;
  do {
		c1 = (unsigned char) *s1++;
		c2 = (unsigned char) *s2++;
	  if (c1 == '\0') return c1 - c2;
  } while (c1 == c2);
  return c1 - c2;
}
