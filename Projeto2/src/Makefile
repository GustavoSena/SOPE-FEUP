CC = gcc
CFLAGS = -Wall
DEPS = Qn.o Un.o args.o reg.h
OBJ = Qn.o Un.o args.o reg.c
TARGETS = Qn Un

all: p07_client p07_server

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