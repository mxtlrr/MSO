#include "mem/mmap.h"

block_t* getmmap(multiboot_info_t* mbd, uint32_t magic){
  if(magic != MAGIC_NUM) return NULL; // Probably using a different version of multiboot
  
  // Check if we have a correct mmap from GRUB
  if(!(mbd->flags >> 6 & 0x1)) return NULL; // We do not
  
  // Well, now we do.
  
  // Let's start looping!
  block_t b[mbd->mmap_length];
  multiboot_uint32_t i;
  for(i = 0; i < mbd->mmap_length; i += sizeof(multiboot_memory_map_t)){
    multiboot_memory_map_t* mmmt = (multiboot_memory_map_t*) (mbd->mmap_addr + i);
    
    // Set values
    b[i].begin_dir = mmmt->addr;
    b[i].end_dir   = mmmt->addr+mmmt->len;
    b[i].size      = mmmt->size;
    
    if(mmmt->type == 1) b[i].free = true;
    else b[i].free = false;
  }
  return b;
}
