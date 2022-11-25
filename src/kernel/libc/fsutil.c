#include "libc/fsutil.h"
#include "libc/string.h"

FILE* fopen(char* fp, char* mode, fs_node_t* fs_root){
  // check mode

  int _mode = 0;
  if(strcmp(mode, "r") == 0) _mode = MODE_R;
  else if(strcmp(mode, "w") == 0) _mode = MODE_W;
  
  // i am gonna have to reuse this code like 3 times.
  int file_is_found = 0;
  struct dirent* node = 0;
  for(int i = 0; (node = readdir_fs(fs_root, i)) != 0; i+=2){
    if(strcmp(node->name, fp) == 0){
      file_is_found = 1;
      break;
    } else continue;
  }

  FILE* f;
  f->mode = _mode;
  f->f    = fp;

  if(file_is_found == 0) return NULL;
  return f;
}
