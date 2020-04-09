CC = gcc
CFLAGS = -Wall
OBJS = simpledu.o stat.o reg.o result.o args.o signal_handlers.o
XHDRS = simpledu.h stat.h reg.h result.h args.h signal_handlers.h
EXEC = simpledu

$(EXEC): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $@

%.o: %.c %.h $(XHDRS)
	@$(CC) $(CFLAGS) -c $<

.PHONY : clean
clean :
	@-rm $(OBJS) $(EXEC)