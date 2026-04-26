#include "grep.h"
#include <stdio.h>
#include <strings.h>

int grep_match_line(const char *line, const GrepOptions *opts) {
  if (opts->ignore_cases) {
    return strcasestr(line, opts->pattern) != NULL;
  }
  return strstr(line, opts->pattern) != NULL;
}

void grep_search_file(FILE *fp, const char *filename, const GrepOptions *opts) {
  char buffer[1024];
  int line_number = 0;
  int match_count = 0;

  while (fgets(buffer, sizeof(buffer), fp) != NULL) {
    line_number++;

    if (grep_match_line(buffer, opts)) {
      match_count++;

      if (!opts->count_only) {
        if (opts->line_numbers) {
          printf("%s:%d:%s", filename, line_number, buffer);
        } else {
          printf("%s:%s", filename, buffer);
        }
      }
    }
  }

  if (opts->count_only) {
    printf("%s:%d\n", filename, match_count);
  }
}
