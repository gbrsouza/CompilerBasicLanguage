# options
DEBUG = -g
WARNING = -Wall -Wextra -pedantic -Wshadow -Wformat=2 -Wfloat-equal -Wconversion -Wlogical-op -Wno-unused-result -Wno-unused-function
OPTMIZE = -O2
STD = -std=c++14
OPTIONSCXX = $(STD) $(WARNING)

# directories
DECLRDIR = -I ./include
SRCDIR = ./src/
BINDIR = ./bin/

CXX = g++
DEL = rm
EXEC = Compiler

### executable target
main: lex bin token.o parser.o main.o
	$(CXX) $(BINDIR)token.o $(BINDIR)parser.o $(BINDIR)main.o -o $(EXEC) $(OPTIONSCXX)

lex:
	lex -o $(SRCDIR)token.cpp $(SRCDIR)token.l

### bin folder (created only if it doesnt exist)
bin:
	mkdir -p ./bin/

%.o: $(SRCDIR)%.cpp
	$(CXX) -c $< -o $(BINDIR)$@ $(OPTIONSCXX) $(DECLRDIR)

main.o: $(SRCDIR)main.cpp
token.o: $(SRCDIR)token.cpp
parser.o: $(SRCDIR)parser.cpp

### clear objects & executable
clean: bin
	$(DEL) ./$(EXEC)
	$(DEL) $(BINDIR)*.o

