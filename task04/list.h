#ifndef LIST_H
#define LIST_H

#include <stddef.h>

#define MAX_STRING_SIZE 100
#define ERR_SUCCESS 0
#define ERR_MEM_ALLOC 1
#define ERR_NULL_PTR_PASSED 2

typedef unsigned error_type;
typedef struct node node;
typedef struct list list;

node* create_node(node* next_node, const char* data_ptr, size_t data_size);
error_type list_create(list** lst);
error_type list_prepend(list** lst, const char* data_ptr, size_t data_size);
error_type list_append(list* lst, const char* data_ptr, const size_t data_size);
void list_delete(list* lst);
void list_print(list* lst);

#endif 
