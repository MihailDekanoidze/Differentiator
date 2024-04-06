#include <stdio.h>
#include "./Differentiator.h"
#include "./include/Tree.h"



int main()
{
    Tree* main_tree = tree_create(10);

    if (file_read("expression.txt", main_tree)){return 1;}
    file_write("after_read.txt", main_tree->node_list);

    node_list_print(main_tree);

    file_write("Test_output.txt", main_tree->node_list);

    Node* diff_tree = diff_the_tree(main_tree->node_list);

    //tree_print(diff_tree, stdout);

    file_write("output.txt", diff_tree);

    printf("\nvar x was found in main tree = %d\n\n", var_search(main_tree->node_list));
    printf("var x was found in diff tree = %d\n", var_search(diff_tree));

    size_t changes = 0;
    const_calculation(diff_tree, &changes);


    printf("changes = %zu\n", changes);
    file_write("after_optimisation.txt", diff_tree);

    node_dtor(diff_tree);

    tree_detor(main_tree);

}