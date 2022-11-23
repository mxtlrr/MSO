override ALL_FILES := $(shell find ./ -type f -name '*.txt')

all:
	@../../bin/initrd-gen $(ALL_FILES)
	@echo copying current initrd to ../../
	@cp initrd.img ../../initrd.img -v
