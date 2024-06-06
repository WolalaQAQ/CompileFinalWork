#include "scanner.h"
#include <cctype>

Scanner::Scanner() {
	keywords = {
		{"program", TokenType::KEYWORD},
		{"var", TokenType::KEYWORD},
		{"int", TokenType::KEYWORD},
		{"float", TokenType::KEYWORD},
		{"char", TokenType::KEYWORD},
		{"double", TokenType::KEYWORD},
		{"void", TokenType::KEYWORD},
		{"struct", TokenType::KEYWORD},
		{"if", TokenType::KEYWORD},
		{"else", TokenType::KEYWORD},
		{"while", TokenType::KEYWORD},
		{"return", TokenType::KEYWORD},
		{"begin", TokenType::KEYWORD},
		{"end", TokenType::KEYWORD}
	};
}

bool Scanner::isDelimiter(char ch) {
	return isspace(ch) || ch == ';' || ch == ',' || ch == ':' || ch == '(' || ch == ')' || ch == '{' || ch == '}' || ch == '.';
}

bool Scanner::isOperator(char ch) {
	return ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '=' || ch == '!' || ch == '>' || ch == '<';
}

TokenType Scanner::identifyTokenType(const std::string& str) {
	if (keywords.find(str) != keywords.end()) {
		return keywords[str];
	} else if (isdigit(str[0])) {
		return TokenType::NUMBER;
	} else if (isalpha(str[0]) || str[0] == '_') {
		return TokenType::IDENTIFIER;
	}
	return TokenType::UNKNOWN;
}

std::vector<Token> Scanner::tokenize(const std::string &code) {
	std::vector<Token> tokens;
	std::string buffer;

	for (size_t i = 0; i < code.size(); ++i) {
		char ch = code[i];

		if (isDelimiter(ch)) {
			if (!buffer.empty()) {
				TokenType type = identifyTokenType(buffer);
				tokens.push_back({type, buffer});
				buffer.clear();
			}
			if (!isspace(ch)) {
				tokens.push_back({TokenType::DELIMITER, std::string(1, ch)});
			}
		} else if (isOperator(ch)) {
			if (!buffer.empty()) {
				TokenType type = identifyTokenType(buffer);
				tokens.push_back({type, buffer});
				buffer.clear();
			}
			std::string op(1, ch);
			if ((ch == '!' || ch == '=' || ch == '>' || ch == '<') && i + 1 < code.size() && code[i + 1] == '=') {
				op += code[++i];
			}
			tokens.push_back({TokenType::OPERATOR, op});
		} else {
			buffer += ch;
		}
	}

	if (!buffer.empty()) {
		TokenType type = identifyTokenType(buffer);
		tokens.push_back({type, buffer});
	}

	return tokens;
}