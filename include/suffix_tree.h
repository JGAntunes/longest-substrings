#ifndef SUFFIX_TREE_H
#define SUFFIX_TREE_H

/* Represents a suffix tree node. The usage of brother/child and hook
avoids the usage of a linked list for the children nodes.*/
typedef struct Node_T {
    int string_num;
    int path_start;
    int string_len;
    struct Node_T* child;
    struct Node_T* brother; /* avoid linked list for brothers by using hook */
    struct Node_T* suffix_link;
    struct Node_T** hook; /* can be either child or brother */
} Node_T;

/* Position between to letters of a given branch */
typedef struct Point_T {
  Node_T* parent;
  Node_T* child;
  int string_len;
} Point_T;

typedef enum { false, true } bool;

void add_leaf(Point_T* point, int string_num, int char_num);

void suffix_link(Point_T* point);

#endif
