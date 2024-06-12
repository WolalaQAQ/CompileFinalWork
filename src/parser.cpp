#include "parser.h"
#include <iostream>

std::string Parser::getNextToken() {
    if (currentTokenIndex_ < tokens_.size()) {
        return tokens_[currentTokenIndex_++];
    }
    return "";
}

void Parser::setTokens(const AnswerTokens &tokens) {
    this->tokens_ = tokens;
//    std::cout << "tokens:\n";
//    for (const auto &token: tokens_) {
//        std::cout << token << ' ';
//    }
//    std::cout << '\n';
    tokens_.emplace_back("$");
    currentTokenIndex_ = 0;
}

bool Parser::parse(const ParseTable &parse_table, const AnswerTokens &tokens) {
    setTokens(tokens);

    std::stack<std::string> parseStack;
    parseStack.emplace("$");
    parseStack.emplace("A_Program");

    std::string currentToken = getNextToken();

    while (!parseStack.empty()) {
        std::string top = parseStack.top();
        parseStack.pop();
        //cout << "parseStack_Top: " << top << '\n';

        if (top == currentToken) {
            currentToken = getNextToken();
            //cout << "successful match: " << top << " next_token: " << currentToken << '\n';
        } else if (parse_table.find(top) != parse_table.end()) {
            std::stringstream ss_history;
            ss_history << "Number " << currentTokenIndex_ << ": " << top << " " << currentToken << " Rest: "
                      << currentTokenIndex_
                      << '\n';
            history_.emplace_back(ss_history.str());
            if (parse_table.at(top).find(currentToken) != parse_table.at(top).end()) {
                std::string rule = parse_table.at(top).at(currentToken);
                if (rule != "ε") {
                    std::vector<std::string> symbols;
                    std::stringstream ss(rule);
                    std::string symbol;

                    while (ss >> symbol) {
                        symbols.push_back(symbol);
                    }
                    for (int i = symbols.size() - 1; i >= 0; --i) {
                        parseStack.push(symbols[i]);
                        std::stringstream ss_history_2;
                        ss_history_2 << "Number " << currentTokenIndex_ << ": " << currentToken << " Rule: " << top << " -> "
                                  << rule
                                  << " Push {" << symbols[i] << "} Rest: " << parseStack.size() << " with i = " << i
                                  << "\n";
                        history_.emplace_back(ss_history_2.str());
                    }
                }
            } else {
                std::stringstream ss_history_3;
                ss_history_3 << "Number " << currentTokenIndex_ << ": " << top << " " << currentToken << " Rest "
                          << currentTokenIndex_
                          << '\n';
                history_.emplace_back(ss_history_3.str());
                return false;
            }
        } else {
            std::stringstream ss_history_4;
            ss_history_4 << top << ' ' << currentToken << ' ' << currentTokenIndex_ << ' ';
            history_.emplace_back(ss_history_4.str());
            return false;
        }
    }

    return true;
}