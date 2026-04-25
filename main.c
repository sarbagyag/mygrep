#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grep.h"

void usage(const char *program_name){
  fprintf(stderr, "Usage: %s [-n] [-i] [-c] pattern [file...]\n",program_name);
  exit(1);
}


int main(int argc, char *argv[]){
  if(argc<2){
    usage(argv[0]);
  }

  GrepOptions opts={0};
  int i=1;

  while(i<argc && argv[i][0]=='-'){
    if (strcmp(argv[i], "-n")==0){
      opts.line_numbers=1;
    }else if (strcmp(argv[i], "-i")==0){
       opts.ignore_cases=1;
    }else if (strcmp(argv[i], "-c")==0){
      opts.count_only=1;
    }else{
      fprintf(stderr, "Unknown flag: %s\n", argv[i]);
      usage(argv[0]);
    }
    i++;
  }

  if(i>=argc){
    fprintf(stderr, "Error: no pattern given\n");
    usage(argv[0]);

  }

  opts.pattern=argv[i];
  int file_start=i+1;

  process_files(argc, argv, file_start, &opts);

  return 0;

}







