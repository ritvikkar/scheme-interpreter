/* main.c
 * CS 251, Carleton College, Fall 2016
 */

#include <stdio.h>
#include <stdlib.h>
#include "value.h"
#include "linkedlist.h"
#include "talloc.h"
#include "tokenizer.h"
#include "parser.h"
#include "interpreter.h"

Frame *environment;

extern ActiveList *list;
int main(int argc, char *argv[]) {
    environment = talloc(sizeof(Frame), FRAME);
    environment -> parent = NULL;
    environment -> bindings = makeNull();
    for(int i = 1; i < argc; i++){
        load(fopen(argv[i], "r"), environment);
    }
    load(stdin, environment);
    tfree(list);
    return 0;
}