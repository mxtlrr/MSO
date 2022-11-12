#include "mem/mmap.h"
#include "libc/stdio.h"

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
    b[i].start_addr = mmmt->addr_low;
    b[i].end_addr   = mmmt->addr_high;
    b[i].size       = mmmt->addr_high - mmmt->addr_low;
    b[i].type       = mmmt->type;
  }
  return b;
}

void printmmap(multiboot_info_t* mbd, uint32_t magic){
  if(magic != MULTIBOOT_BOOTLOADER_MAGIC) return;
  if(!(mbd->flags >> 6 & 0x1)) return;
  
  for(multiboot_uint32_t i = 0; i < mbd->mmap_length; i += sizeof(multiboot_memory_map_t)){
    multiboot_memory_map_t* mmmt = (multiboot_memory_map_t*) (mbd->mmap_addr + i);
    printf("Start Addr: 0x%x | Length Low: 0x%x | Type: %d\n", 
        mmmt->addr_low, mmmt->len_low, mmmt->type);
  }
}
