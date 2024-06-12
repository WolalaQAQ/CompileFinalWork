//
// Created by Adminstrator on 24-6-13.
//

#include "ir_generator.h"

#include <iostream>
#include <utility>

void IRGenerator::add_computing_quarter(int temp, std::string com_w) {
    std::pair<std::string, std::string> num2 = CandIstack.top();
    CandIstack.pop();
    std::pair<std::string, std::string> num1 = CandIstack.top();
    CandIstack.pop();
    std::string temp_answer = "v";
    temp_answer.append(std::to_string(temp));
    std::string temp_answer_type;
    if (num1.second == "float" || num2.second == "float")
        temp_answer_type = "float";
    else
        temp_answer_type = "int";
    count_quar++;
    Quadruple quarter = {count_quar, std::move(com_w), num1.first, num2.first, temp_answer, num1.second, num2.second, temp_answer_type};
    quar.push_back(quarter);
    count_symb++;
    Symbol symbol = {count_symb, temp_answer, temp_answer_type, "v", address_temp};
    vall.emplace(temp_answer, address_temp);
    address_temp += 4;
    symb.push_back(symbol);
    CandIstack.emplace(temp_answer, temp_answer_type);
}

void IRGenerator::add_comparing_quarter(int temp, std::string com_w) {
    std::pair<std::string, std::string> num2 = CandIstack.top();
    CandIstack.pop();
    std::pair<std::string, std::string> num1 = CandIstack.top();
    CandIstack.pop();
    std::string temp_answer = "v";
    temp_answer.append(std::to_string(temp));
    count_quar++;
    Quadruple quarter = {count_quar, std::move(com_w), num1.first, num2.first, temp_answer, num1.second, num2.second, "int"};
    quar.push_back(quarter);
    count_symb++;
    Symbol symbol = {count_symb, temp_answer, "int", "v", address_temp};
    vall.emplace(temp_answer, address_temp);
    address_temp += 4;
    symb.push_back(symbol);
    CandIstack.emplace(temp_answer, "int");
}

void IRGenerator::add_assignment_quarter() {
    std::pair<std::string, std::string> num2 = CandIstack.top();
    CandIstack.pop();
    std::pair<std::string, std::string> num1 = CandIstack.top();
    CandIstack.pop();

    count_quar++;
    Quadruple quarter = {count_quar, ":=", num2.first, "_", num1.first, "_", "_", "_"};
    quar.push_back(quarter);
}

void IRGenerator::form(const std::vector<std::pair<std::string, TokenInfo>>& tokens) {
    int temp_list = 0;
    for (auto &token: tokens) {
        if (token.second.type == "K") {
            if (token.first == "while") {
                while_count++;
                count_quar++;
                Quadruple _wh = {count_quar, "wh" + std::to_string(while_count), "_", "_", "_", "_", "_", "_"};
                quar.push_back(_wh);
                while_loop[while_count] = 1;
                Kstack_temp.emplace("while");
            } else if (token.first == "else") {
                if_count++;
                if_loop[if_count] = 3;
                quar.pop_back();
                Quadruple _else = {count_quar, "el" + std::to_string(if_count), "_", "_", "*p", "_", "_", "_"};
                quar.push_back(_else);
                Kstack_temp.push(token.first);
            } else if (token.first == "if") {
                Kstack_temp.push(token.first);
                Kstack.push(token.first);
            } else if (token.first == "return") {
                Kstack_temp.push(token.first);
            } else {
                Kstack.push(token.first);
            }
        } else if (token.second.type == "P") {
            if (token.first == "+" || token.first == "-") {
                if (P_computing_stack.empty() || P_computing_stack.top() == "(") {
                    P_computing_stack.push(token.first);
                } else {
                    while (!(P_computing_stack.empty() || P_computing_stack.top() == "(")) {
                        std::string computing_sign = P_computing_stack.top();
                        P_computing_stack.pop();
                        add_computing_quarter(temp_list, computing_sign);
                        temp_list++;
                    }
                    P_computing_stack.push(token.first);
                }
            } else if (token.first == "*" || token.first == "/") {
                if (P_computing_stack.top() == "*" || P_computing_stack.top() == "/") {
                    while (P_computing_stack.top() == "*" || P_computing_stack.top() == "/") {
                        std::string computing_sign = P_computing_stack.top();
                        P_computing_stack.pop();
                        add_computing_quarter(temp_list, computing_sign);
                        temp_list++;
                    }
                    P_computing_stack.push(token.first);
                } else {
                    P_computing_stack.push(token.first);
                }
            } else if (token.first == "(") {
                P_computing_stack.push(token.first);
                if (!CandIstack.empty() && Assignmentstack.empty() && P_comparing_stack.empty()) {
                    std::pair<std::string, std::string> program = CandIstack.top();
                    CandIstack.pop();
                    count_quar++;
                    Quadruple _program = {count_quar, "program", program.first, "_", "_", "_", "_", "_"};
                    symb.pop_back();
                    vall.pop();
                    address_temp -= 4;
                    Symbol program_ = {count_symb, program.first, "null", "f", 0};
                    symb.push_back(program_);
                    quar.push_back(_program);
                }
            } else if (token.first == "<" || token.first == "<=" || token.first == ">" || token.first == ">=" || token.first == "==" || token.first == "!=") {
                P_comparing_stack.push(token.first);
            } else if (token.first == "=") {
                Assignmentstack.push(token.first);
            } else if (token.first == ")") {
                while (P_computing_stack.top() != "(") {
                    std::string computing_sign = P_computing_stack.top();
                    P_computing_stack.pop();
                    add_computing_quarter(temp_list, computing_sign);
                    temp_list++;
                }
                if (P_computing_stack.top() == "(")
                    P_computing_stack.pop();
                if (!P_comparing_stack.empty()) {
                    if (CandIstack.size() == 2) {
                        std::string comparing_sign = P_comparing_stack.top();
                        P_comparing_stack.pop();
                        add_comparing_quarter(temp_list, comparing_sign);
                        temp_list++;
                    }
                }
                if (!Kstack.empty()) {
                    std::string keyword = Kstack.top();
                    Kstack.pop();
                    if (keyword == "if") {
                        if (!CandIstack.empty()) {
                            std::pair<std::string, std::string> comparing_answer = CandIstack.top();
                            if_count++;
                            CandIstack.pop();
                            count_quar++;
                            Quadruple _if = {count_quar, "if" + std::to_string(if_count), comparing_answer.first, "_", "*p", "_", "_", "_"};
                            quar.push_back(_if);
                            if_loop[if_count] = 1;
                        }
                    } else {
                        Kstack.push(keyword);
                    }
                }
                if (while_loop[while_count] == 1) {
                    if (P_computing_stack.empty() || P_computing_stack.top() != "(") {
                        std::string result_comparing = (*(quar.end() - 1)).result;
                        count_quar++;
                        CandIstack.pop();
                        Quadruple _do = {count_quar, "do" + std::to_string(while_count), result_comparing, "_", "*p", "_", "_", "_"};
                        quar.push_back(_do);
                        while_loop[while_count] = 2;
                    }
                }
            } else if (token.first == ";") {
                if (!P_computing_stack.empty() || !P_comparing_stack.empty()) {
                    while (!P_computing_stack.empty()) {
                        std::string computing_sign = P_computing_stack.top();
                        P_computing_stack.pop();
                        add_computing_quarter(temp_list, computing_sign);
                        temp_list++;
                    }
                    while (!P_comparing_stack.empty()) {
                        std::string comparing_sign = P_comparing_stack.top();
                        P_comparing_stack.pop();
                        add_comparing_quarter(temp_list, comparing_sign);
                        temp_list++;
                    }
                }
                if (!Assignmentstack.empty()) {
                    if (CandIstack.size() >= 2) {
                        add_assignment_quarter();
                    }
                }
                if (!Kstack_temp.empty()) {
                    if (Kstack_temp.top() == "return" && !CandIstack.empty()) {
                        count_quar++;
                        Quadruple _return = {count_quar, "return", CandIstack.top().first, "_", "_", "_", "_", "_"};
                        quar.push_back(_return);
                        CandIstack.pop();
                        int symb_size = symb.size();
                        for (int i = symb_size - 1; i >= 0; i--) {
                            if (symb[i].category == "f") {
                                symb[i].address = address_temp;
                                address_temp += 4;
                            }
                        }
                    }
                }
            } else if (token.first == "}") {
                if (if_loop[if_count] != 0 && (Kstack_temp.top() == "if" || Kstack_temp.top() == "else")) {
                    count_quar++;
                    Quadruple _if_end = {count_quar, "ie" + std::to_string(if_count), "_", "_", "_", "_", "_", "_"};
                    quar.push_back(_if_end);
                    if_loop[if_count]++;
                    if_count--;
                    Kstack_temp.pop();
                }
                if (while_loop[while_count] == 2 && Kstack_temp.top() == "while") {
                    count_quar++;
                    Quadruple _while_end = {count_quar, "we" + std::to_string(while_count), "_", "_", "*p", "_", "_", "_"};
                    quar.push_back(_while_end);
                    while_loop[while_count] = 3;
                    while_count--;
                    Kstack_temp.pop();
                }
            }
        } else {
            CandIstack.emplace(token.first, token.second.type);
            if (!Kstack.empty()) {
                if (token.second.type == "I") {
                    bool is_symboled = false;
                    for (const auto &s: symb) {
                        if (s.name == token.first) {
                            is_symboled = true;
                            break;
                        }
                    }
                    if (!is_symboled) {
                        std::string s_type;
                        if (Kstack.top() == "float")
                            s_type = "float";
                        else
                            s_type = "int";

                        count_symb++;

                        Symbol symbol = {count_symb, token.first, s_type, "v", address_temp};
                        symb.push_back(symbol);
                        vall.emplace(token.first, address_temp);
                        address_temp += 4;
                    }
                }
            }
        }
    }
    int quar_size = quar.size();
    for (int i = 0; i < quar_size; i++) {
        if (quar[i].result == "*p") {
            if (quar[i].op[0] == 'i' && quar[i].op[1] == 'f') {
                int j;
                for (j = i; j < quar_size; j++) {
                    if (quar[j].op[0] == 'i' && quar[j].op[1] == 'e' && quar[i].op[2] == quar[j].op[2]) {
                        quar[i].result = "(" + std::to_string(quar[j].index) + ")";
                        break;
                    }
                }
                for (int k = j; k > i; k--) {
                    if (quar[k].op[0] == 'e' && quar[k].op[1] == 'l' && quar[k].op[2] == quar[j].op[2]) {
                        quar[k].result = "(" + std::to_string(quar[j].index) + ")";
                        quar[i].result = "(" + std::to_string(quar[k].index + 1) + ")";
                        break;
                    }
                }
            }
        }
        if (quar[i].op[0] == 'w' && quar[i].op[1] == 'h') {
            int j;
            for (j = i; j < quar_size; j++) {
                if (quar[j].op[0] == 'w' && quar[j].op[1] == 'e' && quar[i].op[2] == quar[j].op[2]) {
                    quar[j].result = "(" + std::to_string(quar[i].index + 1) + ")";
                    break;
                }
            }
            for (int k = j; k > i; k--) {
                if (quar[k].op[0] == 'd' && quar[k].op[1] == 'o' && quar[k].op[2] == quar[j].op[2]) {
                    if (j <= quar_size) {
                        quar[k].result = "(" + std::to_string(quar[j].index + 1) + ")";
                    } else {
                        quar[k].result = "$";
                    }
                    break;
                }
            }
        }
    }
    std::cout << "Quadruple:\n";
    for (const auto &q: quar) {
        std::cout << "(" << q.index << ")  (" << q.op << ", " << q.x1 << ", " << q.x2 << ", " << q.result << ")" << "\n";
    }
    std::cout << "Symbol:\n";
    std::cout << "\tNAME\tTYPE\tCAT\tADDR\n";
    for (const auto &s: symb) {
        std::cout << "(" << s.index << "):\t" << s.name << "\t" << s.type << "\t" << s.category << "\t" << s.address << "\n";
    }
    std::cout << "VALL:\n";
    while (!vall.empty()) {
        std::cout << vall.top().first << '\t' << vall.top().second << '\n';
        vall.pop();
    }
}