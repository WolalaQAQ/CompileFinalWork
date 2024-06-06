#include "parser.h"
#include <iostream>

Parser::Parser(Scanner& scanner, SymbolTable& symbolTable)
	: scanner(scanner), symbolTable(symbolTable), currentTokenIndex(0) {}

void Parser::parse(const std::string& code) {
	tokens = scanner.tokenize(code);
	currentTokenIndex = 0;

	while (currentTokenIndex < tokens.size()) {
		parseDeclaration();
	}
}

void Parser::parseDeclaration() {
	Token token = currentToken();

	if (token.type == TokenType::KEYWORD) {
		if (token.value == "struct") {
			parseStructDeclaration();
		} else if (token.value == "int" || token.value == "float" || token.value == "char" || token.value == "double" || token.value == "void") {
			Token nextToken = tokens[currentTokenIndex + 1];
			if (nextToken.type == TokenType::IDENTIFIER && tokens[currentTokenIndex + 2].value == "(") {
				parseFunctionDeclaration();
			} else {
				SymbolType type;
				if (token.value == "int") {
					type = SymbolType::INTEGER;
				} else if (token.value == "float") {
					type = SymbolType::FLOAT;
				} else if (token.value == "char") {
					type = SymbolType::CHAR;
				} else if (token.value == "double") {
					type = SymbolType::DOUBLE;
				} else {
					type = SymbolType::UNKNOWN;
				}

				nextToken(); // Consume type
				Token identifierToken = nextToken; // Consume identifier

				symbolTable.addSymbol(identifierToken.value, type);

				matchToken(TokenType::DELIMITER, ";"); // Consume semicolon
			}
		}
	} else {
		parseStatement();
	}
}

void Parser::parseStructDeclaration() {
	matchToken(TokenType::KEYWORD, "struct");

	Token structNameToken = nextToken();
	std::string structName = structNameToken.value;
	std::unordered_map<std::string, Symbol> members;

	matchToken(TokenType::DELIMITER, "{");

	while (!matchToken(TokenType::DELIMITER, "}")) {
		Token typeToken = nextToken();
		SymbolType type;

		if (typeToken.value == "int") {
			type = SymbolType::INTEGER;
		} else if (typeToken.value == "float") {
			type = SymbolType::FLOAT;
		} else if (typeToken.value == "char") {
			type = SymbolType::CHAR;
		} else if (typeToken.value == "double") {
			type = SymbolType::DOUBLE;
		} else {
			type = SymbolType::UNKNOWN;
		}

		Token memberToken = nextToken();
		members[memberToken.value] = {type, 0, symbolTable.getSize(type)};

		matchToken(TokenType::DELIMITER, ";");
	}

	symbolTable.addStructSymbol(structName, members);
}

void Parser::parseFunctionDeclaration() {
	Token returnTypeToken = nextToken();
	Token functionNameToken = nextToken();
	nextToken(); // Consume '('

	while (!matchToken(TokenType::DELIMITER, ")")) {
		nextToken(); // Consume parameter list
	}

	symbolTable.addFunctionSymbol(functionNameToken.value);

	matchToken(TokenType::DELIMITER, "{");

	while (!matchToken(TokenType::DELIMITER, "}")) {
		parseStatement();
	}
}

void Parser::parseStatement() {
	Token token = currentToken();

	if (token.type == TokenType::KEYWORD) {
		if (token.value == "if" || token.value == "while") {
			nextToken(); // Consume 'if' or 'while'
			matchToken(TokenType::DELIMITER, "(");
			parseExpression();
			matchToken(TokenType::DELIMITER, ")");
			parseStatement();
		}
	} else if (token.type == TokenType::IDENTIFIER) {
		parseExpression();
		matchToken(TokenType::DELIMITER, ";");
	} else {
		nextToken(); // Consume unknown statement
	}
}

void Parser::parseExpression() {
	nextToken(); // For simplicity, assume single token expressions
}

Token Parser::currentToken() {
	if (currentTokenIndex < tokens.size()) {
		return tokens[currentTokenIndex];
	}
	return {TokenType::END, ""};
}

Token Parser::nextToken() {
	if (currentTokenIndex < tokens.size()) {
		return tokens[currentTokenIndex++];
	}
	return {TokenType::END, ""};
}

bool Parser::matchToken(TokenType type, const std::string& value) {
	if (currentTokenIndex < tokens.size() && tokens[currentTokenIndex].type == type && (value.empty() || tokens[currentTokenIndex].value == value)) {
		++currentTokenIndex;
		return true;
	}
	return false;
}
