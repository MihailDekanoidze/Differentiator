#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "./include/Tree.h"
#include "./include/InputText.h"
#include "./Differentiator.h"


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
    return;
}

void file_write(const char* file_name, Node* root)
{
    FILE* output_tree = fopen(file_name, "w");

    size_t level = 0;
    tree_print(root, output_tree, &level);

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
        //PRINT_ARG(new_node);
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

    if (!is_number)
    {
        printf("Not a number!\n");
        return;
    }

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

VAL_FILLING(Operation, op, val->op = op);
//VAL_FILLING(double,  number, val->number = number);
VAL_FILLING(Function, func, val->func = func);
VAL_FILLING(char, var, val->var = var);

node_data* val_double(double data)         
{              
    union node_data* val = (node_data*)calloc(1, sizeof(node_data));    
    //memcpy(&(val->number), &data, sizeof(double));    
    val->number = data;   

    printf("I am crete a new node_data to address %p, val is %lg\n", &val, val->number);

    return val;                                                         
}



Node* diff_the_tree(const Node* node)
{
    ClearBuffer();
    PRINT_ARG(node);


    printf("left is null = %d\n", node->left == NULL);
    printf("right is null = %d\n", node->right == NULL);

    if(node->left != NULL)
        PRINT_ARG(node->left);

    if(node->right != NULL)
        PRINT_ARG(node->right);

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
        return _NUM(1);
    case operation:{
        switch (node->val->op)
        {
        case add:
        case sub:
            {
                node_data* Oper = val_Operation(node->val->op);
                Node* new_node = create_node(operation, node->val, DL, DR);;
                free(Oper);
                return new_node;
            }
            return create_node(operation, node->val, DL, DR);
        case mul:
            {
                node_data* Add = val_Operation(add);
                node_data* Mul = val_Operation(mul);

                Node* node_mul_left =  create_node(operation, Mul, DL, CR);
                Node* node_mul_right =  create_node(operation, Mul, CL, DR);

                Node* new_node = create_node(operation, Add, node_mul_left, node_mul_right);;

                free(Add);
                free(Mul);

                return new_node;
            }
        case divis:
            {
                node_data* Sub = val_Operation(sub);      //array with funct node_datas
                node_data* Mul = val_Operation(mul);
                node_data* Div = val_Operation(divis);

                Node* node_mul_left =  create_node(operation, Mul, DL, CR);
                Node* node_mul_right =  create_node(operation, Mul, CL, DR);
                Node* node_numerat = create_node(operation, Sub, node_mul_left, node_mul_right);
                Node* node_denomin = create_node(operation, Mul, CR, CR);

                Node* new_node = create_node(operation, Div, node_numerat, node_denomin);

                free(Sub);
                free(Mul);
                free(Div);

                return new_node;
            }
        default:
            printf("Unxepected operation\n");
            return NULL;
        }
    }
    case func:
    {
        switch (node->val->func)
        {
        case S:
            return _MUL(_COS(CL), DL);
        case C:
            return _MUL(_NUM(-1), _MUL(_SIN(CL), DL));        
        default:
            printf("Unxepected function\n");
            return NULL;
        }
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

    //printf("node val address is %p, val is ", &(node->val));
    //PRINT_ARG(node);

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
    new_node->val = (node_data*)calloc(1, sizeof(node_data));
    printf("??????????????\n");

    printf("Before memcpy ");

    if(data_type == number) printf(" %lg ", val->number);         
    else printf(" %c (%d)\n", get_oper_symbol(val->op), get_oper_symbol(val->op));

    memcpy(&new_node->data_type, &data_type, sizeof(Type));
    memcpy(new_node->val, val, sizeof(node_data));

    //new_node->data_type = data_type;
    //new_node->val = val;
    printf("New_node val address is %p, val is ", &(new_node->val));
    PRINT_ARG(new_node);
    //free(val);

    printf("After memcpy ");

    if(data_type == number) printf(" %lg ", new_node->val->number);         
    else printf(" %c ", get_oper_symbol(new_node->val->op));

    printf("\n");


    new_node->left  = left;
    new_node->right = right;

    return new_node;   
}