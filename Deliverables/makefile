
BIN = c-
CC = g++
CFLAGS = -std=c++17 -Wall

SRCS = $(BIN).y $(BIN).l tree.c
HDRS = scanType.h tree.h globals.h
OBJS = lex.yy.o $(BIN).tab.o tree.o


# Default target
$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(BIN)

# Flex generates lex.yy.c (depends on Bison header)
lex.yy.c: $(BIN).l $(BIN).tab.h $(HDRS)
	flex $(BIN).l

# Bison generates parser source and header
$(BIN).tab.c $(BIN).tab.h: $(BIN).y $(HDRS)
	bison -v -t -d $(BIN).y


# Compile Flex output
lex.yy.o: lex.yy.c $(HDRS)
	$(CC) $(CFLAGS) -c lex.yy.c

# Compile Bison output
$(BIN).tab.o: $(BIN).tab.c $(HDRS)
	$(CC) $(CFLAGS) -c $(BIN).tab.c

# Compile tree.c
tree.o: tree.c tree.h
	$(CC) $(CFLAGS) -c tree.c


# Clean generated files
clean:
	rm -f *~ $(OBJS) $(BIN) lex.yy.c $(BIN).tab.h $(BIN).tab.c $(BIN).output

# Tarball
tar: $(HDRS) $(SRCS) makefile
	tar -cvf $(BIN).tar $(HDRS) $(SRCS) makefile