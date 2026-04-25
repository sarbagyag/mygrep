#ifndef GREP_H
#define GREP_H

#include <stdio.h>

typedef struct{
  int line_numbers;
  int ignore_cases;
  int count_only;
  const char *pattern;
} GrepOptions;

//grep.c 

int grep_match_line(const char *line, const GrepOptions *opts);
void grep_search_file(FILE *fp, const char *filename, const GrepOptions *opts);

//fileio.c 
void process_files(int argc, char *argv[], int file_start, const GrepOptions *opts);

#endif


