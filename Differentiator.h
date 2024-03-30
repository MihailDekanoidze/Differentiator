#ifndef DIFFERENTIATOR_H
#define DIFFERENTIATOR_H

#include <stdio.h>
#include "./include/Tree.h"



void file_read(const char* file_name, Tree* tree);
Node* differentiator_tree_read(char* source, Tree* akinator_tree, size_t* pos);
void operator_scan(char* source, size_t* pos, Node* curr_node);
void number_scan(char* source, size_t* pos, Node* curr_node);

Operation get_oper_code(char* source);
void skip_number(char* source, size_t *pos);
size_t nsymbol_in_str(char* source, char symbol);

#endif