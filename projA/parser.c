/* parser.c
 * CS 251, Carleton College, Fall 2016
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "tokenizer.h"
#include "value.h"
#include "linkedlist.h"
#include "parser.h"
#include "talloc.h"

/*
 * function should take a linked list of tokens from a Scheme 
 * program (as in the output of your tokenizer), and 
 * returns a pointer to a parse tree representing that program
 */
Value *parse(Value *tokens)
{
    int depth = 0;
    Value *stack = makeNull();
    while (!isNull(tokens)) 
    {
        if ((car(tokens)) -> type == CLOSE_TYPE)
        {
            tokens = cdr(tokens);
            Value *newFunction = makeNull();
            //This loop will never catch the unpaired ) case because the main loop stops executing
            //as soon as tokens is null.
            while (!isNull(stack) && car(stack) -> type != OPEN_TYPE)
            {
                newFunction = cons(car(stack), newFunction);
                stack = cdr(stack);
            }
            if (isNull(stack))
            {
                printf("syntax error 2\n");
                texit(1);
            }
            else if (car(stack) -> type == OPEN_TYPE) //encounter open paren
            {

                stack = cdr(stack);
                stack = cons(newFunction, stack);
                depth--;
            }
        }
        else if (car(tokens) -> type == OPEN_TYPE){
            depth++;
            stack = cons(car(tokens), stack);
            tokens = cdr(tokens);
        }
        else
        {
            stack = cons(car(tokens), stack);
            tokens = cdr(tokens);
        }
    }
    if (depth != 0)
    {
        printf("syntax error: unpaired \"(\"\n");
        texit(1);
    }
    return reverse(stack);
}

/*
 * function should display a parse tree to the screen, using 
 * parentheses to denote tree structure. (In other words, the 
 * output should visually look identical to legal Scheme code.)
 */
void printTree(Value *tree)
{
    Value *v;
    while (!isNull(tree))
    {   
        v = car(tree);
        switch(v -> type) 
        {
            case INT_TYPE:
                printf("%d ", v -> i);
                break;
            case DOUBLE_TYPE:
                printf("%f ", v -> d);
                break;
            case STR_TYPE:
                printf("%s ", v -> s);
                break;
            case OPEN_TYPE:
                printf("%s ", v -> s);
                break;
            case CLOSE_TYPE:
                printf("%s ", v -> s);
                break;
            case BOOL_TYPE:
                printf("%s ", v -> s);
                break;
            case SYMBOL_TYPE:
                printf("%s ", v -> s);
                break;
            case CONS_TYPE:
                //printf("( ");
            	printTree(cdr(v));
                //printf(")");
            	break;
            case NULL_TYPE:
                printf("null");
                break;
            default:
                printf("I shouldn't see this\n");
                break;
        }
        
        v = cdr(tree);
        
        if(v -> type == CONS_TYPE)
        {
            printTree(cdr(tree));
            break;
        }
        
        else
        {
            printf("");
            
            switch(v -> type) 
            {
                case INT_TYPE:
                    printf("%d ", v -> i);
                    break;
                case DOUBLE_TYPE:
                    printf("%f ", v -> d);
                    break;
                case STR_TYPE:
                    printf("%s ", v -> s);
                    break;
                case OPEN_TYPE:
                    printf("%s ", v -> s);
                    break;
                case CLOSE_TYPE:
                    printf("%s ", v -> s);
                    break;
                case BOOL_TYPE:
                    printf("%s ", v -> s);
                    break;
                case SYMBOL_TYPE:
                    printf("%s ", v -> s);
                    break;
                case CONS_TYPE:
                    printTree(cdr(v));
                    break;
                case NULL_TYPE:
                    printf("");
                    break;
                default:
                    printf("I shouldn't see this\n");
                    break;
            }
            break;
        }      
    }
}