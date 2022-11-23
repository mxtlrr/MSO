CC := gcc
CFLAGS := -Wall -Wextra -O3 -g -std=gnu99

all: initrd


initrd:
	@mkdir -p ../bin
	@$(CC) initrd.c $(CFLAGS) -o ../bin/initrd-gen
	@make -C test_files
