# CompilerBasicLanguage
A compiler for Basic Language.

## Dependencies
Make sure to have G++, Lex and Yacc installed before running the `make` command.

## Installation and running

Download or clone the repository. In the folder where it was downloaded / cloned, run:

`$ cd CompilerBasicLanguage/`

`$ make`

Then, the compiler is able to run. It uses the standard input and output, therefore, in order to compile the BASIC code in some file `input.basic` to a file named `output.cpp`, just run the following command:

`$ ./Compiler < input.basic > output.cpp`
