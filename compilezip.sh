#!/bin/sh
lex uccompiler.l
yacc -d -v uccompiler.y
clang-3.8 -Wall -o uccompiler -Wno-unused-function *.c
zip uccompiler.zip uccompiler.l uccompiler.y tree.c tree.h semantics.c semantics.h structs.h sym_table.c sym_table.h 