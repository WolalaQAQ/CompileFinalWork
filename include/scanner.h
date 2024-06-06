//
// Created by wolala on 24-6-5.
//

#ifndef COMPILEFINALWORK_INCLUDE_SCANNER_H
#define COMPILEFINALWORK_INCLUDE_SCANNER_H

#include <string>
#include <vector>
#include <unordered_map>

#include <string>
#include <vector>
#include <unordered_map>

enum class TokenType {
  KEYWORD,
  IDENTIFIER,
  NUMBER,
  OPERATOR,
  DELIMITER,
  END,
  UNKNOWN
};

struct Token {
  TokenType type;
  std::string value;
};

class Scanner {
private:
  std::unordered_map<std::string, TokenType> keywords;
  bool isDelimiter(char ch);
  bool isOperator(char ch);
  TokenType identifyTokenType(const std::string& str);

public:
  Scanner();
  std::vector<Token> tokenize(const std::string &code);
};


#endif //COMPILEFINALWORK_INCLUDE_SCANNER_H
