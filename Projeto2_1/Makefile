all: program1 program2
CC = gcc
CFLAGS = -pthread -Wall
#args.h args.c Qn.c Un.h reg.c reg.h utils.c utils.h
SRCS = args.c reg.c utils.c
OBJS = $(SRCS:.c=.o)
%.o:%.c
	$(CC) -c $(CFLAGS) $< -o $@
program1: $(OBJS)
	$(CC) Qn.c -o Q1 $(OBJS) $(CFLAGS)
program2: $(OBJS)
	$(CC) Un.c -o U1 $(OBJS) $(CFLAGS)
clean :
	-rm $(OBJS) $(EXEC)
