//
// Created by wolala on 24-6-5.
//

#ifndef COMPILEFINALWORK_INCLUDE_PARSER_H
#define COMPILEFINALWORK_INCLUDE_PARSER_H

#include "scanner.h"
#include "grammar.h"
#include <vector>
#include <string>
#include <stack>
#include <iostream>

class Parser {
public:
  Parser() = default;

  bool parse(const Grammar &grammar, const AnswerTokens &tokens);

private:
  AnswerTokens tokens_;
  int currentTokenIndex_ = 0;

  std::string getNextToken();

  void setParser(const AnswerTokens& tokens);

};

#endif //COMPILEFINALWORK_INCLUDE_PARSER_H
