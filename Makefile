AS 	     := as
AS_FLAGS := --32

CC	 		 := gcc
CFLAGS	 := -m32 -ffreestanding -O2 -std=gnu99 -Iinclude/ -fno-stack-protector -nostdlib -g

# (i'm not good at GNU AS)
NASM_AS  := nasm
NASM_ASF := -felf32


all: clean mso mkiso

override OBJS := $(shell find ./obj/ -type f -name '*.o')

# Source files
override CFILES    := $(shell find ./src/ -type f -name '*.c')
override ASMFILES  := $(shell find ./include/ -type f -name '*.s')
override ASM2FILES := $(shell find ./src/kernel/ -type f -name '*.s')

clean:
	rm -rf bin/ mso.iso isoroot/ initrd.img

mso:
	mkdir -p obj/ bin/
	make -C initrd_src

	$(AS) $(AS_FLAGS) src/boot/boot.s -o obj/boot.o
	@$(foreach file, $(CFILES), $(CC) $(CFLAGS) -c $(file) -o obj/$(basename $(notdir $(file))).o; echo CC $(file);)
	@$(foreach file, $(ASMFILES), $(NASM_AS) $(NASM_ASF) $(file) -o obj/$(basename $(notdir $(file))).o; echo NASM $(file);)
	@$(foreach file, $(ASM2FILES), $(NASM_AS) $(NASM_ASF) $(file) -o obj/$(basename $(notdir $(file))).o; echo NASM $(file);)
	

	$(CC) -m32 -g -Tsrc/linker.ld -o bin/mso.bin -ffreestanding -O2 -nostdlib -fno-stack-protector $(OBJS) -lgcc

mkiso:
	mkdir -p isoroot/boot/grub
	cp bin/mso.bin isoroot/boot/mso.bin
	cp bin/initrd.img isoroot/boot/initrd.img
	cp src/boot/grub.cfg isoroot/boot/grub/grub.cfg
	grub-mkrescue -o mso.iso isoroot


run: mso.iso
	qemu-system-i386 -cdrom mso.iso -debugcon stdio

run-debug: mso.iso
	qemu-system-i386 -cdrom mso.iso -d int -M smm=off -no-shutdown -no-reboot -monitor stdio

