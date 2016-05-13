#ifndef SUFFIX_TREE_H
#define SUFFIX_TREE_H

typedef enum { false = 0, true = !false } bool;

/* Represents a suffix tree node. The usage of brother/child and hook
avoids the usage of a linked list for the children nodes.*/
typedef struct Node_T {
    int line_num;
    int path_start;
    int* path_end;
    struct Node_T* child;
    struct Node_T* brother; /* avoid linked list for brothers by using hook */
    struct Node_T* suffix_link;
    struct Node_T** hook; /* can be either child or brother */
    /* aux props to calculate longest substring */
    bool* lines;
    int num_lines;
} Node_T;

/* Position between to letters of a given branch */
typedef struct Point_T {
  Node_T* a;
  Node_T* b;
  /* maintain a pointer to the root node as an aux for operations */
  Node_T* root;
  int* char_pos;
  int string_len;
} Point_T;

Node_T** build_suffix_tree(char** text, int lines, int* length);

void print_tree (Node_T* node, char** text, int level);

int* longest_substring(Node_T** suffix_tree, int num_lines);

#endif
