#include "compiler.h"
#include <fstream>
#include <sstream>
#include <iostream>

Compiler::Compiler() : parser(scanner, symbolTable) {}

void Compiler::processFile(const std::string& filename) {
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cerr << "Error: Could not open file " << filename << std::endl;
		return;
	}

	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string code = buffer.str();

	parser.parse(code);
}

void Compiler::printSymbolTable() {
	symbolTable.printTable();
}