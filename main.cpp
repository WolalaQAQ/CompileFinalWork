#include "compiler.h"
#include <iostream>

int main() {
	Compiler compiler;
	compiler.processFile("/home/wolala/Documents/CompilePractice/CompileFinalWork/example/example.c");
	compiler.printSymbolTable();
	return 0;
}
