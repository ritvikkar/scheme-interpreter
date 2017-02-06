/*
 * tokenizer.c
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
#include "talloc.h"

/*
 * The function takes a linked list of tokens as input, and displays those 
 * tokens, one per line, with each token's type.
 */

void displayTokens(Value *list)
{
    Value *v;
    while (!isNull(list))
    {
        v = car(list);
        switch(v -> type) 
        {
            case INT_TYPE:
                printf("%d:integer\n", v -> i);
                break;
            case DOUBLE_TYPE:
                printf("%f:double\n", v -> d);
                break;
            case STR_TYPE:
                printf("\"%s\":string\n", v -> s);
                break;
            case OPEN_TYPE:
                printf("%s:open\n", v -> s);
                break;
            case CLOSE_TYPE:
                printf("%s:close\n", v -> s);
                break;
            case BOOL_TYPE:
                printf("%s:boolean\n", v -> s);
                break;
            case SYMBOL_TYPE:
                printf("%s:symbol\n", v -> s);
                break;
            case CONS_TYPE:
                printf("cons\n");
                break;
            case NULL_TYPE:
                printf("null\n");
                break;
            default:
                printf("I shouldn't see this\n");
                break;
        }
        list = cdr(list);
    }
}


/*
* Finds the next unescaped quote in input after currentChar and creates
* a new STR_TYPE value with the relevant string. If no unescaped quote 
* is found, the program will crash with an error.
*/
int quoteHelper(int startingIndex, char *input, int inputSize){
    
    if (input[startingIndex] != '\"'){
        fprintf(stderr, "Error: String doesn't begin with quote.\n");
        texit(1);
        return 0;
    }
    int currentIndex = startingIndex + 1;
    while(currentIndex < inputSize) {
        if (input[currentIndex] == '\"'){
            return currentIndex;
        }
        else if (input[currentIndex] == '\\'){
            char nextChar = input[currentIndex + 1];
            switch (nextChar){
                case 'n' :
                    break;
                case 't' :
                    break;
                case '\"' :
                    break;
                case '\'' :
                    break;
                case '\\' :
                    break;
                default :
                    fprintf(stderr, "Error: escaped unescapable character \"%c\"\n", nextChar);
                    texit(1);
            }
            currentIndex++;
        }
        currentIndex++;
    }
    fprintf(stderr, "Error: Unterminated string.\n");
    texit(1);
    return 0;
}


/*
* Returns the last index of an integer or double from input.
* If no well-formed number is found, the program will crash with an error.
*/
int numHelper(int startingIndex, char *input, int inputSize){
    int containsDecimalPoint = 0;
    if (input[startingIndex] != '+' && input[startingIndex] != '-' && input[startingIndex] != '.' && !(input[startingIndex] > 47 && input[startingIndex] < 58)){
        fprintf(stderr, "Error: Number starting with illegal character %c.\n", input[startingIndex]);
        texit(1);
    }
    else if (input[startingIndex] == '.' && !(input[startingIndex + 1] > 47 && input[startingIndex + 1] < 58)){
        if(!(input[startingIndex + 1] > 47 && input[startingIndex + 1] < 58)){
            fprintf(stderr, "Error: Number has illegal character %c following decimal point.\n", input[startingIndex + 1]);
            texit(1);
        }
        else{
            containsDecimalPoint = 1;
        } 
    }
    int currentIndex = startingIndex + 1;
    while (currentIndex < inputSize){
        if (input[currentIndex] == ' ' || input[currentIndex] == '(' || input[currentIndex] == ')' || input[currentIndex] == '\t' || input[currentIndex] == '\n' || input[currentIndex] == ';'){
            return currentIndex - 1; 
        }
        else if (input[currentIndex] == '.'){
            if (containsDecimalPoint){
                fprintf(stderr, "Error: Number contains multiple decimal points.");
                texit(1);
            }
            else{
                containsDecimalPoint = 1;
            }
        }
        else if (!(input[currentIndex] > 47 && input[currentIndex] < 58)){
            fprintf(stderr, "Error: Number contains invalid character %c.\n", input[currentIndex]);
            texit(1);
        }          
        currentIndex++;
    }
    
    return 0;
}

int isSymbol(char *str, int startingPoint, int size) 
{
    int i;
    if(str[startingPoint] != 43 || str[startingPoint] != 45 || str[startingPoint] != 46)
    {
        for(i = startingPoint; i<size; i++)
        {
            char next = str[i+1];
            //a && z || A && Z
            if((str[i] >= 97 && str[i]<= 122) ||
               (str[i] >= 65 && str[i]<= 90))
            {            
                // ' ' || '\n' || ( || )
                if (next == 32 || next == '\n' || 
                    next == 40 || next == 41 || next == '\t' || next == ';')  
                {
                    return 1;
                }
            }//check for alphabets

            else if (next == 43 || next == 45 || 
                     next == 46)
            {
                return 1;
            }// + || - || .

            else if (next == 32 || next == '\n' || 
                     next == 40 || next == 41 || next == '\t' || next == ';')  
            {
                return 1;
            }// ' ' || '\n' || ( || )

            else if (str[i] >= 48 && str[i] <= 57)
            {
                // ' ' || '\n' || ( || )
                if (next == 32 || next == '\n' || 
                    next == 40 || next == 41 || next == '\t' || next == ';')  
                {
                    return 1;
                }
            } //check if its a number

            else if (str[i] == 33 || str[i] == 37 || 
                     str[i] == 38 || str[i] == 42 || 
                     str[i] == 47 || str[i] == 58 || 
                     str[i] == 60 || str[i] == 61 || 
                     str[i] == 62 || str[i] == 63 || 
                     str[i] ==126 || str[i] == 95 || 
                     str[i] == 94)
            {
                // ' ' || '\n' || ( || )
                if (next == 32 || next == '\n' || 
                    next == 40 || next == 41 || next == '\t' || next == ';')  
                {
                    return 1;
                }
            } //check if its any of these: ! $ % & * / : < = > ? ~ _ ^

            else
            {
                fprintf(stderr, "Error: %c followed by unacceptable character", str[i]);
                texit(1);
                return 0;
            }
        }
    }
    
    else
    {
        fprintf(stderr, "Error: The first character is unacceptable");
        return 0;
        texit(1);
    }
    
    return 0;
}

int returnSymbolLength(char *sym, int startingPoint, int size) 
{
    int length=0;
    for (int i=startingPoint; i < size; i++) 
    {
        if (sym[i+1]== ' ' || sym[i+1] == '\n' || 
            sym[i+1] == ')' || sym[i+1] == '(' ||
            sym[i+1] == '\t' || sym[i+1] == ';') 
        {
          return length;
        }
        
        length++;
    }
    return length;
}

char *returnSymbol(char *expression, int startingPoint, int length) 
{
    char *str;
    
    str=(char*)talloc(sizeof(char) * (3+length), STR);
    int j=startingPoint;
    int i;
    
    for (i=0; i<=length; i++)
    {
        str[i]=expression[j];
        j++;
    }
    str[i+1]= '\0';
    return str;
}

int isBoolean(char *str, int startingPoint) 
{
    if (str[startingPoint] == '#') 
    {
        if (str[startingPoint+1] == 't' || str[startingPoint+1] == 'f')
        {
            if(str[startingPoint+2] == ' ' || str[startingPoint+2] == '(' || 
                str[startingPoint+2] == ')' || str[startingPoint+2] == '\n' || 
                str[startingPoint+2] == '\t' || str[startingPoint+2] == ';')
                return 1;
        }
    }
    return 0;
}

char *returnBoolean(char *expression, int startingPoint) 
{
    char *boolean;    
    
    if(expression[startingPoint+1] == 't')
    {
        boolean = "#t";
    }
    

    if(expression[startingPoint+1] == 'f')
    {
        boolean = "#f";
    }
    
    return boolean;
}


Value *tokenizeHelper(FILE *inputFile){
    char *input = talloc(sizeof(char) * 100, STR);
    int size = 0; //size of the input
    int arrayLength = 100;
    char charRead = (char)fgetc(inputFile);
    while (charRead != EOF){
        if (size >= arrayLength)
        {
            arrayLength = 2 * arrayLength;
            char *new = talloc(sizeof(char) * (arrayLength), STR);
            int i;
            for (i = 0; i < size; i++){
                new[i] = input[i];
            }
            input = new;
        }
        input[size] = charRead;
        size += 1;
        charRead = (char)fgetc(inputFile);
    }
    size_t len = strlen(input);
    return tokenize(input, len);
}

/*
 * Returns a linked list consisting of all tokens found in stdin (the input stream
 * in C), which your tokenize function should read in its entirety.
 */
Value *tokenize(char *input, int size)
{
    Value *list = makeNull();
    int currentChar = 0;
    while (currentChar < size) {
        if (input[currentChar] == '\"') {
            Value *v = makeNull();
            int endPoint = quoteHelper(currentChar, input, size);
            int strLength = endPoint - currentChar + 1;
            char *str = talloc(sizeof(char) * strLength, STR);
            int i;
            int escapes = 0;
            for(i = 1; i < strLength - 1; i++){
                if (input[currentChar + i] == '\\'){
                    switch (input[currentChar + i + 1]){
                        case '\\':
                            str[i - 1] = '\\';
                            break;
                        case 'n':
                            str[i - 1] = '\n';
                            break;
                        case 't':
                            str[i - 1] = '\t';
                            break;
                        case '\"':
                            str[i - 1] = '\"';
                            break;
                        case '\'':
                            str[i - 1] = '\'';
                            break;
                        default:
                            fprintf(stderr, "Error: Illegal escape character.\n");
                            texit(1);
                    }
                    escapes++;
                    i++;
                }
                else {
                    str[i - 1 - escapes] = input[currentChar + i];
                }
            }
            str[i - 1] = '\0';
            list = cons(v, list);
            char *new = talloc(sizeof(char) * (strLength - escapes - 2), STR);
            for (i = 0; i < strLength - escapes - 2; i++){
                new[i] = str[i];
            }
            v -> type = STR_TYPE;
            v -> s = new;
            currentChar = endPoint + 1;
        } 
        else if (input[currentChar] == '\'')
            {
                Value *v = makeNull();
                v -> type = SYMBOL_TYPE;
                v -> s = "\'";
                list = cons(v, list);
            }
        else if (input[currentChar] == '+' || input[currentChar] == '-') {
            int next = currentChar + 1;
            if(next >= size){
                Value *v = makeNull();
                v -> type = SYMBOL_TYPE;
                v -> s = &input[currentChar];
                list = cons(v, list);
            }
            else{
                if(input[next] == ' ' || input[next] == '(' || input[next] == ')' || input[next] == '\n' || input[next] == '\t' || input[next] == ';'){
                    Value *v = makeNull();
                    v -> type = SYMBOL_TYPE;
                    char *ch;
                    if(input[currentChar] == '+'){
                        ch = "+";
                    }
                    else{
                        ch = "-";
                    }
                    v -> s = ch;
                    list = cons(v, list);
                }
                else if (input[next] == '.' || (input[next] > 47 && input[next] < 58)){
                    int endPoint = numHelper(currentChar, input, size);
                    int isFloat = 0;
                    char str[endPoint - currentChar + 1];
                    int i;
                    for (i = currentChar; i <= endPoint; i++){
                        if (input[i] == '.'){
                            isFloat = 1;
                        }
                        str[i - currentChar] = input[i];
                    }
                    str[i] = '\0';
                    Value *v = makeNull();
                    if (isFloat){
                        v -> type = DOUBLE_TYPE;
                        v -> d = atof(str);
                    }
                    else{
                        v -> type = INT_TYPE;
                        v -> i = atoi(str);
                    }
                    list = cons(v, list);
                    currentChar = endPoint;
                }
                else{
                    fprintf(stderr, "Error: %c followed by unacceptable character %c.\n", input[currentChar], input[next]);
                    texit(1);
                }
            }
        }
        
        else if ((input[currentChar] > 47 && input[currentChar] < 58) || input[currentChar] == '.'){
            int endPoint = numHelper(currentChar, input, size);
            int isFloat = 0;
            char str[endPoint - currentChar + 1];
            int i;
            for (i = currentChar; i <= endPoint; i++){
                if (input[i] == '.'){
                    isFloat = 1;
                }
                str[i - currentChar] = input[i];
            }
            str[i - currentChar] = '\0';
            Value *v = makeNull();
            if (isFloat){
                v -> type = DOUBLE_TYPE;
                v -> d = atof(str);
            }
            else{
                v -> type = INT_TYPE;
                v -> i = atoi(str);
            }
            list = cons(v, list);
            currentChar = endPoint;
        }
        
        else if(isBoolean(input, currentChar))
        {
            char *boolean = returnBoolean(input, currentChar);
            Value *v = makeNull();
            v -> type = BOOL_TYPE;
            v -> s = boolean;
            list = cons(v, list);
            currentChar += 2;
            continue;
        }
        else if(input[currentChar] == '(')
        {
            Value *v = makeNull();
            v -> type = OPEN_TYPE;
            char *ch = "(";
            v -> s = ch;
            list = cons(v, list);
        }
        
        else if(input[currentChar] == ')')
        {
            Value *v = makeNull();
            v -> type = CLOSE_TYPE;
            char *ch = ")";
            v -> s = ch;
            list = cons(v, list);
        }
                
        else if(input[currentChar] == ';'){
            while(input[currentChar] != '\n' && currentChar < size){
                currentChar++;
            }
        }
        else if (input[currentChar] == ' ' || input[currentChar] == '\n' || input[currentChar] == '\t'){
            currentChar++;
            continue;
        }
        else {
            int symLength = returnSymbolLength(input, currentChar, size);
            char *symbol = returnSymbol(input, currentChar, symLength); 
            Value *v = makeNull();
            v -> type = SYMBOL_TYPE;
            v -> s = symbol;
            list = cons(v, list);
            currentChar = currentChar + symLength;
        }
        currentChar++;
        int i = length(list);
    }

    Value *revList = reverse(list);
    return revList;
}
