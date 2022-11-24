#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct initrd_header{
  unsigned char magic; // 0xBF
  char name[64];
  unsigned int offset;
  unsigned int length;
};

int main(int argc, char **argv){
  int nheaders = (argc-1)/2;
  struct initrd_header headers[64];
  printf("size of header: %ld\n", sizeof(struct initrd_header));
  unsigned int off = sizeof(struct initrd_header)*64 + sizeof(int);
  int i;
  for(i = 0; i != nheaders; i++){
    printf("writing file %s as file %s in FS with offset 0x%x\n", argv[i*2+1], argv[i*2+2], off);
    
    // start copying the data
    
    /// name
    strcpy(headers[i].name, argv[i*2+2]);
    
    /// offset
    headers[i].offset = off;

    /// read file and get length
    FILE *stream = fopen(argv[i*2+1], "r");
    if(stream == 0) return 1;
    fseek(stream, 0, SEEK_END);
    headers[i].length = ftell(stream);
    off += headers[i].length;
    fclose(stream);

    /// write magic bytes
    headers[i].magic = 0xBF;

    printf("data for each file. file: \"%s\":\nmagic->0x%x\nlength->%d\n\n",
          argv[i*2+1], headers[i].magic, headers[i].length);
  }
   
  FILE *wstream = fopen("initrd.img", "w");
  unsigned char *data = (unsigned char *)malloc(off);
  fwrite(&nheaders, sizeof(int), 1, wstream);
  fwrite(headers, sizeof(struct initrd_header), 64, wstream);
  
  for(i = 0; i < nheaders; i++) {
    FILE *stream = fopen(argv[i*2+1], "r");
    unsigned char *buf = (unsigned char *)malloc(headers[i].length);
    fread(buf, 1, headers[i].length, stream);
    fwrite(buf, 1, headers[i].length, wstream);
    fclose(stream);
    free(buf);
  }
  
  fclose(wstream);
  free(data);
  
  return 0;
}
