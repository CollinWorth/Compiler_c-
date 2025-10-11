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
	rm -f *~ $(OBJS) $(BIN) lex.yy.cpp c-.tab.hpp c-.tab.cpp $(BIN).output

# Define the files to include in the tar
TAR_FILES = *.h *.cpp *.y *.l makefile

# Method 1: Use gnutar if available (install with: brew install gnu-tar)
#tar:
#	gnutar -cf c-.tar $(TAR_FILES)

# Method 2: Use --no-mac-metadata flag (if your tar supports it)
#tar:
#	tar --no-mac-metadata -cf c-.tar $(TAR_FILES)

# Method 3: Create tar in a clean temporary directory
tar:
	mkdir -p /tmp/clean_tar
	cp $(TAR_FILES) /tmp/clean_tar/
	cd /tmp/clean_tar && tar -cf c-.tar *
	mv /tmp/clean_tar/c-.tar ./
	rm -rf /tmp/clean_tar

# Method 4: Use pax instead of tar
#tar:
#	pax -w -f c-.tar $(TAR_FILES)

# Method 5: Explicitly exclude extended attributes
#tar:
#	COPYFILE_DISABLE=1 COPYFILE_EXTENDED_ATTRIBUTES_DISABLE=1 tar --no-xattrs -cf c-.tar $(TAR_FILES) 2>/dev/null || COPYFILE_DISABLE=1 tar -cf c-.tar $(TAR_FILES)

# Method 6: Use rsync to copy to temp dir then tar
#tar:
#	mkdir -p /tmp/clean_submission
#	rsync -av --exclude='.*' $(TAR_FILES) /tmp/clean_submission/
#	cd /tmp/clean_submission && tar -cf $(CURDIR)/c-.tar *
#	rm -rf /tmp/clean_submission

# Method 7: Stripping extended attributes

.PHONY: tar tar-fixed tar-clean
