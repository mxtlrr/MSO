# MSO
A simple operating system. It's not designed to be complicated.

# Compilation

```sh
# Only run make twice if you've done make clean.
make && make
```

## **Note**
For now I have not implemented a cross compiler. You need:
- GCC that supports `gcc -m32`
- GNU AS that lets you do `as --32`

*THIS WILL BE CHANGED IN THE FUTURE!*

# Roadmap

## Phase 0: Baby Steps

- [X] GDT
- [X] IDT
- [X] Output library
  - [X] Debugcon
  - [X] VGA
- [X] Memory Manager

## Phase 1: File system

- [X] Initrd
- [ ] VFS
  - [X] `fopen()`
  - [ ] `fread()`
  - [ ] `fwrite()`
  - [ ] Get file name.
