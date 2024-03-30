#include <stdio.h>
#include "./Differentiator.h"
#include "./include/Tree.h"



int main()
{
    Tree* main_tree = tree_create(10);

    file_read("tree.txt", main_tree);

    FILE* read_tree = fopen("after_read.txt", "w+");

    size_t level = 0;
    tree_print(main_tree->node_list, read_tree, &level);
    fclose(read_tree);

    //node_list_print(main_tree);

    Node* diff_tree = diff_the_tree(main_tree->node_list);
    tree_detor(main_tree);

    /*size_t level = 0;
    tree_print(diff_tree, main_tree->tree_log, &level);*/
    
    //level = 0;
    file_write("output.txt", diff_tree);

    node_dtor(diff_tree);


    //node_dtor(main_tree->node_list);

                                                                                                                                                                                                         printf ("Миша дурак!\n");
}