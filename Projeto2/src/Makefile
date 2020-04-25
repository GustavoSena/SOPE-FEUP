
CC = gcc
CFLAGS = -Wall
DEPS = Qn.h Un.h args.h reg.h utils.h
OBJ = Qn.o Un.o args.o reg.c utils.c
TARGETS = Qn Un

all: Qn Un

%.o: %.c $(DEPS)
	@$(CC) $(CFLAGS) -c -o $@ $<
	@echo $@

Qn: $(OBJ)
	@$(CC) $(CFLAGS) -o $@ $@.c $(OBJ) -lm
	@echo $@
	
Un: $(OBJ)
	@$(CC) $(CFLAGS) -o $@ $@.c $(OBJ) -lm
	@echo $@

clean:
	@rm *.o $(TARGETS)