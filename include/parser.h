//
// Created by wolala on 24-6-5.
//

#ifndef COMPILEFINALWORK_INCLUDE_PARSER_H
#define COMPILEFINALWORK_INCLUDE_PARSER_H

#include "scanner.h"
#include "symbol_table.h"
#include <vector>

class Parser {
public:
  Parser(Scanner& scanner, SymbolTable& symbolTable);
  void parse(const std::string& code);

private:
  Scanner& scanner;
  SymbolTable& symbolTable;
  std::vector<Token> tokens;
  size_t currentTokenIndex;

  void parseDeclaration();
  void parseStructDeclaration();
  void parseFunctionDeclaration();
  void parseStatement();
  void parseExpression();
  Token currentToken();
  Token nextToken();
  bool matchToken(TokenType type, const std::string& value = "");
};


#endif //COMPILEFINALWORK_INCLUDE_PARSER_H
