/*
 * garbage.h
 * CS 251, Carleton College, Fall 2016
 */


#include <stdlib.h>
#include "value.h"
#include "interpreter.h"

#ifndef _GARBAGE
#define _GARBAGE


/*
 * Marks all values pointed to by values in active. Adds newly marked
 * values to active. Returns number of values marked.
 */
int markStep();

/* 
 * Adds topEnv to the list. If markStep returns 0, finish. Otherwise,
 * calls markStep on the updated list.
 */
void mark(Frame *topEnv);

/*
 * Sweeps through talloc's active list, freeing any unmarked memory.
 */
ActiveList *sweep(ActiveList *list);
                
#endif