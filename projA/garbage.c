/*
 * garbage.c
 * CS 251, Carleton College, Fall 2016
 */

#include <stdio.h>
#include <string.h>
#include "value.h"
#include "linkedlist.h"
#include "talloc.h"
#include "interpreter.h"
#include "garbage.h"

ActiveList *active;
ActiveList *new;

/*
 * Marks all values pointed to by pointers in active. Adds newly marked
 * values to active. Returns number of values marked.
 */
int markStep()
{
    new = (ActiveList *)malloc(sizeof(ActiveList));
    (*new).head = NULL;
    int marks = 0;
    Node *prev = NULL;   
    Node *current = active -> head;
    while(current)
    {
        if(current -> id == VALUE)
        {
            switch(current -> value -> type)
            {
                case CONS_TYPE:
                    insert(new, current -> value -> c.car, VALUE);
                    current -> value -> c.car -> mark = 1;
                    marks = marks + 1;
                    if (current -> value -> c.cdr)
                    {
                        insert(new, current -> value -> c.cdr, VALUE);
                        current -> value -> c.cdr -> mark = 1;
                        marks = marks + 1;
                    }
                    break;
                case CLOSURE_TYPE:
                    insert(new, current -> value -> l.params, VALUE);
                    current -> value -> l.params -> mark = 1;
                    marks = marks + 1;
                    insert(new, current -> value -> l.body, VALUE);
                    current -> value -> l.body -> mark = 1;
                    marks = marks + 1;
                    if (current -> value -> l.pointer)
                    {
                        insert(new, current -> value -> l.pointer, FRAME);
                        marks = marks + 1;
                    }
                    break;
                case STR_TYPE: 
                case INT_TYPE: 
                case PRIMITIVE_TYPE: 
                case DOUBLE_TYPE: 
                case BOOL_TYPE: 
                case SYMBOL_TYPE: 
                    break;
                default:
                    break;
            }
        }
        else if (current -> id == FRAME)
        {
            if(current -> frame == environment)
            {
                if(current -> frame -> mark == 0)
                {
                    insert(new, current -> frame -> bindings, VALUE);
                    current -> frame -> bindings -> mark = 1;
                    marks = marks + 1;
                }
            }
            else 
            {
                if (current -> frame -> parent)
                {
                    insert(new, current -> frame -> parent, FRAME);
                    current -> frame -> parent -> mark = 1;
                    marks = marks + 1;
                }
                if (current -> frame -> bindings)
                {
                    insert(new, current -> frame -> bindings, VALUE);
                    current -> frame -> bindings -> mark = 1;
                    marks = marks + 1;
                }
            }
            current -> frame -> mark = 1;
        }
        else
        {
            fprintf(stderr, "Error: unrecognized struct\n");
        }
        current = current -> next;
    }
    freeNodes(active);
    active = (ActiveList *)malloc(sizeof(ActiveList));
    active = new;
    return marks;
}

/* 
 * Adds topEnv to the list. If markStep returns 0, finish. Otherwise,
 * calls markStep on the updated list.
 */
void mark(Frame *topEnv)
{
    int finish = 1;
    active = (ActiveList *)malloc(sizeof(ActiveList));
    (*active).head = NULL;
    insert(active, topEnv, FRAME);
    while (finish != 0)
    {
        finish = markStep();
    }
}

ActiveList *sweep(ActiveList *list)
{
    Node *prev = NULL;
    Node *current = list -> head;
    Node *next;
    while(current){
        next = current -> next;
        switch(current -> id)
        {
            case VALUE:
                if (current -> value -> mark == 0)
                {
                    if(prev){
                        prev -> next = current -> next;
                        if (current -> value)
                        {
                            free(current -> value);
                        }
                    }
                    else{
                        list -> head = current -> next;
                        if (current -> value)
                        {
                            free(current -> value);
                        }
                    }
                    free(current);
                }
                else
                {
                    current -> value -> mark = 0;
                    prev = current;
                }
                break;
            case FRAME:
                if (current -> frame -> mark == 0)
                {
                    if(prev)
                    {
                        prev -> next = current -> next;
                        if (current -> frame)
                        {
                            free(current -> frame);
                        }
                    }
                    else{
                        list -> head = current -> next;
                        if (current -> frame)
                        {
                            free(current -> frame);
                        }
                    }
                    free(current);
                }
                else
                {
                    current -> frame -> mark = 0;
                    prev = current;
                }
                break;
            case STR:
                break;
            default:
                break;
        }
        current = next;
    }
    return list;
}
