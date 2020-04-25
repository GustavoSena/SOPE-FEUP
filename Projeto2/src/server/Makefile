CC = gcc
CFLAGS = -pthread
OBJS = ../utils.o ../reg.o ../args.o
XHDRS = ../utils.h ../reg.h ../args.h
EXEC = Qn

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

%.o: %.c %.h $(XHDRS)
	$(CC) $(CFLAGS) -c $<

.PHONY : clean
clean :
	-rm $(OBJS) $(EXEC)