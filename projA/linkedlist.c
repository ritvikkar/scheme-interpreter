/*
 * linkedlist.c
 * CS 251, Carleton College, Fall 2016
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "talloc.h"
#include "value.h"
#include "linkedlist.h"

/*
 * Create an empty list (a new Value object of type NULL_TYPE).
 */
Value *makeNull()
{
    Value *v;
    v = (Value *)talloc(sizeof(Value), VALUE);
    if(!v) 
    {
        printf("We're out of memory!\n");
        return NULL;
    }
    v->id = VALUE;
    v->type = NULL_TYPE;
    v->mark = 0;
    return v;
}

/*
 * Create a nonempty list (a new Value object of type CONS_TYPE).
 */
Value *cons(Value *car, Value *cdr) //B
{
    Value *v;
    v = (Value *)talloc(sizeof(Value), VALUE);
    if(!v) 
    {
        printf("We're out of memory!\n");
        return NULL;
    }
    v->type = CONS_TYPE;
    v->c.car = car;
    v->c.cdr = cdr;
    return v;
}

/*
 * Print a representation of the contents of a linked list.
 */
void display(Value *list) //B
{
    //printf("[");
    Value *current = list;
    Value v;
    while (current->type == CONS_TYPE)
    {
        v = *current->c.car;
        switch (v.type)
        {
            case INT_TYPE:
                printf(" %i", v.i);
                break;
            case DOUBLE_TYPE:
                printf(" %f", v.d);
                break;
            case STR_TYPE:
                printf(" %s", v.s);
                break;
            case CONS_TYPE:
                printf("Found a cons type\n");
            	printf(" ");
            	display(&v);
                if(current -> c.cdr -> type != CONS_TYPE){
                    printf(" . ");
                    displayValue(current -> c.cdr);
                }
            	break;
            case BOOL_TYPE:
                printf("%s ", v.s);
                break;
            case SYMBOL_TYPE:
                printf("%s ", v.s);
                break;
            case CLOSURE_TYPE:
                printf("<procedure>");
                break;
            default:
                printf("type error\n");
        }
        current = current->c.cdr;
    }
    //printf(" ]\n");
}

void displayValue(Value *val)
{
    switch(val -> type) 
    {
        case INT_TYPE:
            printf("%d ", val -> i);
            break;
        case DOUBLE_TYPE:
            printf("%f ", val -> d);
            break;
        case STR_TYPE:
            printf("\"%s\" ", val -> s);
            break;
        case OPEN_TYPE:
            printf("%s ", val -> s);
            break;
        case CLOSE_TYPE:
            printf("%s ", val -> s);
            break;
        case BOOL_TYPE:
            printf("%s ", val -> s);
            break;
        case SYMBOL_TYPE:
            printf("%s ", val -> s);
            break;
        case CONS_TYPE:
            printf("( ");
            while (!isNull(val)){
                displayValue(car(val));
                if(!isNull(cdr(val)) && cdr(val) -> type != CONS_TYPE){
                    printf(". ");
                    displayValue(cdr(val));
                    break;
                }
                val = cdr(val);
            }
            printf(")");
            fflush(stdout);
            break;
        case NULL_TYPE:
            printf("() ");
            break;
        case VOID_TYPE:
            break;
        case CLOSURE_TYPE:
            printf("#<procedure>");
            break;
        case PRIMITIVE_TYPE:
            printf("primitive");
            break;
        default:
            printf("I shouldn't see this\n");
            break;
    }
}


/*
 * Get the car value of a given list.  (Uses assertions to ensure that 
 * list has a car value.)
 */
Value *car(Value *list)
{
    assert(list->type == CONS_TYPE); //assert that the list isn't empty
    return list->c.car;
}

/*
 * Get the cdr value of a given list.  (Uses assertions to ensure that
 * list has a cdr value.)
 */
Value *cdr(Value *list) //B
{
    assert(list->type == CONS_TYPE);
    return list->c.cdr;
}

/*
 * Test if the given value is a NULL_TYPE value.  (Uses assertions to
 * ensure that the input isn't NULL.)
 */
bool isNull(Value *value)//A
{
	assert(value);
	return value->type == NULL_TYPE;
}

/*
 * Create a new linked list whose entries correspond to the given
 * list's entries, but in reverse order.  The resulting list is a
 * shallow copy of the original -- that is, list entries are not
 * copied, but rather the new list's cons cells should point to
 * precisely the items in the original list.  (Uses assertions to
 * ensure that input is indeed a list.)
 */
Value *reverse(Value *list)
{
    assert(list);
    Value *reversed = makeNull();
    while(!isNull(list)){
    	reversed = cons(car(list), reversed);
    	list = cdr(list);
    }
    return reversed;
}

/*
 * Compute the length of the given list.  (Uses assertions to
 * ensure that input is indeed a list.)
 */
int length(Value *value)//A
{
	assert(value->type == CONS_TYPE || value->type == NULL_TYPE);
	if (value->type == NULL_TYPE){
		return 0;
	}
	else{
		return 1 + length(cdr(value));
	}

}
