#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "./include/Tree.h"
#include "./include/InputText.h"
#include "./Differentiator.h"


int file_read(const char* file_name, Tree* tree)
{
    FILE* input_file = fopen(file_name, "rb");

    if (!input_file)
    {
        printf("File %s was not found\n", file_name);
        return 1;
    }

    TextInfo* file_buffer = (TextInfo*) calloc(1, sizeof(TextInfo));
    InputText(file_buffer, input_file);
    fclose(input_file);

    assert(tree);
    assert(tree->root == NULL);

    tree->root = get_G((char*)(file_buffer)->buffer);

    TextInfoDtor(file_buffer);
    return 0;
}

void file_write(const char* file_name, Node* root)
{
    FILE* output_tree = fopen(file_name, "w+");

    //printf("I plan to print tree to the file %s\n", file_name);
    tree_print(root, output_tree);

    fclose(output_tree);

    return;
}


Operation get_oper_code(char* source)
{
    //printf("symbol is %c\n", *source);
    //printf("int of symbol is %d\n", (int)(*source));

    switch ((int)(*source))
    {
    case '+':
        return add_op;
    case '-':
        return sub_op;
    case '*':
        return mul_op;
    case '/':
        return divis_op;
    default:
        break;
    }

    return null_op;
}

size_t nsymbol_in_str(char* source, char symbol)
{
    size_t i = 0;
    size_t curr_symbol_index = 0;

    while (source[curr_symbol_index])
        if (source[curr_symbol_index++] == symbol) i++;

    return i;
}

node_data* val_double(double data)         
{              
    union node_data* val = (node_data*)calloc(1, sizeof(node_data));    
    //memcpy(&(val->number), &data, sizeof(double));    
    val->number = data;   

    //printf("I am crete a new node_data to address %p, val is %lg\n", &val, val->number);

    return val;                                                         
}



Node* diff_the_tree(const Node* node)
{
    ClearBuffer();
    print_arg(node);
    printf("\n");

    printf("left is null = %d\n", node->left == NULL);
    printf("right is null = %d\n", node->right == NULL);

    if(node->left != NULL)
        print_arg(node->left);

    if(node->right != NULL)
        print_arg(node->right);

    switch (node->data_type)
    {
    case number:
        {
            node_data* Number = val_double(0);
            Node* new_node = create_node(number, Number, NULL, NULL);
            free(Number);

            return new_node;
        }
    case var:
        {
            node_data* Number = val_double(1);
            Node* new_node = create_node(number, Number, NULL, NULL);
            free(Number);

            return new_node;
        }  
    case operation:{
        switch (node->val->op)
        {
        case add_op:
        case sub_op:
            return create_node(operation, node->val, DL, DR);
        case mul_op:
            {
                return _ADD(_MUL(DL, CR), _MUL(CL, DR));
            }
        case divis_op:
                return _DIV(_SUB(_MUL(DL, CR), _MUL(CL, DR)), _MUL(CR, CR));
        case null_op:
        default:
            printf("Unxepected operation\n");
            return NULL;
        }
    }
    case func:
    {
        switch (node->val->func)
        {
        case sin_f:
            return _MUL(_COS(CR), DR);
        case cos_f:
            {
                node_data* Number = val_double(-1);
                Node* num_node = create_node(number, Number, NULL, NULL);
                Node* new_node =  _MUL(num_node, _MUL(_SIN(CR), DR)); ;
                free(Number);
                return new_node;
            }
        case null_f:       
        default:
            printf("Unxepected function\n");
            return NULL;
        }
    }
    case empty_node:
    {
        printf("Empty node\n");
        return NULL;
    }
    default:
        printf("Unxepected type\n");
        return NULL;
    }   
}

Node* copy_tree(const Node* node)
{
    if (node == NULL) return NULL;

    Node* copy_node = (Node*)calloc(1, sizeof(Node));
    copy_node->val = (node_data*)calloc(1, sizeof(node_data));

    printf("node val address is %p, val is ", &(node->val));
    print_arg(node);
    printf("\n");

    memcpy(&copy_node->data_type, &node->data_type, sizeof(Type));
    memcpy(copy_node->val, node->val, sizeof(node_data));

    /*copy_node->data_type = node->data_type;
    copy_node->val = node->val;*/

    copy_node->left  = copy_tree(node->left);
    copy_node->right = copy_tree(node->right);

    return copy_node;
}

Node* create_node(Type data_type, node_data* val, Node* left, Node* right)
{
    Node* new_node = (Node*) calloc(1, sizeof(Node));
    new_node->val = val;
    new_node->data_type = data_type;

    //printf("Before memcpy ");


    //new_node->data_type = data_type;
    //new_node->val = val;
    /*printf("New_node val address is %p, val is ", &(new_node->val));
    print_arg(new_node);
    printf("\n");*/
    //free(val);

    //printf("After memcpy ");

    //if(data_type == number) printf(" %lg ", new_node->val->number);         
    //else printf(" %c ", get_oper_symbol(new_node->val->op));

    //printf("\n");


    new_node->left  = left;
    new_node->right = right;

    return new_node;   
}

void const_calculation(Node* node, size_t* changes)
{
    int var_found = 0;

    if ((node->left == NULL) && (node->right == NULL))
    {
        return;
    }

    if ((node->data_type == operation) || (node->data_type == func))
    {
        var_found = var_search(node->left); 
        if (var_found == OBJECT_FOUND)
        {
            const_calculation(node->left, changes);
        }

        var_found |= var_search(node->left);
        if (var_found == OBJECT_FOUND)
        {
            const_calculation(node->right, changes);
        }
    } 

    if (var_found == OBJECT_FOUND)
    {
        return;
    }

    if (node->data_type == operation)
    {
        node->data_type = number;

        switch (node->val->op)
        {
        case add_op:
            node->val->number = node->left->val->number + node->right->val->number;
            break;
        case sub_op:
            node->val->number = node->left->val->number - node->right->val->number;
            break; 
        case mul_op:
            node->val->number = node->left->val->number * node->right->val->number;
            break;       
        case divis_op:
            node->val->number = node->left->val->number / node->right->val->number;
            break;        
        
        default:
            printf("Unknown operation\n");
            break;
        }

        node_dtor(node->left);
        node_dtor(node->right);

        node->left =  NULL;
        node->right = NULL;

        (*changes)++;
    }

    return;
}


void null_multiply(Node* node, size_t* changes)
{
    if (node == NULL) return;

    if ((node->data_type == operation) && (node->val->op == mul_op))
    {
        if ((node->left->data_type == number) && (node->left->val->number == (double)0))
        {
            node_dtor(node->right);
            node_dtor(node->left);

            (*changes)++;

            node->data_type = number;
            node->val->number = (double)0;

            node->left =  NULL;
            node->right = NULL;

            return;
        }
        if ((node->right->data_type == number) && (node->right->val->number == (double)0))
        {
            node_dtor(node->left);
            node_dtor(node->right);

            (*changes)++;

            node->data_type = number;
            node->val->number = (double)0;

            node->left =  NULL;
            node->right = NULL;

            return;
        }
    }

    return;
}



int var_search(Node* curr_node)
{
    if (curr_node == NULL) return OBJECT_N_FOUND;

    printf("Curr node is ");
    print_arg(curr_node);
    printf("\n");

    if (curr_node->data_type == var)
    {
        return OBJECT_FOUND;
    }

    int var_found = var_search(curr_node->left) | var_search(curr_node->right);

    printf("For node ");
    print_arg(curr_node);
    printf(" var_found = %d\n", var_found);

    if (var_found & OBJECT_FOUND)
    {
        return OBJECT_FOUND;
    }
    return OBJECT_N_FOUND;
}