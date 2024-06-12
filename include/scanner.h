//
// Created by wolala on 24-6-5.
//

#ifndef COMPILEFINALWORK_INCLUDE_SCANNER_H
#define COMPILEFINALWORK_INCLUDE_SCANNER_H

#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

struct TokenInfo {
    std::string type;
    int id{};
};

using AnswerTokens = std::vector<std::string>;
using Token = std::pair<std::string, TokenInfo>;
using Tokens = std::vector<Token>;

class Scanner {
public:
    Scanner() = default;
    void scan(const std::string &code);

    [[nodiscard]] inline Tokens getTokens() const { return tokens_; }
    [[nodiscard]] inline std::vector<std::string> getKeywordList() const { return keywordList; }
    [[nodiscard]] inline std::vector<std::string> getDelimiterList() const { return delimiterList; }
    [[nodiscard]] inline std::map<std::string, TokenInfo> getSymbolTable() const { return symbolTable; }
    [[nodiscard]] inline AnswerTokens getAnswerTokens() const { return answer_tokens_; }

private:
    int isError = 0;

    std::vector<std::string> keywordList =
            {"int", "void", "char", "break", "float", "while", "do", "struct", "const", "case", "for", "return", "if",
             "default", "else", "goto", "functioncall"};

    std::vector<std::string> delimiterList =
            {"-", "/", "(", ")", "==", "!=", "<=", "<", "+", "*", ">", ">=", "=", ",", ";", "++", "--", "{", "}", "?",
             "[", "]", "+=", "-="};

    std::map<std::string, TokenInfo> symbolTable;

    int identifierCount = 0;
    int constantIntCount = 0;
    int constantRealCount = 0;
    int constantCharCount = 0;
    int constantStringCount = 0;

    std::map<std::string, TokenInfo> tokenMap;

    Tokens tokens_;
    AnswerTokens answer_tokens_;

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

#endif//COMPILEFINALWORK_INCLUDE_SCANNER_H
