#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <fstream>
#include <sstream>
#include <stack>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>

using namespace std;

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

	vector<string> keywordList =
		{"int", "void", "char", "break", "float", "while", "do", "struct", "const", "case", "for", "return", "if",
		 "default", "else", "goto", "functioncall"};
	for (int i = 0; i < keywordList.size(); ++i) {
		tokenMap[keywordList[i]] = {"K", i + 1};
	}

	vector<string> delimiterList =
		{"-", "/", "(", ")", "==", "!=", "<=", "<", "+", "*", ">", ">=", "=", ",", ";", "++", "--", "{", "}", "?", "[",
		 "]", "+=", "-="};
	for (int i = 0; i < delimiterList.size(); ++i) {
		tokenMap[delimiterList[i]] = {"P", i + 1};
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

vector<pair<string, TokenInfo>> lexAnalyse(const string &code) {
	vector<pair<string, TokenInfo>> tokens;
	string currentToken;
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
					string potentialDelim = string(1, ch);
					if (nextCh && tokenMap.count(potentialDelim + nextCh)) {
						tokens.push_back({potentialDelim + nextCh, tokenMap[potentialDelim + nextCh]});
						++idx;
					} else if (tokenMap.count(potentialDelim)) {
						tokens.push_back({potentialDelim, tokenMap[potentialDelim]});
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
					tokens.push_back({currentToken, symbolTable[currentToken]});
				} else
					tokens.push_back({currentToken, tokenMap[currentToken]});

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

				tokens.push_back({currentToken, symbolTable[currentToken]});
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
						tokens.push_back({currentToken, symbolTable[currentToken]});
						currentToken.clear();
						state = 0;
						++idx;
					} else {
						cout << "ERROR" << endl;
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
					tokens.push_back({currentToken, symbolTable[currentToken]});
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
				tokens.push_back({currentToken, symbolTable[currentToken]});
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
				cout << "ERROR" << endl;
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
				tokens.push_back({currentToken, symbolTable[currentToken]});
				currentToken.clear();
				state = 0;
				--idx;
			}
			break;

		case 8:
			if (isHexDigit(ch)) {
				currentToken += ch;
			} else if (!symbolTable.count(currentToken)) {
				int num = strtol(currentToken.c_str(), NULL, 16);

				if (!symbolTable.count(to_string(num))) {
					++constantIntCount;
					symbolTable[to_string(num)] = {"C1", constantIntCount};

				}

				tokens.push_back({to_string(num), symbolTable[to_string(num)]});
				currentToken.clear();
				state = 0;
				--idx;
			} else {
				cout << "ERROR" << endl;
				isError = 1;
				return tokens;
			}
			break;

		}
	}

	return tokens;
}

vector<string> get_AnswerTokens() {
	initTokenMap();
	string codes;
	cout << "enter your programe(end at the last line with \"END\")\n";

	while (true) {
		string code;
		getline(cin, code);
		if (code=="END") {
			break;
		}
		codes.append(code);
	}

	cout << "input:" << codes << '\n';

	auto tokens = lexAnalyse(codes);
	vector<string> answer_tokens;

	if (!isError) {
		vector<pair<string, TokenInfo>> sortedSymbols(symbolTable.begin(), symbolTable.end());

		sort(sortedSymbols.begin(),
			 sortedSymbols.end(),
			 [](const pair<string, TokenInfo> &a, const pair<string, TokenInfo> &b) {
			   return a.second.id < b.second.id;
			 });

		for (auto &token : tokens) {
			if (token.second.type=="P" || token.second.type=="K") {
				answer_tokens.push_back(token.first);
			} else if (token.second.type=="I") {
				answer_tokens.push_back("id");
			} else {
				answer_tokens.push_back("number");
			}
		}
	}

	return answer_tokens;
}

class Grammar {
public:
  map<string, vector<vector<string>>> productions;
  set<string> non_terminals;
  set<string> terminals;
  map<string, set<string>> first;
  map<string, set<string>> follow;
  map<pair<string, vector<string>>, set<string>> select;
  map<string, map<string, string>> parseTable;

  Grammar(const map<string, vector<vector<string>>> &productions) {
	  this->productions = productions;

	  for (const auto &prod : productions) {
		  non_terminals.insert(prod.first);
	  }

	  for (const auto &prod : productions) {
		  for (const auto &rhs : prod.second) {
			  for (const auto &symbol : rhs) {

				  if (non_terminals.find(symbol)==non_terminals.end()) {
					  terminals.insert(symbol);
				  }
			  }
		  }
	  }

	  for (const auto &nt : non_terminals) {
		  first[nt] = set<string>();
		  follow[nt] = set<string>();
	  }
	  follow[*non_terminals.begin()].insert("$");

  }

  void compute_first() {
	  bool changed = true;
	  int num = productions.size();
	  int counting_num = 0;
	  while (changed) {
		  changed = false;
		  if (counting_num > 10*num)//FKU
			  break;
		  for (const auto &prod : productions) {
			  const string &lhs = prod.first;

			  counting_num++;
			  for (const auto &rhs : prod.second) {
				  bool epsilon_in_rhs = true;
				  for (const string &symbol : rhs) {
					  if (terminals.find(symbol)!=terminals.end()) {
						  if (first[lhs].insert(symbol).second) {
							  changed = true;
						  }
						  epsilon_in_rhs = false;
						  break;
					  } else {
						  size_t before = first[lhs].size();
						  first[lhs].insert(first[symbol].begin(), first[symbol].end());
						  first[lhs].erase("ε");
						  size_t after = first[lhs].size();

						  if (before!=after) {
							  changed = true;
						  }

						  if (first[symbol].find("ε")==first[symbol].end()) {
							  epsilon_in_rhs = false;
							  break;
						  }
					  }
				  }
				  if (epsilon_in_rhs) {
					  if (first[lhs].insert("ε").second) {
						  changed = true;
					  }
				  }
			  }
		  }
	  }
  }

  void compute_follow() {
	  bool changed = true;
	  while (changed) {
		  changed = false;
		  for (const auto &prod : productions) {
			  const string &lhs = prod.first;
			  for (const auto &rhs : prod.second) {
				  set<string> follow_temp = follow[lhs];

				  /*
				  cout << "follow" << ' ' << lhs << ":\n";
				  for (const auto& f_t : follow_temp)
				  {
					  cout << f_t << ' ';
				  }
				  cout << '\n';
				  */

				  for (auto it = rhs.rbegin(); it!=rhs.rend(); ++it) {
					  const string &symbol = *it;
					  //cout << "symbol " << symbol << '\n';

					  if (non_terminals.find(symbol)!=non_terminals.end()) {
						  size_t before = follow[symbol].size();
						  follow[symbol].insert(follow_temp.begin(), follow_temp.end());
						  size_t after = follow[symbol].size();
						  if (first[symbol].find("ε")!=first[symbol].end()) {
							  follow_temp.insert(first[symbol].begin(), first[symbol].end());
							  follow_temp.erase("ε");
						  } else {
							  follow_temp = first[symbol];
						  }
						  if (before!=after) {
							  changed = true;
						  }
					  } else {
						  follow_temp = {symbol};
					  }
				  }
			  }
		  }
	  }
  }

  void compute_select() {
	  for (const auto &prod : productions) {
		  const string &lhs = prod.first;
		  for (const auto &rhs : prod.second) {
			  set<string> first_rhs;
			  size_t i = 0;
			  while (i < rhs.size()) {
				  const string &symbol = rhs[i];
				  if (terminals.find(symbol)!=terminals.end()) {
					  first_rhs.insert(symbol);
					  break;
				  } else {
					  first_rhs.insert(first[symbol].begin(), first[symbol].end());
					  first_rhs.erase("ε");
					  if (first[symbol].find("ε")!=first[symbol].end()) {
						  i++;
					  } else {
						  break;
					  }
				  }
			  }
			  if (i==rhs.size())
				  first_rhs.insert("ε");
			  select[{lhs, rhs}].insert(first_rhs.begin(), first_rhs.end());
			  if (first_rhs.find("ε")!=first_rhs.end())
				  select[{lhs, rhs}].insert(follow[lhs].begin(), follow[lhs].end());

		  }
	  }

  }

  void construct_parseTable() {
	  for (const auto &prod : select) {
		  const string &lhs = prod.first.first;
		  const vector<string> &rhs = prod.first.second;
		  const set<string> &select_set = prod.second;
		  string parseTable_first;

		  for (const auto &single_aim : rhs) {
			  parseTable_first.append(single_aim);
			  parseTable_first.append(" ");
		  }
		  parseTable_first.erase(parseTable_first.end() - 1);

		  for (const auto &terminal : select_set) {
			  if (terminal!="ε")
				  parseTable[lhs][terminal] = parseTable_first;
		  }

	  }
	  for (const auto &single : parseTable) {
		  for (const auto &single_second : single.second) {
			  //cout << single.first << "->" << single_second.second << ": " << single_second.first << '\n';
		  }
	  }
	  cout << '\n';
  }

  void write_sets_to_file(const string &filename) {
	  ofstream file(filename);

	  file << "FIRST sets:\n";
	  for (const auto &nt : first) {
		  file << "FIRST(" << nt.first << ") = {";
		  for (const auto &sym : nt.second) {
			  file << sym << " ";
		  }
		  file << "}\n";
	  }

	  file << "\nFOLLOW sets:\n";
	  for (const auto &nt : follow) {
		  file << "FOLLOW(" << nt.first << ") = {";
		  for (const auto &sym : nt.second) {
			  file << sym << " ";
		  }
		  file << "}\n";
	  }

	  file << "\nSELECT sets:\n";
	  for (const auto &prod : select) {
		  const string &lhs = prod.first.first;
		  const vector<string> &rhs = prod.first.second;
		  file << "SELECT(" << lhs << " -> ";
		  for (const auto &sym : rhs) {
			  file << sym << " ";
		  }
		  file << ") = {";
		  for (const auto &sym : prod.second) {
			  file << sym << " ";
		  }
		  file << "}\n";
	  }
  }
};

map<string, vector<vector<string>>> read_grammar_from_file(const string &filename) {
	map<string, vector<vector<string>>> productions;
	ifstream file(filename);
	string line;
	int i = 1;

	while (getline(file, line)) {
		istringstream iss(line);
		string lhs;
		string arrow;
		iss >> lhs >> arrow;

		string rhs_str;
		getline(iss, rhs_str);
		istringstream rhs_stream(rhs_str);
		vector<string> rhs_symbols;
		string symbol;

		while (rhs_stream >> symbol) {
			if (symbol=="|") {
				productions[lhs].push_back(rhs_symbols);
				rhs_symbols.clear();
			} else {
				rhs_symbols.push_back(symbol);
			}
		}
		productions[lhs].push_back(rhs_symbols);
		cout << "finish read " << i << endl;
		i++;
	}

	return productions;
}

vector<string> tokens;
int currentTokenIndex = 0;

string getNextToken() {
	if (currentTokenIndex < tokens.size()) {
		return tokens[currentTokenIndex++];
	}
	return "";
}

void resetParser() {
	tokens = get_AnswerTokens();
	cout << "tokens:\n";
	for (const auto &token : tokens) {
		cout << token << ' ';
	}
	cout << '\n';
	tokens.push_back("$");
	currentTokenIndex = 0;
}

bool parse(Grammar grammar) {
	stack<string> parseStack;
	parseStack.push("$");
	parseStack.push("A_Program");

	string currentToken = getNextToken();

	while (!parseStack.empty()) {
		string top = parseStack.top();
		parseStack.pop();
		//cout << "parseStack_Top: " << top << '\n';

		if (top==currentToken) {
			currentToken = getNextToken();
			//cout << "successful match: " << top << " next_token: " << currentToken << '\n';
		} else if (grammar.parseTable.find(top)!=grammar.parseTable.end()) {
			cout << "Number" << currentTokenIndex << ":" << top << " " << currentToken << "rest " << currentTokenIndex
				 << '\n';
			if (grammar.parseTable[top].find(currentToken)!=grammar.parseTable[top].end()) {
				string rule = grammar.parseTable[top][currentToken];
				if (rule!="ε") {
					vector<string> symbols;
					stringstream ss(rule);
					string symbol;

					while (ss >> symbol) {
						symbols.push_back(symbol);

					}
					for (int i = symbols.size() - 1; i >= 0; --i) {
						parseStack.push(symbols[i]);
						cout << "Number" << currentTokenIndex << ":" << currentToken << " Rule: " << top << "->" << rule
							 << " Push {" << symbols[i] << "} rest:" << parseStack.size() << "with i = " << i << "\n";
					}
				}
			} else {
				return false;
			}
		} else {
			cout << top << ' ' << currentToken << ' ' << currentTokenIndex << ' ';
			return false;
		}

	}
}

int main() {
	string input_filename = "grammar02.txt";
	string output_sets_filename = "output_sets.txt";
	//文法中，函数调用需要使用funtioncall关键字，比如：  int a = functioncall add(1,2);
	//能够识别const if else while 数组
	//不支持结构体 类型转换 字符字符串 void关键字
	//按照要求输入，即最后一行输入END作为结束
	//只包含前端代码，请添加UI界面
	map<string, vector<vector<string>>> productions = read_grammar_from_file(input_filename);
	Grammar grammar(productions);
	cout << "grammar finish computing\n";
	grammar.compute_first();
	grammar.compute_follow();
	grammar.compute_select();
	grammar.construct_parseTable();
	grammar.write_sets_to_file(output_sets_filename);

	resetParser();
	if (parse(grammar)) {
		cout << "True" << endl;
	} else {
		cout << "False" << endl;
	}
	return 0;
}
