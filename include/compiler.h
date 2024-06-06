//
// Created by wolala on 24-6-6.
//

#ifndef COMPILEFINALWORK_INCLUDE_COMPILER_H
#define COMPILEFINALWORK_INCLUDE_COMPILER_H

#include "symbol_table.h"
#include "scanner.h"
#include "parser.h"
#include <string>

class Compiler {
public:
  Compiler();
  void processFile(const std::string& filename);
  void printSymbolTable();

private:
  Scanner scanner;
  SymbolTable symbolTable;
  Parser parser;
};

#endif //COMPILEFINALWORK_INCLUDE_COMPILER_H
