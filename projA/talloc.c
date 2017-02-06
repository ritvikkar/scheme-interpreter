/*
 * talloc.c
 * CS 251, Carleton College, Fall 2016
 */

#include <stdlib.h>
#include <stdio.h>
#include "talloc.h"
#include "value.h"

ActiveList *list;
/*
 * A malloc-like function that allocates memory, tracking all
 * allocated pointers in the Active List.  (You can choose your
 * implementation of the Active List, but whatever it is, your talloc
 * code should NOT call functions in linkedlist.h; instead, implement
 * any list-like behavior directly here. Otherwise you'll end up with
 * circular dependencies, since you're going to modify the linked list
 * to use talloc instead of malloc.)
 */
void *talloc(size_t size, structType id){
    if(!list){
        list = (ActiveList *)malloc(sizeof(ActiveList));
		(*list).head = NULL;
    }
	Node *newHead;
    newHead = (Node *)malloc(sizeof(Node));
    if (!newHead) printf("we're out of memory\n");
	newHead -> value = (Value *)malloc(size);
	newHead -> next = list -> head;
    newHead -> id = id;
	list -> head = newHead;
	return newHead -> value;
}

void tfree(ActiveList *L)
{
    Node *current;
    current = L->head;
    while (current)
    {
        Node *next = current->next;
        switch(current -> id)
        {
            case VALUE:
                if(current->value)
                {   
                    free(current->value);
                }
                break;
            case FRAME:
                if(current->frame)
                {
                    free(current->frame);
                }
                break;
            default:
                break;
        }
        free(current);
        current = next;
    }
    free(L);
    L = NULL;
}


void freeNodes(ActiveList *L)
{
    Node *current;
    current = L->head;
    while (current)
    {
        Node *next = current->next;
        free(current);
        current = next;
    }
    free(L);
    L = NULL;
}
/*
 * A simple two-line function to stand in the C function "exit", which
 * calls tfree() and then exit().  (You'll use this later to allow a
 * clean exit from your interpreter when you encounter an error.)
 */
void texit(int status){
    tfree(list);
    exit(status);
}

void initialize(ActiveList *L){
    if(!L){
        L = (ActiveList *)malloc(sizeof(ActiveList));
        (*L).head = NULL;
    }
}

int insert(ActiveList *L, void *pointer, structType id){
    Node *newHead;
    newHead = (Node *)malloc(sizeof(Node));
    if(!newHead){
        return 1;
    }
    if (id == FRAME)
    {
        newHead -> id = FRAME;
        newHead -> frame = pointer;
    }
    else if(id == VALUE)
    {
        newHead -> id = VALUE;
        newHead -> value = pointer;
    }
    newHead -> next = L -> head;
    L -> head = newHead;
    return 0;
}

/*
* Attempts too free memory allocated to element. Returns 0 if successful and 1 if unsuccessful.
*/
int tfreeElement(Value *element){
    Node *prev = NULL;
    Node *current = list -> head;
    while(current){
        if(current -> value == element){
            if(prev){
                prev -> next = current -> next;
                free(current);
                return 0;
            }
            else{
                list -> head = current -> next;
                free(current);
                return 0;
            }
        }
        prev = current;
        current = current -> next;
    }
    return 1;
}