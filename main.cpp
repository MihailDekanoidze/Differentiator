#include <stdio.h>
#include "./Differentiator.h"
#include "./include/Tree.h"

int main()
{
    Tree* main_tree = tree_create();
    Node* diff_tree = NULL; 
    assert(main_tree);

    Errors error = file_read("./input/expression.txt", main_tree);
    ERROR_CHECK(error, PROGRAMM_FINISH, ("Cannot processed file\n"));

    error = file_write("./output/after_read.txt", main_tree->root);
    ERROR_CHECK(error, PROGRAMM_FINISH, ("Cannot processed file\n"));
    PROGRAMM_FINISH;

    tree_optimize(main_tree->root);
    error = file_write("./output/after_optimize.txt", main_tree->root);
    ERROR_CHECK(error, PROGRAMM_FINISH, ("Cannot processed file\n"));

    diff_tree = diff_the_tree(main_tree->root);
    error = file_write("./output/diff_tree.txt", diff_tree);
    ERROR_CHECK(error, PROGRAMM_FINISH, ("Cannot processed file\n"));

    tree_optimize(diff_tree);
    error = file_write("./output/diff_tree_after_opt.txt", diff_tree);
    ERROR_CHECK(error, PROGRAMM_FINISH, ("Cannot processed file\n"));

    //tree_print(diff_tree, stdout);

    //file_write("output.txt", diff_tree);

    //printf("\nvar x was found in main tree = %d\n\n", var_search(main_tree->root));
    //printf("var x was found in diff tree = %d\n", var_search(diff_tree));

    //size_t changes = 0;
    //const_calculation(diff_tree, &changes);


    //printf("changes = %zu\n", changes);
    //file_write("after_optimisation.txt", diff_tree);

    PROGRAMM_FINISH;
}