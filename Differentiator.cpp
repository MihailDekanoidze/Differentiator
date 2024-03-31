#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "./include/Tree.h"
#include "./include/InputText.h"
#include "./Differentiator.h"


const node_data* null_op    = val_Operation(null_operator);
const node_data* Add        = val_Operation(add);
const node_data* Sub        = val_Operation(sub);      //array with funct node_datas
const node_data* Mul        = val_Operation(mul);
const node_data* Div        = val_Operation(divis);
const node_data* Sin        = val_Function(sin);
const node_data* Cos        = val_Function(cos);


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
            arg_scan(source, pos, new_node);      
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

        //print_arg(new_node);

        //printf("\n");
        
        new_node->left  = differentiator_tree_read(source, differentiator_tree, pos);
        skip_spaces(source, pos);
        (*pos)++;

        //printf("the next processing symbol is %c(%d)\n", *(source + *(pos)), *(source + *(pos)));
        //printf("left end for x");

        //print_arg(new_node);
        //printf("\n");

        //new_node = differentiator_tree->node_list + differentiator_tree->node_count - 1;

        skip_spaces(source, pos);
        operation_scan(source, pos, new_node);
        skip_spaces(source, pos);
        
        //printf("the next processing symbol is %c(%d)\n", *(source + *(pos)), *(source + *(pos)));
        //printf("the remained tree = \n%s\n", source + *(pos));
        //printf("the next processing symbol is %c(%d)\n", *(source + *(pos)), *(source + *(pos)));
        //printf("\nbegin right for ");
        //print_arg(new_node);
        //printf("\n");

        new_node->right = differentiator_tree_read(source, differentiator_tree, pos);
        skip_spaces(source, pos);
        (*pos)++;
        //printf("\nend right for");
        //print_arg(new_node);
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

Function get_funct_code(char* func)
{
    printf("get_funct_code has got funct <%c>\n", *func);

    switch (*func)
    {
    case 'S':
        return sin;
    case 'C':
        return cos;
    
    default:
        printf("Unknown func is %c\n", *func);
        return null_func;
    }
}

void operation_scan(char* source, size_t* pos, Node* curr_node)
{
    skip_spaces(source, pos); 

    Operation op = get_oper_code(source + *(pos));

    if (op)
    {
        curr_node->val->op = op;
        curr_node->data_type = operation;
    }
    else
    {
        curr_node->val->func = get_funct_code(source + *(pos));
        curr_node->data_type = func;
    }
    //printf("type is operation\n");
    //printf("operation is %c\n", *(source + *(pos)));
    (*pos)++;

    return;
}

void arg_scan(char* source, size_t* pos, Node* curr_node)
{
    (*pos)++;

    if (*(source + *(pos++)) == '#')
    {
        curr_node = NULL;
        return ;
    }

    int is_number = sscanf(source + *(pos), "%lg", &(curr_node->val->number));

    if (!is_number)
    {
        curr_node->data_type = var;
        curr_node->val->var = *(source + *pos);  
        
        printf("type is variable\n");
        printf("variable is %c\n", curr_node->val->var); 
    }
    else
    {
        curr_node->data_type = number;
        printf("type is number\n");
        printf("number is %lg\n", curr_node->val->number);
    }

    curr_node->left = NULL;
    curr_node->right = NULL;    

    skip_arg(source, pos);
    (*pos)++;

    return;
}


void skip_arg(char* source, size_t *pos)
{
    while ((*(source + *pos)) != '#') {(*pos)++;}
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
    print_arg(node);


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
        case add:
        case sub:
            return create_node(operation, node->val, DL, DR);
        case mul:
            {
                return _ADD(_MUL(DL, CR), _MUL(CL, DR));
            }
        case divis:
                return _DIV(_SUB(_MUL(DL, CR), _MUL(CL, DR)), _MUL(CR, CR));
        default:
            printf("Unxepected operation\n");
            return NULL;
        }
    }
    case func:
    {
        switch (node->val->func)
        {
        case sin:
            return _MUL(_COS(CL), DL);
        case cos:
            {
                node_data* Number = val_double(-1);
                Node* num_node = create_node(number, Number, NULL, NULL);
                Node* new_node =  _MUL(num_node, _MUL(_SIN(CL), DL)); ;
                free(Number);
                return new_node;
            }       
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
    //print_arg(node);

    memcpy(&copy_node->data_type, &node->data_type, sizeof(Type));
    memcpy(copy_node->val, node->val, sizeof(node_data));

    /*copy_node->data_type = node->data_type;
    copy_node->val = node->val;*/

    copy_node->left  = copy_tree(node->left);
    copy_node->right = copy_tree(node->right);

    return copy_node;
}

Node* create_node(Type data_type, const node_data* val, Node* left, Node* right)
{
    Node* new_node = (Node*) calloc(1, sizeof(Node));
    new_node->val = (node_data*)calloc(1, sizeof(node_data));

    printf("Before memcpy ");

    
    memcpy(&new_node->data_type, &data_type, sizeof(Type));
    memcpy(new_node->val, val, sizeof(node_data));

    //new_node->data_type = data_type;
    //new_node->val = val;
    printf("New_node val address is %p, val is ", &(new_node->val));
    print_arg(new_node);
    //free(val);

    printf("After memcpy ");

    //if(data_type == number) printf(" %lg ", new_node->val->number);         
    //else printf(" %c ", get_oper_symbol(new_node->val->op));

    printf("\n");


    new_node->left  = left;
    new_node->right = right;

    return new_node;   
}


