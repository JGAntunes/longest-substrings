#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "suffix_tree.h"
#define LINE_BUFFER 256

void print_node_label (Node_T* node, char** text) {
  int start = node->path_start;
  int end = *(node->path_end);
  int i;
  printf("BEGIN: %d END: %d", start, end);
  for (i = start; i <= end; i++) {
    printf("%c", text[node->line_num][i]);
  }
}


void print_tree (Node_T* node, char** text, int level) {
  int i;
  for (i = 0; i < level; i++) {
    printf("\t");
  }
  printf("<- (%d ", node->line_num);
  if (!node->hook) {
    printf("ROOT");
  } else {
    print_node_label(node, text);
  }
  printf(")\n");
  if (node->brother) {
    print_tree(node->brother, text, level);
  }
  if (node->child) {
    print_tree(node->child, text, level+1);
  }
}
int main (int argc, const  char* argv[] ) {
  if (argc < 3) {
    printf("Correct usage ./<test_suffix_tree_exe> <in_file> <out_file>\n");
    return 1;
  }
  char ** text;
  char const* const in_file_name = argv[1];
  char const* const out_file_name = argv[2];
  int num_lines = 0;
  int* lines_length = 0;
  int i = 0;
  Node_T** suffix_tree;

  FILE* in_file = fopen(in_file_name, "r"); /* should check the result */
  FILE* out_file = fopen(out_file_name, "w"); /* should check the result */
  char line[LINE_BUFFER];
  fgets(line, sizeof(line), in_file);
  num_lines = atoi(line);
  text = (char**) malloc(num_lines * sizeof(char*));
  lines_length = (int*) malloc(num_lines * sizeof(int));
  for (i = 0; i < num_lines; i++) {
    fgets(line, sizeof(line), in_file);
    sscanf(line, "%d", &(lines_length[i]));
    /* don't forget terminator */
    text[i] = (char*) malloc ((lines_length[i] + 1) * sizeof(char));
    sscanf(line, "%*d %s", text[i]);
    /* correct string termination */
    text[i][lines_length[i]] = '\0';
  }
  suffix_tree = build_suffix_tree(text, num_lines, lines_length);
  print_tree(*suffix_tree, text, 0);

  fclose(in_file);
  fclose(out_file);

  return 0;
}
