#include <stdio.h>
#include "value.h"
#include "parser.h"
#include "tokenizer.h"
#include "linkedlist.h"
#include "talloc.h"
#include "interpreter.h"
#include "garbage.h"

int main()
{
    Value *input = tokenize(stdin);
    Value *tree = parse(input);
    interpret(tree);
    mark(environment);
    list = sweep(list);
    tfree(list);
    return(0);
}