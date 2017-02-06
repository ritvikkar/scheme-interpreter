/*
 * value.h
 * CS 251, Carleton College, Fall 2016
 */

#ifndef _VALUE
#define _VALUE

typedef enum {
    PTR_TYPE, 
    INT_TYPE,
    DOUBLE_TYPE,
    STR_TYPE,
    CONS_TYPE,
    NULL_TYPE,
    OPEN_TYPE, 
    CLOSE_TYPE, 
    BOOL_TYPE,
    SYMBOL_TYPE,
    VOID_TYPE,
    CLOSURE_TYPE,
    PRIMITIVE_TYPE
} valueType;

typedef enum {
    VALUE,
    FRAME, 
    STR
} structType;

struct __Value {
   structType id;
   valueType type;
   union {
      void *p;
      int i;
      double d;
      char *s;
      struct ConsCell {
	 struct __Value *car;
	 struct __Value *cdr;
      } c;
       struct Closure {
           struct __Value *params;
           struct __Value *body;
           void *pointer; //Tried to make this a Frame pointer but got into problems with two structs referring to each other
       } l;
       struct __Value *(*pf)(struct __Value *);
   };
    int mark;
};

typedef struct __Value Value;

#endif
