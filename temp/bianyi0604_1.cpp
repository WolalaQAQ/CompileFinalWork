#include <iostream>
#include <stack>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <set>
#include <map>
#include <algorithm>

using namespace std;
//词法分析

int isError = 0;
struct TokenInfo {
    string type;
    int id;
};

map<string, TokenInfo> symbolTable;
int identifierCount = 0;
int constantIntCount = 0;
int constantRealCount = 0;
int constantCharCount = 0;
int constantStringCount = 0;
map<string, TokenInfo> tokenMap;

void initTokenMap() {

    vector<string> keywordList = { "int", "void", "break", "float", "while", "do", "struct", "const", "case", "for", "return", "if", "default","else", };
    for (int i = 0; i < keywordList.size(); ++i) {
        tokenMap[keywordList[i]] = { "K", i + 1 };
    }


    vector<string> delimiterList = { "-", "/", "(", ")","==", "<=", "<", "+", "*", ">", "=", ",", ";", "++", "{", "}" };
    for (int i = 0; i < delimiterList.size(); ++i) {
        tokenMap[delimiterList[i]] = { "P", i + 1 };
    }
}

bool isLetter(char ch) {
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

bool isDigit(char ch) {
    return ch >= '0' && ch <= '9';
}

bool isHexDigit(char ch) {
    return isDigit(ch) || (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F');
}

vector<pair<string, TokenInfo>> lexAnalyse(const string& code) {
    vector<pair<string, TokenInfo>> tokens;
    string currentToken;
    int state = 0;

    for (size_t idx = 0; idx <= code.length(); ++idx) {
        char ch = code[idx];
        char nextCh = (idx + 1 < code.length()) ? code[idx + 1] : '\0';

        switch (state) {
        case 0:
            if (!(ch == ' ' || ch == '\t' || ch == '\n'))
            {
                if (isLetter(ch)) {
                    currentToken = ch;
                    state = 1;
                }
                else if (isDigit(ch)) {
                    currentToken = ch;
                    state = 2;
                }
                else if (ch == '\'') {
                    currentToken = ch;
                    state = 3;
                }
                else if (ch == '\"') {
                    currentToken = ch;
                    state = 4;
                }
                else
                {
                    string potentialDelim = string(1, ch);
                    if (nextCh && tokenMap.count(potentialDelim + nextCh)) {
                        tokens.push_back({ potentialDelim + nextCh, tokenMap[potentialDelim + nextCh] });
                        ++idx;
                    }
                    else if (tokenMap.count(potentialDelim)) {
                        tokens.push_back({ potentialDelim, tokenMap[potentialDelim] });
                    }
                }
            }

            break;

        case 1:
            if (isLetter(ch) || isDigit(ch)) {
                currentToken += ch;
            }
            else {
                if (!tokenMap.count(currentToken))
                {
                    if (!symbolTable.count(currentToken))
                    {
                        ++identifierCount;
                        symbolTable[currentToken] = { "I", identifierCount };
                    }
                    tokens.push_back({ currentToken, symbolTable[currentToken] });
                }
                else
                    tokens.push_back({ currentToken, tokenMap[currentToken] });


                currentToken.clear();
                state = 0;
                --idx;
            }
            break;

        case 2:
            if (isDigit(ch))
            {
                currentToken += ch;
            }
            else if (ch == 'x' || ch == 'X')
            {
                currentToken += ch;
                state = 8;
            }
            else if (ch == 'e' || ch == 'E') {
                currentToken += ch;
                state = 6;
            }
            else if (ch == '.' && isDigit(nextCh))
            {
                currentToken += ch;
                state = 5;
            }
            else {
                if (!symbolTable.count(currentToken)) {
                    ++constantIntCount;
                    symbolTable[currentToken] = { "C1", constantIntCount };

                }

                tokens.push_back({ currentToken, symbolTable[currentToken] });
                currentToken.clear();
                state = 0;
                --idx;
            }
            break;

        case 3:
            if (ch == '\\' && nextCh != '\0') {
                currentToken += ch;
                currentToken += nextCh;
                ++idx;
            }
            else {
                currentToken += ch;
                if (nextCh == '\'') {

                    if (currentToken.length() == 2)
                    {
                        currentToken += nextCh;
                        ++constantCharCount;
                        symbolTable[currentToken] = { "CT", constantCharCount };
                        tokens.push_back({ currentToken, symbolTable[currentToken] });
                        currentToken.clear();
                        state = 0;
                        ++idx;
                    }
                    else
                    {
                        cout << "ERROR" << endl;
                        isError = 1;
                        return tokens;
                    }

                }
            }
            break;

        case 4:
            if (ch == '\\' && nextCh != '\0') {
                currentToken += ch;
                currentToken += nextCh;
                ++idx;
            }
            else {
                currentToken += ch;
                if (nextCh == '\"') {
                    currentToken += nextCh;
                    ++constantStringCount;
                    symbolTable[currentToken] = { "ST", constantStringCount };
                    tokens.push_back({ currentToken, symbolTable[currentToken] });
                    currentToken.clear();
                    state = 0;
                    ++idx;
                }
            }
            break;

        case 5:
            if (isDigit(ch)) {
                currentToken += ch;
            }
            else if (ch == 'e' || ch == 'E') {
                currentToken += ch;
                state = 6;
            }
            else {
                ++constantRealCount;
                symbolTable[currentToken] = { "C2", constantRealCount };
                tokens.push_back({ currentToken, symbolTable[currentToken] });
                currentToken.clear();
                state = 0;
                --idx;
            }
            break;

        case 6:
            if (ch == '-') {
                currentToken += ch;
            }
            else if (isDigit(ch)) {
                currentToken += ch;
                state = 7;
            }
            else {
                cout << "ERROR" << endl;
                isError = 1;
                return tokens;
            }
            break;

        case 7:
            if (isDigit(ch)) {
                currentToken += ch;
            }
            else {
                ++constantRealCount;
                symbolTable[currentToken] = { "C2", constantRealCount };
                tokens.push_back({ currentToken, symbolTable[currentToken] });
                currentToken.clear();
                state = 0;
                --idx;
            }
            break;

        case 8:
            if (isHexDigit(ch))
            {
                currentToken += ch;
            }
            else if (!symbolTable.count(currentToken))
            {
                int num = strtol(currentToken.c_str(), NULL, 16);

                if (!symbolTable.count(to_string(num))) {
                    ++constantIntCount;
                    symbolTable[to_string(num)] = { "C1", constantIntCount };

                }

                tokens.push_back({ to_string(num), symbolTable[to_string(num)] });
                currentToken.clear();
                state = 0;
                --idx;
            }
            else
            {
                cout << "ERROR" << endl;
                isError = 1;
                return tokens;
            }
            break;



        }
    }



    return tokens;
}

vector<string> get_AnswerTokens()
{
    initTokenMap();
    string code;
    getline(cin, code);
    auto tokens = lexAnalyse(code);
    vector<string> answer_tokens;

    if (!isError)
    {
        vector<pair<string, TokenInfo>> sortedSymbols(symbolTable.begin(), symbolTable.end());

        sort(sortedSymbols.begin(), sortedSymbols.end(), [](const pair<string, TokenInfo>& a, const pair<string, TokenInfo>& b) {
            return a.second.id < b.second.id;
            });

        for (auto& token : tokens)
        {
            if (token.second.type == "P")
            {
                answer_tokens.push_back(token.first);
            }
            else
            {
                answer_tokens.push_back("id");
            }
        }
    }

    return answer_tokens;
}

//语法分析
map<string, map<string, string>> parseTable;
vector<string> tokens;
int currentTokenIndex = 0;

/*
E  -> TE'
E' -> +TE' | -TE' | ε
T  -> FT'
T' -> *FT' | /FT' | ε
F  -> (E) | id
*/

void initializeParseTable() //select集
{
    parseTable["E"]["id"] = "T E'";
    parseTable["E"]["("] = "T E'";
    parseTable["E'"]["+"] = "+ T E'";
    parseTable["E'"]["-"] = "- T E'";
    parseTable["E'"][")"] = "ε";
    parseTable["E'"]["$"] = "ε";
    parseTable["T"]["id"] = "F T'";
    parseTable["T"]["("] = "F T'";
    parseTable["T'"]["+"] = "ε";
    parseTable["T'"]["-"] = "ε";
    parseTable["T'"]["*"] = "* F T'";
    parseTable["T'"]["/"] = "/ F T'";
    parseTable["T'"][")"] = "ε";
    parseTable["T'"]["$"] = "ε";
    parseTable["F"]["id"] = "id";
    parseTable["F"]["("] = "( E )";
}

string getNextToken() {
    if (currentTokenIndex < tokens.size()) {
        return tokens[currentTokenIndex++];
    }
    return "";
}

void resetParser() {
    tokens = get_AnswerTokens();
    tokens.push_back("$");
    currentTokenIndex = 0;
}

bool parse() {
    stack<string> parseStack;
    parseStack.push("$");
    parseStack.push("E");

    string currentToken = getNextToken();

    while (!parseStack.empty()) {
        string top = parseStack.top();
        parseStack.pop();

        if (top == currentToken) {
            currentToken = getNextToken();
        }
        else if (parseTable.find(top) != parseTable.end()) {
            if (parseTable[top].find(currentToken) != parseTable[top].end()) {
                string rule = parseTable[top][currentToken];
                if (rule != "ε") {
                    vector<string> symbols;
                    stringstream ss(rule);
                    string symbol;
                    while (ss >> symbol) {
                        symbols.push_back(symbol);
                    }
                    for (int i = symbols.size() - 1; i >= 0; --i) {
                        parseStack.push(symbols[i]);
                    }
                }
            }
            else {
                return false;
            }
        }
        else {
            return false;
        }
    }
}

int main() {
    initializeParseTable();
    resetParser();

    if (parse()) {
        cout << "True" << endl;
    }
    else {
        cout << "False" << endl;
    }

    return 0;
}