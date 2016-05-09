#ifndef SUFFIX_TREE_H
#define SUFFIX_TREE_H

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
} Node_T;

/* Position between to letters of a given branch */
typedef struct Point_T {
  Node_T* a;
  Node_T* b;
  int string_len;
} Point_T;

typedef enum { false = 0, true = !false } bool;

Node_T** build_suffix_tree(char** text, int lines, int* length);

#endif
