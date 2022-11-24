CC := gcc
CFLAGS := -Wall -Wextra -O3 -g -std=gnu99

all: initrd


initrd:
	@$(CC) initrd.c $(CFLAGS) -g -o initrd-gen
	@./initrd-gen file.txt file.txt
	@cp initrd.img ../bin/initrd.img	
	@rm initrd-gen
