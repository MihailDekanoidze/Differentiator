#include <stdio.h>
#include "./Differentiator.h"
#include "./include/Tree.h"



int main()
{
    Tree* main_tree = tree_create(10);

    file_read("tree.txt", main_tree);
}