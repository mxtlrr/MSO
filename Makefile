AS 	 := as
AS_FLAGS := --32

CC	 := gcc
CFLAGS	 := -m32 -ffreestanding -O2 -Wall -Wextra -std=gnu99

all: clean mso mkiso

OBJS := obj/boot.o obj/kernel.o

clean:
	rm -rf obj/ bin/ mso.iso isoroot/

mso:
	mkdir -p obj/ bin/
	$(AS) $(AS_FLAGS) src/boot/boot.s -o obj/boot.o
	$(CC) -c src/kernel/kernel.c -o obj/kernel.o $(CFLAGS)

	$(CC) -Tsrc/linker.ld -o bin/mso.bin -ffreestanding -O2 -nostdlib $(OBJS) -lgcc

mkiso:
	mkdir -p isoroot/boot/grub
	cp bin/mso.bin isoroot/boot/mso.bin
	cp src/boot/grub.cfg isoroot/boot/grub/grub.cfg
	grub-mkrescue -o mso.iso isoroot
