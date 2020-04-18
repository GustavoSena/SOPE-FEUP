CC = gcc
CFLAGS = -Wall
DEPS = args.h utils.h
OBJ = args.o utils.c
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