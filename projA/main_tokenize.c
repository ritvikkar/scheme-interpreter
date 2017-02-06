/*
 * main_tokenize.c
 * CS 251, Carleton College, Fall 2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "value.h"
#include "tokenizer.h"
#include "linkedlist.h"
#include "talloc.h"

int main() 
{
    Value *list = tokenize();
    displayTokens(list);
    tfree();
}