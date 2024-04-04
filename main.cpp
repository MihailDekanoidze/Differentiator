#include <stdio.h>
#include "./Differentiator.h"
#include "./include/Tree.h"



int main()
{
    Tree* main_tree = tree_create(10);

    if (!file_read("tree.txt", main_tree)){return 1;}
    file_write("after_read.txt", main_tree->node_list);

    node_list_print(main_tree);

    file_write("Test_output.txt", main_tree->node_list);


    Tree* diff = tree_create(main_tree->node_count)

    Node* diff_tree = diff_the_tree(main_tree->node_list);


    tree_detor(main_tree);

}