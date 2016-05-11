#ifndef SUFFIX_TREE_PRIVATE_H
#define SUFFIX_TREE_PRIVATE_H

void init_suffix_tree(Node_T** root, Point_T** point);

void reset_point(Point_T* point);

Node_T* new_node(int line_num, int start, int* end);

Node_T* new_internal_node(int line_num, Point_T* point);

void set_child(Node_T* parent_node, Node_T* new_node);

void set_internal_node(Point_T* point, Node_T* internal_node, Node_T* leaf_node);

void set_active_edge(Point_T* point, char new_char, char** text);

void set_point(Point_T* point, char next_char, char** text);

void suffix_link(Node_T** node_to_link, Node_T* dest_node);

int path_length(Node_T* node);

bool skip_count(Point_T* point, char new_char, char** text);

void insert_char(Point_T* point, int line_num, int char_pos, char** text);

bool char_in_edge_next(Point_T* point, char new_char, char** text);

bool char_in_edge_start(Point_T* point, char** text);

#endif
