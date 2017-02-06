/*
 * talloc.h
 * CS 251, Carleton College, Fall 2016
 */

#include <stdlib.h>
#include "value.h"
#include "interpreter.h"

#ifndef _TALLOC
#define _TALLOC

typedef struct __Node Node;

struct __Node {
    structType id;
    Node *next;
    union {
    Value *value;
    Frame *frame;
    };
};

struct __ActiveList {
    Node *head;
};

typedef struct __ActiveList ActiveList;

extern ActiveList *list;

void initialize(ActiveList *L);

int insert(ActiveList *L, void *pointer, structType id);

void freeNodes(ActiveList *L);

/*
 * A malloc-like function that allocates memory, tracking all
 * allocated pointers in the Active List.  (You can choose your
 * implementation of the Active List, but whatever it is, your talloc
 * code should NOT call functions in linkedlist.h; instead, implement
 * any list-like behavior directly here. Otherwise you'll end up with
 * circular dependencies, since you're going to modify the linked list
 * to use talloc instead of malloc.)
 */
void *talloc(size_t size, structType id);

/*
 * Free all pointers allocated by talloc, as well as whatever memory
 * you malloc'ed to create/update the Active List.
 */
void tfree(ActiveList *L);

/*
 * A simple two-line function to stand in the C function "exit", which
 * calls tfree() and then exit().  (You'll use this later to allow a
 * clean exit from your interpreter when you encounter an error.)
 */
void texit(int status);

#endif

