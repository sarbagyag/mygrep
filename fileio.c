#include <stdio.h>
#include <stdlib.h>
#include "grep.h"


void process_files(int argc, char **argv, int file_start, const GrepOptions *opts){
  if (file_start >= argc){
     grep_search_file(stdin,"stdin",opts);
     return;
  }

  for (int i=file_start; i<argc; i++){
    FILE *fp = fopen(argv[i], "r");
    if (fp==NULL){
      perror(argv[i]);
          continue;
    }

    grep_search_file(fp, argv[i], opts);
    fclose(fp);
  }
}
