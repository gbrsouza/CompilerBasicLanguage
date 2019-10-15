# options
DEBUG = -g
WARNING = -Wall -Wextra -pedantic -Wshadow -Wformat=2 -Wfloat-equal -Wconversion -Wlogical-op -Wno-unused-result -Wno-unused-function
OPTMIZE = -O2
STD = -std=c++14
OPTIONSCXX = $(STD) $(WARNING)

# directories
DECLRDIR = ./include/
INCLUDES = -I $(DECLRDIR)
SRCDIR = ./src/
BINDIR = ./bin/

CXX = g++
DEL = rm
EXEC = Compiler

### executable target
main: lex yacc bin token.o visitor.o parser.o tree_nodes.o main.o
	$(CXX) $(BINDIR)token.o $(BINDIR)visitor.o $(BINDIR)parser.o $(BINDIR)tree_nodes.o $(BINDIR)main.o -o $(EXEC) $(OPTIONSCXX)

lex:
	lex -o $(SRCDIR)token.cpp $(SRCDIR)token.l

yacc:
	yacc -o $(SRCDIR)parser.cpp $(SRCDIR)grammar.y

### bin folder (created only if it doesnt exist)
bin:
	mkdir -p ./bin/

%.o: $(SRCDIR)%.cpp
	$(CXX) -c $< -o $(BINDIR)$@ $(OPTIONSCXX) $(INCLUDES)

main.o: $(SRCDIR)main.cpp
token.o: $(SRCDIR)token.cpp
parser.o: $(SRCDIR)parser.cpp
tree_nodes.o: $(SRCDIR)tree_nodes.cpp
visitor.o: $(SRCDIR)visitor.cpp

### clear objects & executable
clean: bin
	$(DEL) ./$(EXEC)
	$(DEL) $(BINDIR)*.o
	$(DEL) $(SRCDIR)token.cpp
	$(DEL) $(SRCDIR)parser.cpp
