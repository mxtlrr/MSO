#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef char  byte;
typedef short word;

#define MAGIC_NUM 0x3377
#define ENDING    0xFF

struct initrd_header {
  word magic;   // 0x3377
  char fname[64];
  uint32_t offset;
  uint32_t filesize;
  byte ending;  // always 0x00
};

int main(int argc, char** argv){
  int nfiles = argc-1;
  printf("I am adding %d files to the initrd file.\n", nfiles);
  
  struct initrd_header hdrs[nfiles];
  uint32_t off = sizeof(struct initrd_header) * nfiles + sizeof(int);

  // Write to the file
  for(int i = 0; i != nfiles; i++){
    printf("Offset of file '%s' -> 0x%x\n", argv[i+1], off);

    // copy data
    word magic  = MAGIC_NUM;
    byte ending = ENDING;

    /// write file name
    strcpy(hdrs[i].fname, argv[i+1]);

    /// write the file length to the header
    FILE* fp = fopen(argv[i+1], "r");
    if(!fp) return 1;
    fseek(fp, 0, SEEK_END); // go to the end of the file
    hdrs[i].filesize = ftell(fp); // tell me where i am
    fclose(fp);

    /// write the magic and the ending
    hdrs[i].magic  = magic;
    hdrs[i].ending = ending;

    printf("hdrs[i] data:\nmagic -> 0x%x\nending -> 0x%x\noffset -> 0x%x\nfile name -> %s\n\n",
          hdrs[i].magic, hdrs[i].ending, hdrs[i].offset, hdrs[i].fname);
  }

  // Now that we have the initrd headers, let's write it to the file
  FILE* _fp = fopen("./initrd.img", "w");
  uint8_t* data = (uint8_t*)malloc(off);
  fwrite(&nfiles, sizeof(int), 1, _fp);
  fwrite(hdrs, sizeof(struct initrd_header), nfiles, _fp);

  // write the header's stuff to the file
  for(int j = 0; j != nfiles; j++){
    FILE* fp = fopen(argv[j+1], "r");
    uint8_t* buf = (uint8_t*)malloc(hdrs[j].filesize);
    fread(buf,  1, hdrs[j].filesize, fp);
    fwrite(buf, 1, hdrs[j].filesize, _fp);
    fclose(fp);
    free(buf);
  }
  printf("Done! Initrd written to -> initrd.img\n");
  fclose(_fp);
  free(data);
}
