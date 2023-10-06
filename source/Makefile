CC=gcc -std=c99
CFLAGS=-I.
DEPS = pointlist.h edgelist.h
OBJ = cts.o pointlist.o edgelist.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

cts: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean
clean: 
	-rm $(OBJ)