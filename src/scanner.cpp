#include "scanner.h"
#include <fstream>

void Scanner::initTokenMap() {
	for (int i = 0; i < keywordList.size(); ++i) {
		tokenMap[keywordList[i]] = {"K", i + 1};
	}
	for (int i = 0; i < delimiterList.size(); ++i) {
		tokenMap[delimiterList[i]] = {"P", i + 1};
	}
}

std::vector<Token> Scanner::lexAnalyse(const std::string &code) {
	std::vector<std::pair<std::string, TokenInfo>> tokens;
	std::string currentToken;
	int state = 0;

	for (size_t idx = 0; idx <= code.length(); ++idx) {
		char ch = code[idx];
		char nextCh = (idx + 1 < code.length()) ? code[idx + 1] : '\0';

		switch (state) {
		case 0:
			if (!(ch==' ' || ch=='\t' || ch=='\n')) {
				if (isLetter(ch)) {
					currentToken = ch;
					state = 1;
				} else if (isDigit(ch)) {
					currentToken = ch;
					state = 2;
				} else if (ch=='\'') {
					currentToken = ch;
					state = 3;
				} else if (ch=='\"') {
					currentToken = ch;
					state = 4;
				} else {
					std::string potentialDelim = std::string(1, ch);
					if (nextCh && tokenMap.count(potentialDelim + nextCh)) {
						tokens.emplace_back(potentialDelim + nextCh, tokenMap[potentialDelim + nextCh]);
						++idx;
					} else if (tokenMap.count(potentialDelim)) {
						tokens.emplace_back(potentialDelim, tokenMap[potentialDelim]);
					}
				}
			}

			break;

		case 1:
			if (isLetter(ch) || isDigit(ch)) {
				currentToken += ch;
			} else {
				if (!tokenMap.count(currentToken)) {
					if (!symbolTable.count(currentToken)) {
						++identifierCount;
						symbolTable[currentToken] = {"I", identifierCount};
					}
					tokens.emplace_back(currentToken, symbolTable[currentToken]);
				} else
					tokens.emplace_back(currentToken, tokenMap[currentToken]);

				currentToken.clear();
				state = 0;
				--idx;
			}
			break;

		case 2:
			if (isDigit(ch)) {
				currentToken += ch;
			} else if (ch=='x' || ch=='X') {
				currentToken += ch;
				state = 8;
			} else if (ch=='e' || ch=='E') {
				currentToken += ch;
				state = 6;
			} else if (ch=='.' && isDigit(nextCh)) {
				currentToken += ch;
				state = 5;
			} else {
				if (!symbolTable.count(currentToken)) {
					++constantIntCount;
					symbolTable[currentToken] = {"C1", constantIntCount};

				}

				tokens.emplace_back(currentToken, symbolTable[currentToken]);
				currentToken.clear();
				state = 0;
				--idx;
			}
			break;

		case 3:
			if (ch=='\\' && nextCh!='\0') {
				currentToken += ch;
				currentToken += nextCh;
				++idx;
			} else {
				currentToken += ch;
				if (nextCh=='\'') {

					if (currentToken.length()==2) {
						currentToken += nextCh;
						++constantCharCount;
						symbolTable[currentToken] = {"CT", constantCharCount};
						tokens.emplace_back(currentToken, symbolTable[currentToken]);
						currentToken.clear();
						state = 0;
						++idx;
					} else {
						std::cout << "ERROR" << std::endl;
						isError = 1;
						return tokens;
					}

				}
			}
			break;

		case 4:
			if (ch=='\\' && nextCh!='\0') {
				currentToken += ch;
				currentToken += nextCh;
				++idx;
			} else {
				currentToken += ch;
				if (nextCh=='\"') {
					currentToken += nextCh;
					++constantStringCount;
					symbolTable[currentToken] = {"ST", constantStringCount};
					tokens.emplace_back(currentToken, symbolTable[currentToken]);
					currentToken.clear();
					state = 0;
					++idx;
				}
			}
			break;

		case 5:
			if (isDigit(ch)) {
				currentToken += ch;
			} else if (ch=='e' || ch=='E') {
				currentToken += ch;
				state = 6;
			} else {
				++constantRealCount;
				symbolTable[currentToken] = {"C2", constantRealCount};
				tokens.emplace_back(currentToken, symbolTable[currentToken]);
				currentToken.clear();
				state = 0;
				--idx;
			}
			break;

		case 6:
			if (ch=='-') {
				currentToken += ch;
			} else if (isDigit(ch)) {
				currentToken += ch;
				state = 7;
			} else {
				std::cout << "ERROR" << std::endl;
				isError = 1;
				return tokens;
			}
			break;

		case 7:
			if (isDigit(ch)) {
				currentToken += ch;
			} else {
				++constantRealCount;
				symbolTable[currentToken] = {"C2", constantRealCount};
				tokens.emplace_back(currentToken, symbolTable[currentToken]);
				currentToken.clear();
				state = 0;
				--idx;
			}
			break;

		case 8:
			if (isHexDigit(ch)) {
				currentToken += ch;
			} else if (!symbolTable.count(currentToken)) {
				int num = std::stoi(currentToken, nullptr, 16);

				if (!symbolTable.count(std::to_string(num))) {
					++constantIntCount;
					symbolTable[std::to_string(num)] = {"C1", constantIntCount};

				}

				tokens.emplace_back(std::to_string(num), symbolTable[std::to_string(num)]);
				currentToken.clear();
				state = 0;
				--idx;
			} else {
				std::cout << "ERROR" << std::endl;
				isError = 1;
				return tokens;
			}
			break;

		}
	}

	return tokens;
}

AnswerTokens Scanner::get_AnswerTokens(const std::string &codes) {
	initTokenMap();
//	std::string codes;
//	std::cout << "enter your programe(end at the last line with \"END\")\n";
//
//	while (true) {
//		std::string code;
//		getline(std::cin, code);
//		if (code=="END") {
//			break;
//		}
//		codes.append(code);
//	}

//	std::cout << "input:" << codes << '\n';

	tokens_ = lexAnalyse(codes);
	AnswerTokens answer_tokens;

	if (!isError) {
		std::vector<std::pair<std::string, TokenInfo>> sortedSymbols(symbolTable.begin(), symbolTable.end());

		sort(sortedSymbols.begin(),
			 sortedSymbols.end(),
			 [](const std::pair<std::string, TokenInfo> &a, const std::pair<std::string, TokenInfo> &b) {
			   return a.second.id < b.second.id;
			 });

		for (auto &token : tokens_) {
			if (token.second.type=="P" || token.second.type=="K") {
				answer_tokens.push_back(token.first);
			} else if (token.second.type=="I") {
				answer_tokens.emplace_back("id");
			} else {
				answer_tokens.emplace_back("number");
			}
		}
	}

	return answer_tokens;
}
