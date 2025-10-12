
BIN = c-
CXX = g++
CXXFLAGS = -std=c++17 -Wall -g

SRCS = main.cpp $(BIN).y $(BIN).l tree.cpp symbolTable.cpp semantic.cpp
HDRS = scanType.h tree.h globals.h symbolTable.h semantic.h
OBJS = main.o lex.yy.o c-.tab.o tree.o symbolTable.o semantic.o

$(BIN): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(BIN)


# Bison generates the parser (c-.tab.cpp) and its header (c-.tab.hpp)
c-.tab.cpp c-.tab.hpp: c-.y
	bison -d -v -t -o c-.tab.cpp c-.y

# Flex generates the scanner
lex.yy.cpp: c-.l c-.tab.hpp
	flex -o lex.yy.cpp c-.l

# Compile the scanner's object file, which depends on the parser's header
lex.yy.o: lex.yy.cpp c-.tab.hpp
	$(CXX) $(CXXFLAGS) -c lex.yy.cpp

# Compile the parser's object file
c-.tab.o: c-.tab.cpp c-.tab.hpp
	$(CXX) $(CXXFLAGS) -c c-.tab.cpp

tree.o: tree.cpp tree.h
	$(CXX) $(CXXFLAGS) -c tree.cpp

main.o: main.cpp globals.h tree.h c-.tab.hpp semantic.h
	$(CXX) $(CXXFLAGS) -c main.cpp

symbolTable.o: symbolTable.cpp symbolTable.h
	$(CXX) $(CXXFLAGS) -c symbolTable.cpp

semantic.o: semantic.cpp semantic.h tree.h symbolTable.h
	$(CXX) $(CXXFLAGS) -c semantic.cpp


# Clean generated files
clean:
	rm -f *~ $(OBJS) $(BIN) lex.yy.cpp c-.tab.hpp c-.tab.cpp $(BIN).output c-.tar

# Tarball
tar: $(HDRS) $(SRCS) makefile
	tar -cvf $(BIN).tar $(HDRS) $(SRCS) makefile
