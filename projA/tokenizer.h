/* tokenizer.h
 * CS 251, Carleton College, Fall 2016
 */

#include <stdlib.h>
#include "value.h"

#ifndef _TOKENIZER
#define _TOKENIZER

Value *tokenizeHelper(FILE *inputFile);

/*
 * Returns a linked list consisting of all tokens found in stdin (the input stream
 * in C), which your tokenize function should read in its entirety.
 */
Value *tokenize(char *input, int size);


/*
 * The function takes a linked list of tokens as input, and displays those 
 * tokens, one per line, with each token's type.
 */
void displayTokens(Value *list);

#endif