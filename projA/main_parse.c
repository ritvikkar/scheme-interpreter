/* main_parse.c
 * CS 251, Carleton College, Fall 2016
 */
 
#include <stdio.h>
#include <stdlib.h>
#include "value.h"
#include "linkedlist.h"
#include "talloc.h"
#include "tokenizer.h"
#include "parser.h"


 int main() {
    Value *list = tokenize();
    Value *tree = parse(list);
    printTree(tree);
    printf("\n");
    tfree();
    return 0;
 }
