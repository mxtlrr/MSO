AS 	 := as
AS_FLAGS := --32

CC	 := gcc
CFLAGS	 := -m32 -ffreestanding -O2 -std=gnu99 -Iinclude/ -fno-stack-protector -nostdlib

all: clean mso mkiso

override OBJS := $(shell find ./obj/ -type f -name '*.o')
override CFILES := $(shell find ./ -type f -name '*.c')

clean:
	rm -rf obj/ bin/ mso.iso isoroot/

mso:
	mkdir -p obj/ bin/
	$(AS) $(AS_FLAGS) src/boot/boot.s -o obj/boot.o
	@$(foreach file, $(CFILES), $(CC) $(CFLAGS) -c $(file) -o obj/$(basename $(notdir $(file))).o; echo CC $(file);)
	
	$(CC) -m32 -Tsrc/linker.ld -o bin/mso.bin -ffreestanding -O2 -nostdlib -fno-stack-protector $(OBJS) -lgcc

mkiso:
	mkdir -p isoroot/boot/grub
	cp bin/mso.bin isoroot/boot/mso.bin
	cp src/boot/grub.cfg isoroot/boot/grub/grub.cfg
	grub-mkrescue -o mso.iso isoroot
