/*
 * interpreter.h
 * CS 251, Carleton College, Fall 2016
 */

#include <stdlib.h>
#include "value.h"

#ifndef _INTERPRETER
#define _INTERPRETER

struct __Frame {
    structType id;
    Value *bindings;
    struct __Frame *parent;
    int mark; 
};

typedef struct __Frame Frame;

extern Frame *environment;

Frame *makeFrame();

void bindFunc(char *name, Value*(*function)(Value *), Frame *frame);

void bind(char *name, Value *value, Frame *frame);

/*
 * Takes a list of Scheme expressions, calls eval on each of them
 * in the top-level environment, and prints each result.
 */
void interpret(Value *tree, Frame *environment);

/*
 * Applies a function to a list of arguments.
 */
Value *apply(Value *function, Value *args);

/*
 * Takes the parse tree of a Scheme expression and an environment
 * frame in which to evaluate that expression. Returns a pointer to
 * a Value representing the value of that expression.
 */
Value *eval(Value *expr, Frame *frame);

void load(FILE *inputFile, Frame *frame);

#endif