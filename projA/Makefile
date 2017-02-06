CC = clang
CFLAGS = -g

SRCS = interpreter.c linkedlist.c main.c parser.c talloc.c tokenizer.c
HDRS = interpreter.h linkedlist.h parser.h talloc.h tokenizer.h
OBJS = $(SRCS:.c=.o)
    
interpreter: $(OBJS)
	$(CC)  $(CFLAGS) $^  -o $@

memtest:
	valgrind --leak-check=full --show-leak-kinds=all ./interpreter

%.o : %.c $(HDRS)
	$(CC)  $(CFLAGS) -c $<  -o $@

clean:
	rm *.o
	rm talloctest