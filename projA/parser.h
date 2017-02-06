/* parser.h
 * CS 251, Carleton College, Fall 2016
 */

#include <stdlib.h>
#include "value.h"

#ifndef _PARSER
#define _PARSER

/*
 * function should take a linked list of tokens from a Scheme 
 * program (as in the output of your tokenizer), and 
 * returns a pointer to a parse tree representing that program
 */
Value *parse(Value *tokens);

/*
 * function should display a parse tree to the screen, using 
 * parentheses to denote tree structure. (In other words, the 
 * output should visually look identical to legal Scheme code.)
 */
void printTree(Value *tree);

#endif