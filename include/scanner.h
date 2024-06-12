//
// Created by wolala on 24-6-5.
//

#ifndef COMPILEFINALWORK_INCLUDE_SCANNER_H
#define COMPILEFINALWORK_INCLUDE_SCANNER_H

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <utility>

struct TokenInfo {
  std::string type;
  int id{};
};

using AnswerTokens = std::vector<std::string>;
using Token = std::pair<std::string, TokenInfo>;

class Scanner {
public:
  Scanner() = default;
  AnswerTokens get_AnswerTokens(const std::string &codes);

  inline std::vector<Token> getTokens() const {
	return tokens_;
  }

  inline std::vector<std::string> getKeywordList() const {
	return keywordList;
  }

  inline std::vector<std::string> getDelimiterList() const {
	return delimiterList;
  }

  inline std::map<std::string, TokenInfo> getSymbolTable() const {
	return symbolTable;
  }

private:
  int isError = 0;

  std::vector<std::string> keywordList =
	  {"int", "void", "char", "break", "float", "while", "do", "struct", "const", "case", "for", "return", "if",
	   "default", "else", "goto", "functioncall"};

  std::vector<std::string> delimiterList =
	  {"-", "/", "(", ")", "==", "!=", "<=", "<", "+", "*", ">", ">=", "=", ",", ";", "++", "--", "{", "}", "?",
	   "[",
	   "]", "+=", "-="};

  std::map<std::string, TokenInfo> symbolTable;
  int identifierCount = 0;
  int constantIntCount = 0;
  int constantRealCount = 0;
  int constantCharCount = 0;
  int constantStringCount = 0;
  std::map<std::string, TokenInfo> tokenMap;

  std::vector<Token> tokens_;

  static inline bool isLetter(char ch) {
	  return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
  }

  static inline bool isDigit(char ch) {
	  return ch >= '0' && ch <= '9';
  }

  static inline bool isHexDigit(char ch) {
	  return isDigit(ch) || (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F');
  }

  void initTokenMap();

  std::vector<Token> lexAnalyse(const std::string &code);

};

#endif //COMPILEFINALWORK_INCLUDE_SCANNER_H
