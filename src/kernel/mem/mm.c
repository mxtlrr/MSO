#include "mem/mm.h"

block_t blocks[128];
int   iterators = 0;

uint32_t find_hole(block_t* b, uint32_t n){
  // b should be an array so we can get the length of the array
  // and loop through that.
  int sz = (sizeof(b)/sizeof(b[0]));
  for(int i = 0; i != 7; i++){ // assume 7 but idk
    if(b[i].size > n) return b[i].start_addr;
  }

  // If we're still here, either:
  // -> Not enough space
  // -> Allocated size too big
  return 0x0;
}

uint32_t malloc(block_t* b, uint32_t size){
  uint32_t hole = find_hole(b, size);
  if(hole == 0x0) return 0x0; // what!

  // Add to block
  blocks[iterators].start_addr = (uint32_t)hole;
  blocks[iterators].end_addr   = (uint32_t)hole+size;
  blocks[iterators].size       = (uint32_t)blocks[iterators].end_addr-blocks[iterators].start_addr;
  blocks[iterators].type       = 2;   // Reserved! [it is not free]
  
  iterators++;
  return (uint32_t)hole; // maybe
}
