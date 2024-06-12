#include "parser.h"
#include <iostream>

std::string Parser::getNextToken() {
	if (currentTokenIndex_ < tokens_.size()) {
		return tokens_[currentTokenIndex_++];
	}
	return "";
}

void Parser::setParser(const AnswerTokens& tokens) {
	this->tokens_ = tokens;
	std::cout << "tokens:\n";
	for (const auto &token : tokens_) {
		std::cout << token << ' ';
	}
	std::cout << '\n';
	tokens_.emplace_back("$");
	currentTokenIndex_ = 0;
}

bool Parser::parse(const Grammar &grammar, const AnswerTokens &tokens) {
	setParser(tokens);

	std::stack<std::string> parseStack;
	parseStack.emplace("$");
	parseStack.emplace("A_Program");

	std::string currentToken = getNextToken();

	while (!parseStack.empty()) {
		std::string top = parseStack.top();
		parseStack.pop();
		//cout << "parseStack_Top: " << top << '\n';

		if (top==currentToken) {
			currentToken = getNextToken();
			//cout << "successful match: " << top << " next_token: " << currentToken << '\n';
		} else if (grammar.parseTable.find(top)!=grammar.parseTable.end()) {
			std::cout << "Number" << currentTokenIndex_ << ":" << top << " " << currentToken << "rest "
					  << currentTokenIndex_
					  << '\n';
			if (grammar.parseTable.at(top).find(currentToken)!=grammar.parseTable.at(top).end()) {
				std::string rule = grammar.parseTable.at(top).at(currentToken);
				if (rule!="ε") {
					std::vector<std::string> symbols;
					std::stringstream ss(rule);
					std::string symbol;

					while (ss >> symbol) {
						symbols.push_back(symbol);

					}
					for (int i = symbols.size() - 1; i >= 0; --i) {
						parseStack.push(symbols[i]);
						std::cout << "Number" << currentTokenIndex_ << ":" << currentToken << " Rule: " << top << "->"
								  << rule
								  << " Push {" << symbols[i] << "} rest:" << parseStack.size() << "with i = " << i
								  << "\n";
					}
				}
			} else {
				std::cout << "Number" << currentTokenIndex_ << ":" << top << " " << currentToken << "rest "
						  << currentTokenIndex_
						  << '\n';
				return false;
			}
		} else {
			std::cout << top << ' ' << currentToken << ' ' << currentTokenIndex_ << ' ';
			return false;
		}
	}

	return true;
}