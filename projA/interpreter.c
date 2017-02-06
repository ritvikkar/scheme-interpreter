/* interpreter.c
 * CS 251, Carleton College, Fall 2016
 */
#include <string.h> 
#include <stdlib.h>
#include <stdio.h>
#include "tokenizer.h"
#include "value.h"
#include "linkedlist.h"
#include "parser.h"
#include "talloc.h"
#include "interpreter.h"
#include "garbage.h"

Frame *environment;

/*
* Primitive Functions start here
*/

Value *primitiveEq(Value *args)
{
    if(length(args) != 2)
    {
        fprintf(stderr, "eq?: arity mismatch;\nthe expected number of arguments does not match\nexpected: 2\ngiven: %d\n", length(args));
        texit(1);
    }

    Value *body = args;
    Value *result = makeNull();
    result -> type = BOOL_TYPE;
    result -> s = "#f";
   
    Value *curr = car(body);
    Value *next = car(cdr(body));
    
    if(curr->type!=next->type)
    {
        result -> s = "#f";
        return result;
    }
    
    if(curr->type==SYMBOL_TYPE || next->type==SYMBOL_TYPE)
    {
        if(&(curr->i) == &(next->i))
        {
            result -> s = "#t";
            return result;
        }
    }
    
    else
    {
        if(curr->type==INT_TYPE || curr->type==DOUBLE_TYPE)
        {
            if(curr->type==INT_TYPE)
            {
                if(curr->i==next->i)
                {
                    result -> s = "#t";
                    return result;
                }
            }
            else
            {
                if(curr->d==next->d)
                {
                    result -> s = "#t";
                    return result;
                }
            }
                
        }
        
        if(curr->s == next->s)
        {
            result -> s = "#t";
            return result;
        }
            
    }

    return result;
}

Value *primitiveEqual(Value *args)
{
    if(length(args) != 2)
    {
        fprintf(stderr, "equal?: arity mismatch;\nthe expected number of arguments does not match\nexpected: 2\ngiven: %d\n", length(args));
        texit(1);
    }

    Value *body = args;
    Value *result = makeNull();
    result -> type = BOOL_TYPE;
    result -> s = "#f";
   
    Value *curr = car(body);
    Value *next = car(cdr(body));
    
    if(curr->type!=next->type)
    {
        result -> s = "#f";
        return result;
    }
    if(curr->type==SYMBOL_TYPE || next->type==SYMBOL_TYPE)
    {
        if((curr->i) == (next->i))
        {
            result -> s = "#t";
            return result;
        }
    }
    
    else
    {
        if(curr->type==INT_TYPE || curr->type==DOUBLE_TYPE)
        {
            if(curr->type==INT_TYPE)
            {
                if(curr->i==next->i)
                {
                    result -> s = "#t";
                    return result;
                }
            }
                        
            else
            {
                if(curr->d==next->d)
                {
                    result -> s = "#t";
                    return result;
                }
            }
                
        }
                    
        else if(curr->type==CONS_TYPE)
        {
            while(!isNull(curr) && !isNull(next))
            {                
                if(car(curr) == car(next))
                {
                    curr = cdr(curr);
                    next = cdr(next);
                    result -> s = "#t";
                }
                else
                {
                    result -> s = "#t";
                    return result;
                    break;
                }
                
            }
        }

        
        else if(curr->s == next->s)
        {
            result -> s = "#t";
            return result;
        }
            
    }

    return result;
}

Value *primitiveIsNull(Value *args)
{
    if(length(args) != 1)
    {
        fprintf(stderr, "Error: arity mismatch, function null? takes one argument");
        texit(1);
    }
    Value *result = makeNull();
    result -> type = BOOL_TYPE;
    if (isNull(car(args)))
    {
        result -> s = "#t";
    }
    else
    {
        result -> s = "#f";
    }
    return result;
}

Value *primitiveCons(Value *args)
{
    if(length(args) != 2) 
    {
        fprintf(stderr, "Error: arity mismatch, function cons takes two arguments\n");
        texit(1);
    }
    return cons((car(args)), car(cdr(args)));
}

Value *primitiveCar(Value *args)
{
    Value *body = args;
    if(length(args) != 1)
    {
        fprintf(stderr, "Error: arity mismatch, function car takes one argument");
        texit(1);
    }
    if(car(body) -> type != CONS_TYPE)
    {
        fprintf(stderr, "Error: Expected pair for car\n");
        texit(1);
    }
    return car(car(args));
}

Value *primitiveCdr(Value *args)
{
    Value *body = args;
    if(length(args) != 1)
    {
        fprintf(stderr, "Error: arity mismatch, function cdr takes one argument");
        texit(1);
    }
    if(car(body) -> type != CONS_TYPE)
    {
        fprintf(stderr, "Error: Expected pair for cdr\n");
        texit(1);
    }
    return cdr(car(args));
}

Value *primitiveAdd(Value *args)
{
    Value *body = args;
    Value *sum = makeNull();

    Value *tempBody = args;

    int isDouble = 0;

    while(!isNull(tempBody))
    {
        Value *current = car(tempBody);

        if(current -> type == DOUBLE_TYPE)
        {
            sum -> type = DOUBLE_TYPE;
            sum -> d = 0.0;
            isDouble = 1;
            break;
        }

        else
        {
            sum -> type = INT_TYPE;
            sum -> i = 0;
        }

        tempBody = cdr(tempBody);
    }



    while(!isNull(body))
    {
        Value *current = car(body);

        if(isDouble == 1)
        {
            if(current -> type == INT_TYPE)
            {
                sum -> d = sum -> d + current -> i;
            }

            else
                sum -> d = sum -> d + current -> d;
        }

        else
            sum -> i = sum -> i + current -> i;

        body = cdr(body);
    }

    return sum;

}

Value *primitiveSub(Value *args)
{
    Value *body = args;

    Value *tempBody = args;
    Value *sub = makeNull();

    int isDouble = 0;

    while(!isNull(tempBody))
    {
        Value *current = car(tempBody);

        if(current -> type == DOUBLE_TYPE)
        {
            sub -> type = DOUBLE_TYPE;
            sub = car(body);
            //make the base value of the subtraction the first element
            isDouble = 1;
            break;
        }

        else
        {
            sub -> type = INT_TYPE;
            sub = car(body);
            //make the base value of the subtraction the first element

        }

        tempBody = cdr(tempBody);
    }


    body = cdr(body);
    //now sutract the rest of the expression from the first
    while(!isNull(body))
    {
        Value *current = car(body);

        if(isDouble == 1)
        {
            if(current -> type == INT_TYPE)
            {
                sub -> d = sub -> d - current -> i;
            }

            else
                sub -> d = sub -> d - current -> d;
        }

        else
            sub -> i = sub -> i - current -> i;

        body = cdr(body);
    }

    return sub;

}

Value *primitiveMul(Value *args)
{
    Value *body = args;

    Value *tempBody = args;
    Value *mult = makeNull();

    int isDouble = 0;

    while(!isNull(tempBody))
    {
        Value *current = car(tempBody);

        if(current -> type == DOUBLE_TYPE)
        {
            mult -> type = DOUBLE_TYPE;
            mult = car(body);
            //make the base value of the multiplication the first element
            isDouble = 1;
            break;
        }

        else
        {
            mult -> type = INT_TYPE;
            mult = car(body);
            //make the base value of the multiplication the first element

        }

        tempBody = cdr(tempBody);
    }
    body = cdr(body);
    //now multiply the rest of the expression from the first
    while(!isNull(body))
    {
        Value *current = car(body);

        if(isDouble == 1)
        {
            if(current -> type == INT_TYPE)
            {
                mult -> d = mult -> d * current -> i;
            }

            else
                mult -> d = mult -> d * current -> d;
        }

        else
            mult -> i = mult -> i * current -> i;

        body = cdr(body);
    }
    return mult;
}

Value *primitiveDiv(Value *args)
{
    Value *body = args;

    Value *tempBody = args;
    Value *div = makeNull();
    
    div -> type = DOUBLE_TYPE;
    if(car(body) -> type == INT_TYPE)
        div -> d = (double) car(body) -> i;
    else
        div -> d = car(body) -> d;
    
    body = cdr(body);
    while(!isNull(body))
    {
        Value *current = car(body);
        if(current -> d == 0.0 || current -> i == 0)
        {
            fprintf(stderr, "/: division by zero\n");
            texit(1);
        }
            
        if(current -> type == INT_TYPE)
            div -> d = div -> d / (double) current -> i;
        else
            div -> d = div -> d / current -> d;
        body = cdr(body);
    }
    
    if((int) div -> d - div -> d == 0)
    {
        div -> type = INT_TYPE;
        div -> i = div -> d;
    }
    
    return div;
}

Value *primitiveGreaterThan(Value *args)
{
    Value *body = args;
    Value *cond = makeNull();
    cond -> type = BOOL_TYPE;
    cond -> s = "#f";
    int isInt = 0;
    while(!isNull(cdr(body)))
    {
        Value *curr = makeNull();
        
        if(car(body) -> type == INT_TYPE) 
        {
            curr -> type = INT_TYPE;
            curr -> i = car(body) -> i;
            isInt = 1;
        }
        else
        {
            curr -> type = DOUBLE_TYPE;
            curr -> d = car(body) -> d;
        }
                
        Value *next = makeNull();
        next -> type = INT_TYPE;
        next -> d = car(cdr(body)) -> d;
        
        if(isInt == 1)
        {
            if(car(cdr(body)) -> type == INT_TYPE) 
            {
                if(curr -> i > car(cdr(body)) -> i)
                    cond -> s = "#t";
                else
                {
                    cond -> s = "#f";
                    break;
                }
            }
            
            else
            {
                if(curr -> i > car(cdr(body)) -> d)
                    cond -> s = "#t";
                else
                {
                    cond -> s = "#f";
                    break;
                }
            }
        }
        
        else
        {
            if(car(cdr(body)) -> type == INT_TYPE) 
            {
                if(curr -> d > car(cdr(body)) -> i)
                    cond -> s = "#t";
                else
                {
                    cond -> s = "#f";
                    break;
                }
            }
            
            else
            {
                if(curr -> d > car(cdr(body)) -> d)
                    cond -> s = "#t";
                else
                {
                    cond -> s = "#f";
                    break;
                }
            }
        }
            
        body = cdr(body);
    }
    return cond;
}

Value *primitiveGreaterThanEqual(Value *args)
{
    Value *body = args;
    Value *cond = makeNull();
    cond -> type = BOOL_TYPE;
    cond -> s = "#f";
    int isInt = 0;
    while(!isNull(cdr(body)))
    {
        Value *curr = makeNull();
        
        if(car(body) -> type == INT_TYPE) 
        {
            curr -> type = INT_TYPE;
            curr -> i = car(body) -> i;
            isInt = 1;
        }
        else
        {
            curr -> type = DOUBLE_TYPE;
            curr -> d = car(body) -> d;
        }
                
        Value *next = makeNull();
        next -> type = INT_TYPE;
        next -> d = car(cdr(body)) -> d;
        
        if(isInt == 1)
        {
            if(car(cdr(body)) -> type == INT_TYPE) 
            {
                if(curr -> i >= car(cdr(body)) -> i)
                    cond -> s = "#t";
                else
                {
                    cond -> s = "#f";
                    break;
                }
            }
            
            else
            {
                if(curr -> i >= car(cdr(body)) -> d)
                    cond -> s = "#t";
                else
                {
                    cond -> s = "#f";
                    break;
                }
            }
        }
        
        else
        {
            if(car(cdr(body)) -> type == INT_TYPE) 
            {
                if(curr -> d >= car(cdr(body)) -> i)
                    cond -> s = "#t";
                else
                {
                    cond -> s = "#f";
                    break;
                }
            }
            
            else
            {
                if(curr -> d >= car(cdr(body)) -> d)
                    cond -> s = "#t";
                else
                {
                    cond -> s = "#f";
                    break;
                }
            }
        }
            
        body = cdr(body);
    }
    return cond;
}

Value *primitiveLesserThan(Value *args)
{
    Value *body = args;
    Value *cond = makeNull();
    cond -> type = BOOL_TYPE;
    cond -> s = "#f";
    int isInt = 0;
    while(!isNull(cdr(body)))
    {
        Value *curr = makeNull();
        
        if(car(body) -> type == INT_TYPE) 
        {
            curr -> type = INT_TYPE;
            curr -> i = car(body) -> i;
            isInt = 1;
        }
        else
        {
            curr -> type = DOUBLE_TYPE;
            curr -> d = car(body) -> d;
        }
                
        Value *next = makeNull();
        next -> type = INT_TYPE;
        next -> d = car(cdr(body)) -> d;
        
        if(isInt == 1)
        {
            if(car(cdr(body)) -> type == INT_TYPE) 
            {
                if(curr -> i < car(cdr(body)) -> i)
                    cond -> s = "#t";
                else
                {
                    cond -> s = "#f";
                    break;
                }
            }
            
            else
            {
                if(curr -> i < car(cdr(body)) -> d)
                    cond -> s = "#t";
                else
                {
                    cond -> s = "#f";
                    break;
                }
            }
        }
        
        else
        {
            if(car(cdr(body)) -> type == INT_TYPE) 
            {
                if(curr -> d < car(cdr(body)) -> i)
                    cond -> s = "#t";
                else
                {
                    cond -> s = "#f";
                    break;
                }
            }
            
            else
            {
                if(curr -> d < car(cdr(body)) -> d)
                    cond -> s = "#t";
                else
                {
                    cond -> s = "#f";
                    break;
                }
            }
        }
            
        body = cdr(body);
    }
    return cond;
}

Value *primitiveLesserThanEqual(Value *args)
{
    Value *body = args;
    Value *cond = makeNull();
    cond -> type = BOOL_TYPE;
    cond -> s = "#f";
    int isInt = 0;
    while(!isNull(cdr(body)))
    {
        Value *curr = makeNull();
        
        if(car(body) -> type == INT_TYPE) 
        {
            curr -> type = INT_TYPE;
            curr -> i = car(body) -> i;
            isInt = 1;
        }
        else
        {
            curr -> type = DOUBLE_TYPE;
            curr -> d = car(body) -> d;
        }
                
        Value *next = makeNull();
        next -> type = INT_TYPE;
        next -> d = car(cdr(body)) -> d;
        
        if(isInt == 1)
        {
            if(car(cdr(body)) -> type == INT_TYPE) 
            {
                if(curr -> i <= car(cdr(body)) -> i)
                    cond -> s = "#t";
                else
                {
                    cond -> s = "#f";
                    break;
                }
            }
            
            else
            {
                if(curr -> i <= car(cdr(body)) -> d)
                    cond -> s = "#t";
                else
                {
                    cond -> s = "#f";
                    break;
                }
            }
        }
        
        else
        {
            if(car(cdr(body)) -> type == INT_TYPE) 
            {
                if(curr -> d <= car(cdr(body)) -> i)
                    cond -> s = "#t";
                else
                {
                    cond -> s = "#f";
                    break;
                }
            }
            
            else
            {
                if(curr -> d <= car(cdr(body)) -> d)
                    cond -> s = "#t";
                else
                {
                    cond -> s = "#f";
                    break;
                }
            }
        }
            
        body = cdr(body);
    }
    return cond;
}


Value *primitiveEquals(Value *args)
{
    Value *body = args;
    Value *result = makeNull();
    result -> type = BOOL_TYPE;
    result -> s = "#f";
    
    while(!isNull(cdr(body)))
    {
        Value *curr = car(body);
        Value *next = car(cdr(body));
        
        if(curr->type==INT_TYPE || curr->type==DOUBLE_TYPE ||
           curr->type==INT_TYPE || curr->type==DOUBLE_TYPE)
        {
            fprintf(stderr, "=:contract violation\nexpected: number?\n");
            texit(1);
        }
        
        else if(curr->type==INT_TYPE || curr->type==DOUBLE_TYPE)
        {
            if(curr->type==INT_TYPE && next->type==INT_TYPE)
            {
                if(curr->i==next->i)
                {
                    result -> s = "#t";
                }
                else
                    result -> s = "#f";
            }
            
            else if(curr->type==INT_TYPE && next->type==DOUBLE_TYPE)
            {  
                if(curr->i==next->d)
                {
                    result -> s = "#t";
                }
                else
                    result -> s = "#f";

            }
            
            else if(curr->type==DOUBLE_TYPE && next->type==INT_TYPE)
            {  
                if(curr->d==next->i)
                {
                    result -> s = "#t";
                }
                else
                    result -> s = "#f";
                
            }

            else if(curr->type==DOUBLE_TYPE && next->type==DOUBLE_TYPE)
            {  
                if(curr->d==next->d)
                {
                    result -> s = "#t";
                }
                else
                    result -> s = "#f";
            }
            
            else
            {
                result -> s = "#f";
                return result;
                break;
            }
        }


        else
        {
            fprintf(stderr, "=:contract violation\nexpected: number?\n");
            texit(1);
        }
        
        body = cdr(body);
    }

    return result;
}


/*
* Helper functions for Special Forms start here
*/

/*
* Takes in an if expression and a Frame, and returns
* the evaluated value of the expression.
*/
Value *ifHelper(Value *expr, Frame *frame){
    Value *query = eval((car(cdr(expr))), frame);
    if (query -> type == BOOL_TYPE && strcmp(query -> s, "#f"))
    {
        return eval((car(cdr(cdr(expr)))), frame);
    }
    else
    {
        return eval((car(cdr(cdr(cdr(expr))))), frame);
    }
}

/*
* Takes in a cond expression and a Frame, and returns
* the evaluated value of the expression.
*/
Value *condHelper(Value *expr, Frame *frame){
    Value *body = cdr(expr);
    while(!isNull(body))
    {
        Value *test = car(car(body));
        Value *then = car(cdr(car(body)));
        if(test -> type == SYMBOL_TYPE && !strcmp(test -> s, "else")){
            if(isNull(cdr(body))){
                return eval(then, frame);
            }
            else{
                fprintf(stderr, "Error: else clause must be last in cond statement.\n");
                texit(1);
            }
        }
        Value *result = eval(test, frame);
        if(result -> type == BOOL_TYPE && !strcmp(result -> s, "#f")){
            body = cdr(body);
        }
        else{
            return eval(then, frame);
        }
    }
    Value *voidReturn = makeNull();
    voidReturn -> type = VOID_TYPE;
    return voidReturn;
}

/*
* Takes in a let expression and a Frame, and returns
* the evaluated value of the expression.
*/
Value *letHelper(Value *expr, Frame *frame){
    Value *newBindings = car(cdr(expr));
    Value *body = cdr(cdr(expr));
    Frame *child = makeFrame();
    child -> parent = frame;
    child -> bindings = makeNull();
    while(!isNull(newBindings)){
        Value *binding = car(newBindings);
        if(binding -> type != CONS_TYPE || cdr(binding) -> type != CONS_TYPE){
            fprintf(stderr, "Error: Malformed bindings.\n");
            texit(1);
        }
        else{
            Value *name = car(binding);
            if(name -> type != SYMBOL_TYPE){
                fprintf(stderr, "Error: Cannot bind to non-symbol.\n");
                texit(1);
            }
            Value *value = eval(car(cdr(binding)), frame);
            bind(name -> s, value, child);
        }
        newBindings = cdr(newBindings);
    }
    if(isNull(body)){
        fprintf(stderr, "Error: No body in let expression.\n");
        texit(1);
    }
    while(!isNull(cdr(body))){
        eval(car(body), child);
        body = cdr(body);
    }
    return eval(car(body), child);
}

/*
* Takes in a let* expression and a Frame, and returns
* the evaluated value of the expression.
*/
Value *letstarHelper(Value *expr, Frame *frame){
    Frame *currentFrame = frame;
    Value *newBindings = car(cdr(expr));
    Value * body = cdr(cdr(expr));
    Frame *child = makeFrame();
    child -> parent = frame;
    child -> bindings = makeNull();
    while(!isNull(newBindings)){
        Value *binding = car(newBindings);
        Value *name = car(binding);
        Value *value = eval(car(cdr(binding)), child);
        if(name -> type != SYMBOL_TYPE){
            fprintf(stderr, "Error: Cannot bind to non-symbol.\n");
            texit(1);
        }
        bind(name -> s, value, child);
        newBindings = cdr(newBindings);
    }
    if(isNull(body)){
        fprintf(stderr, "Error: No body in let* expression.\n");
        texit(1);
    }
    while(!isNull(cdr(body))){
        eval(car(body), child);
        body = cdr(body);
    }
    return eval(car(body), child);
}

/*
* Takes in a letrec expression and a Frame, and returns
* the evaluated value of the expression.
*/
Value *letrecHelper(Value *expr, Frame *frame){
    Frame *currentFrame = frame;
    Value *newBindings = car(cdr(expr));
    Value * body = cdr(cdr(expr));
    while(!isNull(newBindings)){
        Value *binding = car(newBindings);
        Frame *child = makeFrame();
        child -> parent = currentFrame;
        child -> bindings = makeNull();
        Value *name = car(binding);
        Value *value = eval(car(cdr(binding)), currentFrame);
        if(name -> type != SYMBOL_TYPE){
            fprintf(stderr, "Error: Cannot bind to non-symbol.\n");
            texit(1);
        }
        bind(name -> s, value, child);
        currentFrame = child;
        newBindings = cdr(newBindings);
    }
    if(isNull(body)){
        fprintf(stderr, "Error: No body in letrec expression.\n");
        texit(1);
    }
    while(!isNull(cdr(body))){
        eval(car(body), currentFrame);
        body = cdr(body);
    }
    return eval(car(body), currentFrame);
}

/*
* Takes in a define expression and a Frame, binds the second element in expr
* to the first element in expr, and returns a void Value.
*/
Value *defineHelper(Value *expr, Frame *frame){
    Value *name = car(cdr(expr));
    Value *value = eval(car(cdr(cdr(expr))), frame);
    if(name -> type != SYMBOL_TYPE){
        fprintf(stderr, "Error: Cannot bind to non-symbol.\n");
        texit(1);
    }
    bind(name -> s, value, environment);
    Value *voidReturn = makeNull();
    voidReturn -> type = VOID_TYPE;
    return voidReturn;
}

/*
* Takes in a lambda expression and a frame, and returns
* the closure type Value created from this expression. 
*/
Value *lambdaHelper(Value * expr, Frame *frame){
    Value *newClosure = makeNull();
    newClosure -> type = CLOSURE_TYPE;
    newClosure -> l.params = car(cdr(expr));
    newClosure -> l.body = cdr(cdr(expr));
    newClosure -> l.pointer = frame;
    return newClosure;
}

/*
* Takes in an and expression and a Frame, and returns
* the evaluated value of the expression.
*/
Value *andHelper(Value *expr, Frame *frame){
    Value *body = cdr(expr);
    while (!isNull(body)){
        Value *current = car(body);
        Value *result = eval(current, frame);
        if(result -> type == BOOL_TYPE && !strcmp(result -> s, "#f")){
            return result;
        }
        body = cdr(body);
    }
    Value *result = makeNull();
    result -> type = BOOL_TYPE;
    result -> s = "#t";
    return result;
}

/*
* Takes in an or expression and a Frame, and returns
* the evaluated value of the expression.
*/
Value *orHelper(Value *expr, Frame *frame){
    Value *body = cdr(expr);
    while (!isNull(body)){
        Value *current = car(body);
        Value *result = eval(current, frame);
        if((result -> type == BOOL_TYPE && !strcmp(result -> s, "#t")) || result -> type != BOOL_TYPE){
            return result;
        }
        body = cdr(body);
    }
    Value *result = makeNull();
    result -> type = BOOL_TYPE;
    result -> s = "#f";
    return result;
}

/*
* Tries to bind the value of the second element of
* expr to the first element of expr. Fails and crashes
* if the first element of expr isn't an existing binding
* in frame, and returns a void Value on success.
*/
Value *setbangHelper(Value *expr, Frame *frame){
    Value *body = cdr(expr);
    Value *name = car(body);
    Value *value = eval(car(cdr(body)), frame);
    Value *currentBindings = frame -> bindings;
    Value *newBindings = makeNull();
    while(!isNull(currentBindings)){
        Value *binding = car(currentBindings);
        if(!strcmp(car(binding) -> s, name -> s)){
            Value *oldVal = car(cdr(binding));
            *oldVal = *value;
            if(isNull(cdr(cdr(body)))){
                Value *voidReturn = makeNull();
                voidReturn -> type = VOID_TYPE;
                return voidReturn;
            }
            else{
                return(eval(cdr(cdr(body)), frame));
            }
        }
        currentBindings = cdr(currentBindings); 
    }
    fprintf(stderr, "Error: Cannot set undefined variable.\n");
    texit(1);
}


/*
* Tallocs a new Frame and returns a pointer to it.
*/
Frame *makeFrame()
{
    Frame *f;
    f = (Frame *)talloc(sizeof(Frame), FRAME);
    if(!f) 
    {
        printf("We're out of memory!\n");
        return NULL;
    }
    f->id = FRAME;
    f->mark = 0;
    return f;
}

/*
* Creates a new two element list representing the binding of function
* to name and adds it to the list of bindings in frame.
*/
void bindFunc(char *name, Value*(*function)(Value *), Frame *frame)
{
    Value *value = makeNull();
    value -> type = PRIMITIVE_TYPE;
    value -> pf = function;
    Value *ref = makeNull();
    ref -> type = SYMBOL_TYPE;
    ref -> s = name;
    Value *newBinding = makeNull();
    newBinding -> type = CONS_TYPE;
    newBinding = cons(value, newBinding);
    newBinding = cons(ref, newBinding);
    frame -> bindings = cons(newBinding, frame -> bindings);
}

/*
* Creates a new two element list representing the binding of value
* to name and adds it to the list of bindings in frame.
*/
void bind(char *name, Value *value, Frame *frame){
    Value *newBinding = makeNull();
    newBinding -> type = CONS_TYPE;
    newBinding = cons(value, newBinding);
    Value *ref = makeNull();
    ref -> type = SYMBOL_TYPE;
    ref -> s = name;
    newBinding = cons(ref, newBinding);
    frame -> bindings = cons(newBinding, frame -> bindings);
}
    
/*
 * Applies a function to a list of arguments.
 */
Value *apply(Value *function, Value *args)    
{
    if (function -> type == CLOSURE_TYPE)
    {
        Frame *newFrame = makeFrame();
        newFrame -> parent = function -> l.pointer;
        newFrame -> bindings = makeNull();
        newFrame -> bindings -> type = CONS_TYPE;
        Value *formParams = function -> l.params;
        Value *actParams = args;
        if(formParams -> type == CONS_TYPE){
            if(length(formParams) != length(actParams))
            {
                fprintf(stderr, "arity mismatch, function takes %d arguments\n", length(formParams));
                texit(1);
            }
            while(!isNull(formParams) && !isNull(actParams)){
                Value *name = car(formParams);
                Value *value = car(actParams);
                if(name -> type != SYMBOL_TYPE){
                    fprintf(stderr, "Error: Cannot bind to non-symbol.");
                    texit(1);
                }
                bind(name -> s, value, newFrame);
                formParams = cdr(formParams);
                actParams = cdr(actParams);
            }
        }
        else{
            Value *name = formParams;
            Value *valList = makeNull();
            while(!isNull(actParams)){
                valList = cons(car(actParams), valList);
                actParams = cdr(actParams);    
            }
            if(name -> type != SYMBOL_TYPE){
                fprintf(stderr, "Error: Cannot bind to non-symbol.");
                texit(1);
            }
            bind(name -> s, reverse(valList), newFrame);
        }
        Value *body = function -> l.body;
        if(isNull(body)){
            fprintf(stderr, "Error: No body in function.\n");
            texit(1);
        }
        while(!isNull(cdr(body))){
                    eval(car(body), newFrame);
                    body = cdr(body);
        }
        return eval(car(body), newFrame);
    }
    else if (function -> type == PRIMITIVE_TYPE)
    {
        return function->pf(args);
    }
}

/*
* Tries to lookup the symbol in frame and all of its parents.
* Returns the value associated with expr if its found and
* crashes if an unknown symbol is encountered.
*/
Value *lookupSymbol(Value *expr, Frame *frame){
    if(expr -> type == SYMBOL_TYPE){
        while(frame){
            Value *localBindings = frame -> bindings;
            while(!isNull(localBindings)){
                Value *currentBinding = car(localBindings);
                Value *name;
                if(currentBinding && !isNull(currentBinding)){
                    name = car(currentBinding);
                }
                if(name && !strcmp(name -> s, expr -> s)){
                    return car(cdr(currentBinding));    
                }
                
                localBindings = cdr(localBindings);
                if(!localBindings || isNull(localBindings)){
                    break;
                }
            }
            if(!frame -> parent){
                fprintf(stderr, "Error: Symbol %s not found.\n", expr -> s);
                texit(1);
            }
            else{
                frame = frame -> parent;
            }
        }
    }
    else{
        fprintf(stderr, "Error: Cannot lookup non-symbol.\n");
        texit(1);
    }
    return expr;
}

 /*
 * Takes a list of Scheme expressions, calls eval on each of them
 * in the top-level environment, and prints each result.
 */
void interpret(Value *tree, Frame *frame)
{
    environment = frame;
    //Primitive functions only need to be bound once.
    if(isNull(environment -> bindings)){
        environment -> bindings -> type = CONS_TYPE;
        bindFunc("-", primitiveSub, environment);
        bindFunc("/", primitiveDiv, environment);
        bindFunc("+", primitiveAdd, environment);
        bindFunc("*", primitiveMul, environment);
        bindFunc("=", primitiveEquals, environment);
        bindFunc(">", primitiveGreaterThan, environment);
        bindFunc(">=", primitiveGreaterThanEqual, environment);    
        bindFunc("<", primitiveLesserThan, environment);
        bindFunc("<=", primitiveLesserThanEqual, environment);
        bindFunc("car", primitiveCar, environment);
        bindFunc("cdr", primitiveCdr, environment);
        bindFunc("cons", primitiveCons, environment);
        bindFunc("null?", primitiveIsNull, environment);
        bindFunc("eq?", primitiveEq, environment);
        bindFunc("equal?", primitiveEqual, environment);
    }

    while(!isNull(tree))
    {
        if(tree -> type == CONS_TYPE)
        {
            Value *v = eval(car(tree), environment);
            displayValue(v);
            if(v -> type != VOID_TYPE){
                printf("\n");
            }
            tree = cdr(tree);
        }
        else
        {
            Value *v = eval(tree, environment);
            displayValue(v);
        }
    }
}


/*
 * Takes the parse tree of a Scheme expression and an environment
 * frame in which to evaluate that expression. Returns a pointer to
 * a Value representing the value of that expression.
 */
Value *eval(Value *expr, Frame *frame)
{
    /* If code is a variable, look it up in the bindings of this frame.
     * If it's not in these bindings, look it up in the parent frame.
     * If it's not in the top-level frame, print an error
     */
    if (expr -> type == SYMBOL_TYPE)
    {
        Value *v = lookupSymbol(expr, frame);
        return v;
    }
    
    /* If code is a literal, return code */
    
    else if ((expr -> type == INT_TYPE) || (expr -> type == DOUBLE_TYPE) ||
        (expr -> type == STR_TYPE) || (expr -> type == BOOL_TYPE))
    {
        return expr;
    }
    
    else if (expr -> type == CONS_TYPE)
    {
            if (car(expr) -> type != SYMBOL_TYPE)
            {
                Value *value = eval(car(expr), frame);
                Value *rest = cdr(expr);
                if(value -> type != CLOSURE_TYPE && value -> type != PRIMITIVE_TYPE)
                {
                    fprintf(stderr, "Error: expected procedure\n");
                    texit(1);
                }
                else
                {
                    Value *args = makeNull();
                    while(!isNull(rest)){
                        Value *argument = eval(car(rest), frame);
                        args = cons(argument, args);
                        rest = cdr(rest);
                    }
                    return apply(value, reverse(args));
                }
            }
            
            /* If code is an if, evaluate its first expression. If that is not
             * false, evaluate and return its second expressions. Otherwise,
             * evaluate and return its third expression.
             */
            else if (!strcmp(car(expr) -> s, "if"))
            {
                return ifHelper(expr, frame);
            }
        
            else if (!strcmp(car(expr) -> s, "cond"))
            {
                 return condHelper(expr, frame);                
            }
            
            /* If code is a let, create a new frame with this frame as a parent.
            * Create new bindings in new frame. Evaluate the body of the let
            * in new frame.
            */
            else if (!strcmp(car(expr) -> s, "let"))
            {
                return letHelper(expr, frame);
            }
        
            else if (!strcmp(car(expr) -> s, "let*"))
            {
                return letstarHelper(expr, frame);
            }

            else if (!strcmp(car(expr) -> s, "letrec"))
            {
                return letrecHelper(expr, frame);
            }

            /* If code is define, evaluate the right hand side of the expression
             * in the current frame and create a new binding at the top-level frame 
             */
            else if (!strcmp(car(expr) -> s, "define"))
            {
                return defineHelper(expr, frame); 
            }
        
            else if (!strcmp(car(expr) -> s, "lambda"))
            {
                return lambdaHelper(expr, frame);
            }
        
            else if (!strcmp(car(expr) -> s, "and"))
            {
                return andHelper(expr, frame);
            }

            else if (!strcmp(car(expr) -> s, "or"))
            {
                return orHelper(expr, frame);
            }

            /* If code is quote, return code without quote */
            else if (!strcmp(car(expr) -> s, "quote"))
            {
                return (car(cdr(expr)));
            }
            
            else if (!strcmp(car(expr) -> s, "set!")){
                return setbangHelper(expr, frame);
            }
                       
            else 
            {
               /* this is not a special form */
                Value *function = eval(car(expr), frame);
                Value *args = cdr(expr);
                Value *evalArgs = makeNull();
                
                while (!isNull(args))
                {
                    evalArgs = cons((eval(car(args), frame)), evalArgs);
                    args = cdr(args);
                }
                evalArgs = reverse(evalArgs);
                return apply(function, evalArgs);
            }
        }
    return expr;
}

void load(FILE *inputFile, Frame *frame){
    if(fileno(inputFile)){
        Value *list = tokenizeHelper(inputFile);
        Value *tree = parse(list);
        interpret(tree, frame);   
    }
    else{
        printf("Starting interactive mode.\n");
        int i = 0;
        char input[2048];
        char c = 'c';
        char *t = &c;
        while(t){
            fputs("> ", stdout);
            t = fgets(input, 2048, stdin);
            size_t len = strlen(input);
            Value *code = tokenize(input, (int)len);
            Value *tree = parse(code);
            interpret(tree, frame);
        }
        texit(0);
    }
}