CC := gcc
CFLAGS := -Wall -Wextra -O3 -g -std=gnu99

all: initrd

initrd:
	$(CC) initrd.c $(CFLAGS) -o ../bin/initrd-gen
