#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "./include/Tree.h"
#include "./include/InputText.h"
#include "./Differentiator.h"


#define PRINT_ARG(curr_node) if(curr_node->data_type == number) printf(" %lg ", curr_node->val->number);         \
                        else printf(" %c ", get_oper_symbol(curr_node))


void file_read(const char* file_name, Tree* tree)
{
    FILE* input_file = fopen(file_name, "rb");

    TextInfo* file_buffer = (TextInfo*) calloc(1, sizeof(TextInfo));
    InputText(file_buffer, input_file);
    fclose(input_file);

    size_t count_node = nsymbol_in_str((char*)file_buffer->buffer, '{');

    while (tree->capacity < count_node)
        tree_increase_capasity(tree);

    size_t pos = 0;
    differentiator_tree_read((char*)(file_buffer)->buffer, tree, &pos);

    TextInfoDtor(file_buffer);

    FILE* output_tree = fopen("output.txt", "w");

    size_t level = 0;
    tree_print(tree->node_list, output_tree, &level);

    fclose(output_tree);

    return;
}

Node* differentiator_tree_read(char* source, Tree* differentiator_tree, size_t* pos)
{
    //printf("\nbegin read\n");
    //printf("the remained tree = \n%s\n", source + *(pos));

    skip_spaces(source, pos);

    //printf("the next symbol is %c(%d)\n", *(source + *(pos)), *(source + *(pos)));
    
    if (*(source + *(pos)) == '{')                  // reverse if 
    {
        //printf("processing symbol {\n");
        
        (*pos)++;
        
        skip_spaces(source, pos);
        
        //printf("the reamined tree = \n%s\n", source + *(pos));
        //printf("the next processing symbol is %c(%d)\n", *(source + *(pos)), *(source + *(pos)));
        
        /*if (*(source + *(pos)) == '}') 
        {   
            printf("processing symbol }\n");
            
            (*pos)++;
            
            printf("the next processing symbol is %c(%d)\n", *(source + *(pos)), *(source + *(pos)));
            printf("the remained tree = \n%s\n", source + *(pos));
            
            return NULL;
        }*/

        Node* new_node = differentiator_tree->node_list + (differentiator_tree->node_count++);
        new_node->val = (node_data*)calloc(1, sizeof(node_data));


        if (*(source + *(pos)) == '#')
        {
            number_scan(source, pos, new_node);      
            //printf("the remained tree = \n%s\n", source + *(pos));

            return new_node;
        }

        skip_spaces(source, pos);
        
        /*if (*(source + *(pos)) == '}') 
        {
            printf("the next symbol is '}'\n");
            
            new_node->left  = NULL;
            new_node->right = NULL;
            skip_spaces(source, pos);          
        }*/

        /*printf("the remained tree = \n%s\n", source + *(pos));
        printf("the next processing symbol is %c(%d)\n", *(source + *(pos)), *(source + *(pos)));
        printf("\nbegin left for ");*/

        //PRINT_ARG(new_node);

        //printf("\n");
        
        new_node->left  = differentiator_tree_read(source, differentiator_tree, pos);
        skip_spaces(source, pos);
        (*pos)++;

        //printf("the next processing symbol is %c(%d)\n", *(source + *(pos)), *(source + *(pos)));
        //printf("left end for x");

        //PRINT_ARG(new_node);
        //printf("\n");

        //new_node = differentiator_tree->node_list + differentiator_tree->node_count - 1;

        skip_spaces(source, pos);
        operator_scan(source, pos, new_node);
        skip_spaces(source, pos);
        
        //printf("the next processing symbol is %c(%d)\n", *(source + *(pos)), *(source + *(pos)));
        //printf("the remained tree = \n%s\n", source + *(pos));
        //printf("the next processing symbol is %c(%d)\n", *(source + *(pos)), *(source + *(pos)));
        //printf("\nbegin right for ");
        PRINT_ARG(new_node);
        //printf("\n");

        new_node->right = differentiator_tree_read(source, differentiator_tree, pos);
        skip_spaces(source, pos);
        (*pos)++;
        //printf("\nend right for");
        //PRINT_ARG(new_node);
        //printf("\n\n");

        //printf("the remained tree after arg scanned = \n%s\n", source + *(pos));

        //new_node = differentiator_tree->node_list + differentiator_tree->node_count - 1;
        
        return new_node;
    }
    else
    {
        //printf("symbol is not {, but %c(%d)\n", *(source + *(pos)), *(source + *(pos)));
        //printf("the remained tree = \n%s\n", source + *(pos));
        
        skip_spaces(source, pos);

        (*pos)++;
        return NULL;

        //printf("the remained tree = \n%s\n", source + *(pos));
    }

    //printf("Unexpected exit in pos = %zu\n", *pos);

    return NULL;
}
    
    //printf("Unexpected exit in pos = %zu\n", *pos);


Operation get_oper_code(char* source)
{
    //printf("symbol is %c\n", *source);
    //printf("int of symbol is %d\n", (int)(*source));

    switch ((int)(*source))
    {
    case '+':
        return add;
    case '-':
        return sub;
    case '*':
        return mul;
    case '/':
        return divis;
    default:
        break;
    }

    return null_operator;
}

void operator_scan(char* source, size_t* pos, Node* curr_node)
{
    skip_spaces(source, pos); 

    curr_node->val->op = get_oper_code(source + *(pos));
    curr_node->data_type = operation;
    //printf("type is operation\n");
    //printf("operation is %c\n", *(source + *(pos)));
    (*pos)++;

    return;
}

void number_scan(char* source, size_t* pos, Node* curr_node)
{
    (*pos)++;

    int is_number = sscanf(source + *(pos), "%lg", &(curr_node->val->number));

    //if (!is_number){printf("in number scan is not number");}
    curr_node->data_type = number;
    curr_node->left = NULL;
    curr_node->right = NULL;    

    //printf("type is number\n");
    //printf("number is %lg\n", curr_node->val->number);

    skip_number(source, pos);
    (*pos)++;

    return;
}


void skip_number(char* source, size_t *pos)
{
    while (((*(source + *pos)) <= '9') && ((*(source + *pos)) >= '0') 
    || ((*(source + *pos)) == '.')) {(*pos)++;}
}


size_t nsymbol_in_str(char* source, char symbol)
{
    size_t i = 0;
    size_t curr_symbol_index = 0;

    while (source[curr_symbol_index])
        if (source[curr_symbol_index++] == symbol) i++;

    return i;
}