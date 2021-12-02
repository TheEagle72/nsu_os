#include "list.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct node
{
	struct node* next_node;
	char* data;
	size_t length; // not including '\0'
};

struct list
{
	node* head;
	node* tail;
};

node* create_node(node* next_node, const char* data, const size_t data_size)
{
	if (!data)
	{
		return NULL;
	}

	node* node_ptr = malloc(sizeof(node));
	if (!node_ptr)
	{
		return NULL;
	}

	char* data_ptr = malloc(sizeof(char) * data_size);
	if (!data_ptr)
	{
		free(node_ptr);
		return NULL;
	}
	memmove(data_ptr, data, data_size);

	node_ptr->next_node = next_node;
	node_ptr->data = data_ptr;
	return node_ptr;
}

error_type list_create(list** lst)
{
	list* list_ptr = malloc(sizeof(list));
	if (!list_ptr)
	{
		return ERR_MEM_ALLOC;
	}
	list_ptr->head = NULL;
	list_ptr->tail = NULL;

	*lst = list_ptr;

	return ERR_SUCCESS;
}

void list_delete(list* lst)
{
	if (!lst)
	{
		return;
	}

	node* current_node = lst->head;
	while (current_node)
	{
		node* tmp = current_node->next_node;
		free(current_node->data);
		free(current_node);
		current_node = tmp;
	}
}

error_type list_prepend(list** lst, const char* data_ptr, const size_t data_size)
{
	if (!lst)
	{
		return ERR_NULL_PTR_PASSED;
	}
	node* node_ptr = create_node((*lst)->head, data_ptr, data_size);
	if (!node_ptr)
	{
		return ERR_MEM_ALLOC;
	}
	node_ptr->next_node = (*lst)->head;
	node_ptr->length = data_size;
	(*lst)->head = node_ptr;
	if (!(*lst)->tail)
	{
		(*lst)->tail = node_ptr;
	}

	return ERR_SUCCESS;
}

error_type list_append(list* lst, const char* data_ptr, const size_t data_size)
{
	if (!lst)
	{
		return ERR_NULL_PTR_PASSED;
	}
	node* node_ptr = create_node(NULL, data_ptr, data_size);
	if (!node_ptr)
	{
		return ERR_MEM_ALLOC;
	}
	if (!lst->tail)
	{
		lst->head = node_ptr;
		lst->tail = node_ptr;
	}
	node_ptr->length = data_size;
	lst->tail->next_node = node_ptr;
	lst->tail = node_ptr;

	return ERR_SUCCESS;
}

void list_print(list* lst)
{
	if (!lst)
	{
		return;
	}

	node* current_node = lst->head;
	while (current_node)
	{
		for (size_t i = 0; i < current_node->length; ++i)
		{
			printf("%c", *(current_node->data + i));
		}
		current_node = current_node->next_node;
	}
}

error_type list_at(list** lst, const size_t data_size)
{

	return ERR_SUCCESS;
}
