#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include "suffix_tree.h"
#include "suffix_tree_p.h"

int* longest_substring(Node_T** suffix_tree, int num_lines) {
  Node_T* root = *suffix_tree;
  /* this array represents the max substring length common to i-1 lines */
  int* max_common = (int*) malloc(num_lines * sizeof(int));
  int i = 0;
  for (i = 0; i < num_lines; i++) {
    max_common[i] = 0;
  }
  set_suffix_lines(root, num_lines);
  max_common_substring(root, max_common, 0);
  return (max_common);
}

void max_common_substring(Node_T* node, int* max_common, int depth) {
  int i;
  Node_T* child = node->child;
  int new_depth = node->hook ? depth + path_length(node) : depth;
  /* update max_common for each number of common lines based on the path_end */
  /* but only if it isn't root */
  if (node->hook) {
    for (i = 0; i < node->num_lines; i++) {
      if(new_depth > max_common[i]) max_common[i] = depth + path_length(node);
    }
  }
  /* iterate through child nodes */
  while (child) {
    max_common_substring(child, max_common, new_depth);
    child = child->brother;
  }
}

void set_suffix_lines(Node_T* node, int num_lines) {
  Node_T* parent_node = node;
  Node_T* current_node = parent_node->child;
  Node_T* brother = NULL;
  int i = 0;
  /* init suffix lines flags and total counter */
  bool* suffix_lines = (bool*) malloc(num_lines * sizeof(bool));
  for (i = 0; i < num_lines; i++) {
    suffix_lines[i] = false;
  }
  int total_lines = 0;

  /* this is a leaf node */
  if (!current_node) {
    suffix_lines[parent_node->line_num] = true;
    total_lines++;
    parent_node->lines = suffix_lines;
    parent_node->num_lines = total_lines;
    return;
  }

  /* visit childs */
  set_suffix_lines(current_node, num_lines);
  /* update total_lines on every loop */
  total_lines = 0;
  for (i = 0; i < num_lines; i++) {
    /* what's the line of this node? */
    if (!suffix_lines[i] && current_node->lines[i]) suffix_lines[i] = true;
    /* update total_lines */
    if (suffix_lines[i]) total_lines++;
  }

  /* recover some memory */
  free(current_node->lines);

  brother = current_node->brother;
  while (brother) {
    set_suffix_lines(brother, num_lines);
    /* update total_lines on every loop */
    total_lines = 0;
    for (i = 0; i < num_lines; i++) {
      /* what's the line of this node? */
      if (!suffix_lines[i] && brother->lines[i]) suffix_lines[i] = true;
      /* update total_lines */
      if (suffix_lines[i]) total_lines++;
    }
    /* recover some memory */
    free(brother->lines);
    brother = brother->brother;
  }
  /* update node */
  parent_node->lines = suffix_lines;
  parent_node->num_lines = total_lines;
  return;
}

Node_T** build_suffix_tree(char** text, int lines, int* length) {
  int i = 0;
  int j = 0;
  Node_T** suffix_tree = (Node_T**) malloc (sizeof(Node_T*));
  Point_T* point;
  init_suffix_tree(suffix_tree, &point);
  for (i = 0; i < lines; i++) {
    text[i][length[i]] = '\1'; /* force diferent terminators*/
    reset_point(point, i);
    for (j = 0; j <= length[i]; j++) {
      insert_char(point, i, j, text);
      /* printf("\n PHASE %d %d\n", i, j);
      print_tree(*suffix_tree, text, 0); */
    }
    text[i][length[i]] = '\0'; /* re-establish terminator*/
  }
  free(point);
  return suffix_tree;
}

void reset_point(Point_T* point, int line_num) {
  point->a = point->root;
  point->b = NULL;
  point->active_edge = -1;
  point->line_num = line_num;
  point->string_len = 0;
  point->char_pos = (int*) malloc (sizeof(int));
}

void init_suffix_tree(Node_T** root, Point_T** point) {
  *root = (Node_T*) malloc (sizeof(Node_T));
  *point = (Point_T*) malloc (sizeof(Point_T));
  /* init root */
  (*root) = new_node(NULL, 0, 0, NULL);
  (*root)->suffix_link = (*root);
  /* init point */
  (*point)->root = *root;
  reset_point(*point, 0);
  return;
}

Node_T* new_node(Node_T* root, int line_num, int start, int* end) {
  Node_T* node = (Node_T*) malloc(sizeof(Node_T));
  node->line_num = line_num;
  node->path_start = start;
  node->path_end = end;
  node->child = NULL;
  node->brother = NULL;
  /* nodes default suffix links to root */
  node->suffix_link = root;
  node->hook = NULL;
  return node;
}

Node_T* new_internal_node(Point_T* point) {
  int* split = (int*) malloc (sizeof(int));
  *split = point->b->path_start + point->string_len - 1;
  Node_T* internal_node = new_node(point->root, point->b->line_num, point->b->path_start, split);
  return internal_node;
}

void set_child(Node_T* parent_node, Node_T* new_node) {
  Node_T* current_node = parent_node->child;
  Node_T* temp_node = parent_node;
  /* node doesn't have any child */
  if (!current_node) {
    temp_node->child = new_node;
    new_node->hook = &(temp_node->child);
    return;
  }
  while (current_node) {
    temp_node = current_node;
    current_node = current_node->brother;
  }
  temp_node->brother = new_node;
  new_node->hook = &(temp_node->brother);
}

void set_internal_node(Point_T* point, Node_T* internal_node, Node_T* leaf_node) {
  /* maintain link between brothers if any */
  if (point->b->brother) point->b->brother->hook = point->b->hook;
  *(point->b->hook) = point->b->brother;
  point->b->brother = NULL;
  /* update start path for existing node*/
  point->b->path_start += point->string_len;
  /* add new active node child */
  set_child(point->a, internal_node);
  /* add new internal node childs */
  set_child(internal_node, point->b);
  set_child(internal_node, leaf_node);
  /* set correct active edge */
  point->b = internal_node;
}

void set_active_next_node(Point_T* point, char** text) {
  Node_T* current_node = point->a->child;
  while (current_node) {
    int line = current_node->line_num;
    int pos = current_node->path_start;
    if (text[line][pos] == text[point->line_num][point->active_edge]) {
      point->b = current_node;
      return;
    }
    current_node = current_node->brother;
  }
  point->b = NULL;
}

void set_point(Point_T* point, int next_char, char** text) {
  /* node is root if ain't got no hook*/
  if (!point->a->hook && point->string_len > 0) {
    /* decrement active length */
    point->string_len--;
    point->active_edge = next_char;
  }
  /* always follow suffix link, root defaults to itself*/
  point->a = point->a->suffix_link;
}

void suffix_link(Node_T** node_to_link, Node_T* dest_node) {
  /* there's a node waiting for a suffix link and active node isn't root */
  if (*node_to_link && dest_node->hook) {
    (*node_to_link)->suffix_link = dest_node;
    *node_to_link = NULL;
  }
}

int path_length(Node_T* node) {
  return *(node->path_end) - (node->path_start) + 1;
}

bool skip_count(Point_T* point, char** text) {
  int length = path_length(point->b);
  if (point->string_len >= length) {
    point->string_len -= length;
    point->active_edge += length;
    point->a = point->b;
    point->b = NULL;
    /* need to set active edge*/
    if (point->string_len > 0) {
      set_active_next_node(point, text);
    }
    return true;
  }
  return false;
}

void insert_char(Point_T* point, int line_num, int char_pos, char** text) {
  /* remaining suffixes waiting insertion */
  static int remain = 0;
  *(point->char_pos) = char_pos;
  remain++;

  /* new char to insert */
  char next_char = text[line_num][*(point->char_pos)];
  Node_T* internal_node = NULL;
  /* internal node waiting for suffix link */
  Node_T* last_internal_node = NULL;
  /* insert all suffixes */
  while (remain > 0) {

    /* find edge with next char */
    if (!point->string_len) {
      point->active_edge = *(point->char_pos);
    }
    set_active_next_node(point, text);

    /* rule 2 - no edge with next char*/
    if (!char_in_edge_start(point, text)) {
      set_child(point->a, new_node(point->root, line_num, *(point->char_pos), point->char_pos));
      suffix_link(&last_internal_node, point->a);
    } else {
      /* start from next node if active length is greater than edge length */
      if (skip_count(point, text)) continue;
      /* rule 3 - char exists on edge*/
      if (char_in_edge_next(point, next_char, text)) {
        suffix_link(&last_internal_node, point->a);
        point->string_len++;
        return;
      }
      /* rule 2 - edge split, new internal node and leaf node */
      internal_node = new_internal_node(point);
      set_internal_node(point, internal_node, new_node(point->root, line_num, *(point->char_pos), point->char_pos));
      suffix_link(&last_internal_node, internal_node);
      /* newly created internal node waiting for it's suffix link */
      last_internal_node = internal_node;
    }
    /* decrement suffix remain counter */
    remain--;
    /* correctly set the active point for next insertion */
    set_point(point, *(point->char_pos) - remain + 1, text);
  }
}

/* given a point and a char check if there's it exists on the active edge next point */
bool char_in_edge_next(Point_T* point, char new_char, char** text) {
  Node_T* next_node = point->b;
  int line = next_node->line_num;
  int pos = next_node->path_start + point->string_len;
  return (text[line][pos] == new_char);
}

/* given a point and a char check if there's an edge starting with it */
bool char_in_edge_start(Point_T* point, char** text) {
  if (!point->b) return false;
  Node_T* current_node = point->a->child;
  char new_char  = text[point->b->line_num][point->b->path_start];
  while (current_node) {
    int line = current_node->line_num;
    int pos = current_node->path_start;
    if (text[line][pos] == new_char) return true;
    current_node = current_node->brother;
  }
  return false;
}

void print_node_label (Node_T* node, char** text, int num_lines) {
  int start = node->path_start;
  int end = *(node->path_end);
  int i;
  printf("BEGIN: %d END: %d ", start, end);
  for (i = start; i <= end; i++) {
    printf("%c", text[node->line_num][i]);
  }
  printf("| COMMON LINES %d", node->num_lines);
}

void print_tree (Node_T* node, char** text, int num_lines, int level) {
  int i;
  for (i = 0; i < level; i++) {
    printf("\t");
  }
  printf("<- (LINE_NUM %d | ", node->line_num);
  if (!node->hook) {
    printf("ROOT");
  } else {
    print_node_label(node, text, num_lines);
  }
  printf(")\n");
  if (node->child) {
    print_tree(node->child, text, num_lines, level+1);
  }
  if (node->brother) {
    print_tree(node->brother, text, num_lines, level);
  }
}
