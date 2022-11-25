#pragma once

#include "fs/fs.h"
#include "fs/initrd.h"

#define MAX_BYTES_READ 0x800  // 2,048


// TODO: add more modes
enum MODES {
  MODE_R = 0, // read from the file
  MODE_W = 1
};

typedef struct {
  int mode;     // 0 or 1, or whatever is in MODES
  char* f;      // 
}__attribute__((packed)) FILE;

// possible entries for mode:
//  "r" -> MODE_R
//  "w" -> MODE_W

// See if we can open the file for reading/writing.
// If NULL returned, we can't open file (potentially
// doesn't exist)
FILE* fopen(char* fp, char* mode, fs_node_t* fs_root);

// Write data to stream. If it cannot,
// return NULL
void fwrite(FILE* stream, char* data, fs_node_t* fs_root);

// Read data from stream. Return value is an
// unsigned char array (we don't know if there
// is any binary in the file). It will only read
// "bytes" bytes.
uint8_t* fread(FILE* stream, uint32_t bytes, fs_node_t* fs_root);
