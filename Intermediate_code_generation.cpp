#include <iostream>
#include <string>
#include <set>
#include <map>
#include <vector>
#include <algorithm>
#include <stack>

using namespace std;

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


    vector<string> delimiterList = { "-", "/", "(", ")","==", "<=", "<", ">=", "!=", "+", "*", ">", "=", ",", ";", "++", "{", "}" };
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
            if (isDigit(ch) || ch == 'x' || ch == 'X')
            {
                currentToken += ch;
            }
            else if (ch == '.' && isDigit(nextCh))
            {
                currentToken += ch;
                state = 5;
            }
            else {
                if (!symbolTable.count(currentToken)) {
                    if (currentToken.length() > 2 && currentToken[0] == '0' && currentToken[1] == 'x') {
                        ++constantIntCount;
                        symbolTable[currentToken] = { "C1", constantIntCount };
                    }
                    else {
                        ++constantIntCount;
                        symbolTable[currentToken] = { "C1", constantIntCount };
                    }
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
                    currentToken += nextCh;
                    ++constantCharCount;
                    symbolTable[currentToken] = { "CT", constantCharCount };
                    tokens.push_back({ currentToken, symbolTable[currentToken] });
                    currentToken.clear();
                    state = 0;
                    ++idx;
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
                cout << "Error" << endl;
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



        }
    }



    return tokens;
}

struct Quater_sequence {
    int c;
    string w;
    string o1;
    string o2;
    string t;
    string o1_type;
    string o2_type;
    string t_type;
};

struct Symbol_table {
    int c;
    string name;
    string type;
    string cat;
    int addr;
};

vector<Quater_sequence> quar;
vector<Symbol_table> symb;
stack<pair<string, int>> vall;
stack<string> P_computing_stack;
stack<string> P_comparing_stack;
stack<string> Kstack;
stack<string> Kstack_temp;
stack<pair<string, string>> CandIstack;
stack<string> Assignmentstack;
vector<int> if_loop(5, 0);
vector<int> while_loop(5, 0);
int if_count = 0;
int while_count = 0;
int count_quar = 0;
int count_symb = 0;
int address_temp = 0;

void add_computing_quarter(int temp, string com_w)
{
    pair<string,string> num2 = CandIstack.top();
    CandIstack.pop();
    pair<string,string> num1 = CandIstack.top();
    CandIstack.pop();
    string temp_answer = "v";
    temp_answer.append(to_string(temp));
    string temp_answer_type;
    if (num1.second == "float" || num2.second == "float")
        temp_answer_type = "float";
    else
        temp_answer_type = "int";
    count_quar++;
    Quater_sequence quarter = { count_quar, com_w, num1.first, num2.first, temp_answer, num1.second, num2.second, temp_answer_type };
    quar.push_back(quarter);
    count_symb++;
    Symbol_table symbol = { count_symb, temp_answer, temp_answer_type, "v", address_temp };
    vall.push({ temp_answer, address_temp });
    address_temp += 4;
    symb.push_back(symbol);
    CandIstack.push({ temp_answer, temp_answer_type });
}

void add_comparing_quarter(int temp, string com_w)
{
    pair<string,string> num2 = CandIstack.top();
    CandIstack.pop();
    pair<string,string> num1 = CandIstack.top();
    CandIstack.pop();
    string temp_answer = "v";
    temp_answer.append(to_string(temp));
    count_quar++;
    Quater_sequence quarter = { count_quar, com_w, num1.first, num2.first, temp_answer, num1.second, num2.second, "int"};
    quar.push_back(quarter);
    count_symb++;
    Symbol_table symbol = { count_symb, temp_answer, "int",  "v", address_temp };
    vall.push({ temp_answer, address_temp });
    address_temp += 4;
    symb.push_back(symbol);
    CandIstack.push({ temp_answer, "int" });
}

void add_assignment_quarter()
{
    pair<string, string> num2 = CandIstack.top();
    CandIstack.pop();
    pair<string, string> num1 = CandIstack.top();
    CandIstack.pop();

    count_quar++;
    Quater_sequence quarter = { count_quar,":=", num2.first, "_" , num1.first, "_", "_", "_"};
    quar.push_back(quarter);
}

void form(vector<pair<string, TokenInfo>> tokens)
{
    int temp_list = 0;
    for (auto& token : tokens)
    {
        if (token.second.type == "K")
        {
            if (token.first == "while")
            {
                while_count++;
                count_quar++;
                Quater_sequence _wh = { count_quar,"wh" + to_string(while_count), "_", "_", "_","_", "_", "_" };
                quar.push_back(_wh);
                while_loop[while_count] = 1;
                Kstack_temp.push("while");
            }
            else if (token.first == "else")
            {
                if_count++;
                if_loop[if_count] = 3;
                quar.pop_back();

                Quater_sequence _else = { count_quar,"el" + to_string(if_count), "_", "_", "*p","_", "_", "_" };
                quar.push_back(_else);
                Kstack_temp.push(token.first);

            }
            else if (token.first == "if")
            {
                Kstack_temp.push(token.first);
                Kstack.push(token.first);
            }
            else if (token.first == "return")
            {
                Kstack_temp.push(token.first);
            }
            else
            {
                Kstack.push(token.first);
            }
        }
        else if (token.second.type == "P")
        {
            if (token.first == "+" || token.first == "-")
            {
                if (P_computing_stack.empty() || P_computing_stack.top() == "(")
                {
                    P_computing_stack.push(token.first);
                }
                else
                {
                    while (!(P_computing_stack.empty() || P_computing_stack.top() == "("))
                    {
                        string computing_sign = P_computing_stack.top();
                        P_computing_stack.pop();

                        add_computing_quarter(temp_list, computing_sign);
                        temp_list++;
                    }
                    P_computing_stack.push(token.first);
                }
            }
            else if (token.first == "*" || token.first == "/")
            {
                if (P_computing_stack.top() == "*" || P_computing_stack.top() == "/")
                {
                    while (P_computing_stack.top() == "*" || P_computing_stack.top() == "/")
                    {
                        string computing_sign = P_computing_stack.top();
                        P_computing_stack.pop();

                        add_computing_quarter(temp_list, computing_sign);
                        temp_list++;
                    }
                    P_computing_stack.push(token.first);
                }
                else
                {
                    P_computing_stack.push(token.first);
                }
            }
            else if (token.first == "(")
            {
                P_computing_stack.push(token.first);
                if (!CandIstack.empty() && Assignmentstack.empty() && P_comparing_stack.empty())
                {
                    pair<string, string> program = CandIstack.top();
                    CandIstack.pop();
                    count_quar++;
                    Quater_sequence _program = { count_quar, "program", program.first, "_", "_", "_", "_", "_"};
                    symb.pop_back();
                    vall.pop();
                    address_temp -= 4;

                    Symbol_table program_ = { count_symb, program.first, "null", "f", 0 };
                    symb.push_back(program_);
                    quar.push_back(_program);
                }
            }
            else if (token.first == "<" || token.first == "<=" || token.first == ">" || token.first == ">=" || token.first == "==" || token.first == "!=")
            {
                P_comparing_stack.push(token.first);
            }
            else if (token.first == "=")
            {
                Assignmentstack.push(token.first);
            }
            else if (token.first == ")")
            {
                while (P_computing_stack.top() != "(")
                {
                    string computing_sign = P_computing_stack.top();
                    P_computing_stack.pop();

                    add_computing_quarter(temp_list, computing_sign);
                    temp_list++;
                }
                if (P_computing_stack.top() == "(")
                    P_computing_stack.pop();

                if (!P_comparing_stack.empty())
                {
                    if (CandIstack.size() == 2)
                    {
                        string comparing_sign = P_comparing_stack.top();
                        P_comparing_stack.pop();
                        add_comparing_quarter(temp_list, comparing_sign);
                        temp_list++;
                    }
                }

                if (!Kstack.empty())
                {
                    string keyword = Kstack.top();
                    Kstack.pop();
                    if (keyword == "if")
                    {
                        if (!CandIstack.empty())
                        {
                            pair<string, string> comparing_answer = CandIstack.top();
                            if_count++;
                            CandIstack.pop();
                            count_quar++;
                            Quater_sequence _if = { count_quar, "if" + to_string(if_count), comparing_answer.first, "_", "*p","_", "_", "_" };
                            quar.push_back(_if);

                            if_loop[if_count] = 1;
                        }
                    }
                    else
                    {
                        Kstack.push(keyword);
                    }
                }

                if (while_loop[while_count] == 1)
                {
                    if (P_computing_stack.empty() || P_computing_stack.top() != "(")
                    {
                        string result_comparing = (*(quar.end() - 1)).t;
                        count_quar++;
                        CandIstack.pop();
                        Quater_sequence _do = { count_quar, "do" + to_string(while_count), result_comparing, "_", "*p","_", "_", "_" };
                        quar.push_back(_do);
                        while_loop[while_count] = 2;

                    }
                }

            }
            else if (token.first == ";")
            {
                if (!P_computing_stack.empty() || !P_comparing_stack.empty())
                {
                    while (!P_computing_stack.empty())
                    {
                        string computing_sign = P_computing_stack.top();
                        P_computing_stack.pop();

                        add_computing_quarter(temp_list, computing_sign);
                        temp_list++;
                    }
                    while (!P_comparing_stack.empty())
                    {
                        string comparing_sign = P_comparing_stack.top();
                        P_comparing_stack.pop();

                        add_comparing_quarter(temp_list, comparing_sign);
                        temp_list++;
                    }
                }

                if (!Assignmentstack.empty())
                {
                    if (CandIstack.size() >= 2)
                    {
                        add_assignment_quarter();
                    }
                }

                if (!Kstack_temp.empty())
                {
                    if (Kstack_temp.top() == "return" && !CandIstack.empty())
                    {
                        count_quar++;
                        Quater_sequence _return = { count_quar, "return", CandIstack.top().first, "_", "_", "_", "_", "_" };
                        quar.push_back(_return);
                        CandIstack.pop();

                        int symb_size = symb.size();
                        for (int i = symb_size - 1; i >= 0; i--)
                        {
                            if (symb[i].cat == "f")
                            {
                                symb[i].addr = address_temp;
                                address_temp += 4;
                            }
                        }
                    }
                }


            }
            else if (token.first == "}")
            {
                if (if_loop[if_count] != 0 && (Kstack_temp.top() == "if" || Kstack_temp.top() == "else"))
                {
                    count_quar++;
                    Quater_sequence _if_end = { count_quar, "ie" + to_string(if_count), "_", "_", "_", "_", "_", "_" };
                    quar.push_back(_if_end);
                    if_loop[if_count]++;
                    if_count--;
                    Kstack_temp.pop();
                }
                if (while_loop[while_count] == 2 && Kstack_temp.top() == "while")
                {
                    count_quar++;
                    Quater_sequence _while_end = { count_quar, "we" + to_string(while_count), "_", "_", "*p", "_", "_", "_" };
                    quar.push_back(_while_end);
                    while_loop[while_count] = 3;
                    while_count--;
                    Kstack_temp.pop();
                }
            }
        }
        else
        {
            CandIstack.push({ token.first, token.second.type });
            if (!Kstack.empty())
            {
                if (token.second.type == "I")
                {
                    bool is_symboled = 0;
                    for (const auto& s : symb)
                    {
                        if (s.name == token.first)
                        {
                            is_symboled = 1;
                            break;
                        }
                    }

                    if (!is_symboled)
                    {
                        string s_type;
                        if (Kstack.top() == "float")
                            s_type = "float";
                        else
                            s_type = "int";

                        count_symb++;
                        
                        Symbol_table symbol = { count_symb, token.first, s_type, "v", address_temp };
                        symb.push_back(symbol);
                        vall.push({ token.first, address_temp });
                        address_temp += 4;

                    }
                }
            }
        }
    }

    int quar_size = quar.size();
    for (int i = 0; i < quar_size; i++)
    {
        if (quar[i].t == "*p")
        {
            if (quar[i].w[0] == 'i' && quar[i].w[1] == 'f')
            {
                int j;
                for (j = i; j < quar_size; j++)
                {
                    if (quar[j].w[0] == 'i' && quar[j].w[1] == 'e' && quar[i].w[2] == quar[j].w[2])
                    {
                        quar[i].t = "(" + to_string(quar[j].c) + ")";
                        break;
                    }
                }
                for (int k = j; k > i; k--)
                {

                    if (quar[k].w[0] == 'e' && quar[k].w[1] == 'l' && quar[k].w[2] == quar[j].w[2])
                    {
                        quar[k].t = "(" + to_string(quar[j].c) + ")";
                        quar[i].t = "(" + to_string(quar[k].c + 1) + ")";
                        break;
                    }
                }
            }
        }
        if (quar[i].w[0] == 'w' && quar[i].w[1] == 'h')
        {
            int j;
            for (j = i; j < quar_size; j++)
            {
                if (quar[j].w[0] == 'w' && quar[j].w[1] == 'e' && quar[i].w[2] == quar[j].w[2])
                {
                    quar[j].t = "(" + to_string(quar[i].c + 1) + ")";
                    break;
                }
            }
            for (int k = j; k > i; k--)
            {
                if (quar[k].w[0] == 'd' && quar[k].w[1] == 'o' && quar[k].w[2] == quar[j].w[2])
                {
                    if (j <= quar_size)
                    {
                        quar[k].t = "(" + to_string(quar[j].c + 1) + ")";
                    }
                    else
                    {
                        quar[k].t = "$";
                    }
                    break;
                }
            }
        }
    }
    cout << "Quater_sequence:\n";
    for (const auto& q : quar)
    {
        cout << "(" << q.c << ")  (" << q.w << ", " << q.o1 << ", " << q.o2 << ", " << q.t << ")" << "\n";
    }
    cout << "Symbol_table:\n";
    cout << "\tNAME\tTYPE\tCAT\tADDR\n";
    for (const auto& s : symb)
    {
        cout << "(" << s.c << "):\t" << s.name << "\t" << s.type << "\t" << s.cat << "\t" << s.addr << "\n";
    }
    cout << "VALL:\n";
    while (!vall.empty())
    {
        cout << vall.top().first << '\t' << vall.top().second << '\n';
        vall.pop();
    }
}

int main() {
    initTokenMap();
    string code;
    getline(cin, code);
    auto tokens = lexAnalyse(code);

    vector<pair<string, TokenInfo>> sortedSymbols(symbolTable.begin(), symbolTable.end());
    sort(sortedSymbols.begin(), sortedSymbols.end(), [](const pair<string, TokenInfo>& a, const pair<string, TokenInfo>& b)
        {
        return a.second.id < b.second.id;
        });

    cout << "Token :";
    for (auto& token : tokens)
    {
        cout << "(" << token.first << " " << token.second.type << " " << token.second.id << ")";
    }
    cout << "\nI :";
    for (auto& symbol : sortedSymbols)
    {
        if (isLetter(symbol.first[0]))
            cout << symbol.first << ' ';
    }
    cout << "\nC1 :";
    for (auto& symbol : sortedSymbols)
    {
        if (symbol.second.type == "C1")
            cout << symbol.first << ' ';
    }
    cout << "\nC2 :";
    for (auto& symbol : sortedSymbols)
    {
        if (symbol.second.type == "C2")
            cout << symbol.first << ' ';
    }
    cout << "\nCT :";
    for (auto& symbol : sortedSymbols)
    {
        if (symbol.second.type == "CT")
            cout << symbol.first[1] << ' ';
    }
    cout << "\nST :";
    for (auto& symbol : sortedSymbols)
    {
        if (symbol.second.type == "ST")
        {
            for (int i = 1; i < symbol.first.length() - 1; i++)
            {
                cout << symbol.first[i];
            }
            cout << ' ';
        }
    }
    cout << '\n';


    form(tokens);
    return 0;
}
