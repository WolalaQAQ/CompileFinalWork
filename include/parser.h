//
// Created by wolala on 24-6-5.
//

#ifndef COMPILEFINALWORK_INCLUDE_PARSER_H
#define COMPILEFINALWORK_INCLUDE_PARSER_H

#include "grammar.h"
#include "scanner.h"
#include <iostream>
#include <stack>
#include <string>
#include <vector>

class Parser {
public:
    Parser() = default;

    bool parse(const ParseTable &parse_table, const AnswerTokens &tokens);

    inline std::vector<std::string> getHistory() const { return history_; }

private:
    AnswerTokens tokens_;
    int currentTokenIndex_ = 0;
    std::vector<std::string> history_;

    std::string getNextToken();
    void setTokens(const AnswerTokens &tokens);
};

#endif//COMPILEFINALWORK_INCLUDE_PARSER_H
