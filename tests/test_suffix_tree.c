#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "suffix_tree.h"
#define LINE_BUFFER 1024

int main (int argc, const  char* argv[] ) {

  char ** text;
  char* read_result = NULL;
  int num_lines = 0;
  int* lines_length = 0;
  int i = 0;
  Node_T** suffix_tree;
  int* max_common_substrings;

  char line[LINE_BUFFER];
  read_result = fgets(line, sizeof(line), stdin);
  if (!read_result) {
    printf("Reached end of file, buffer insufficient!");
  }

  num_lines = atoi(line);
  text = (char**) malloc(num_lines * sizeof(char*));
  lines_length = (int*) malloc(num_lines * sizeof(int));
  for (i = 0; i < num_lines; i++) {
    read_result = fgets(line, sizeof(line), stdin);
    if (!read_result) {
      printf("Reached end of file, buffer insufficient!");
    }
    sscanf(line, "%d", &(lines_length[i]));
    /* don't forget terminator */
    text[i] = (char*) malloc ((lines_length[i] + 1) * sizeof(char));
    sscanf(line, "%*d %s", text[i]);
    /* correct string termination */
    text[i][lines_length[i]] = '\0';
  }
  suffix_tree = build_suffix_tree(text, num_lines, lines_length);
  print_tree(*suffix_tree, text, 0);
  max_common_substrings = longest_substring(suffix_tree, num_lines);
  /* output to file */
  for (i= 0; i < num_lines - 1; i++) {
    printf("%d", max_common_substrings[i + 1]);
    if (i < num_lines - 2) printf(" ");
  }

  /* free memory */
  free(max_common_substrings);

  return 0;
}
