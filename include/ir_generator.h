//
// Created by Adminstrator on 24-6-13.
//

#ifndef COMPILEFINALWORK_IR_GENERATOR_H
#define COMPILEFINALWORK_IR_GENERATOR_H

#include "scanner.h"
#include <stack>
#include <string>
#include <utility>
#include <vector>

struct Quadruple {
    int index;
    std::string op;
    std::string x1;
    std::string x2;
    std::string result;
    std::string x1_type;
    std::string x2_type;
    std::string result_type;
};

struct Symbol {
    int index;
    std::string name;
    std::string type;
    std::string category;
    int address;
};

class IRGenerator {
public:
    IRGenerator() = default;

    void form(const Tokens &tokens);

    [[nodiscard]] inline std::vector<Quadruple> getQuadruples() const { return quar; }
    [[nodiscard]] inline std::vector<Symbol> getSymbolTable() const { return symb; }
    [[nodiscard]] inline std::stack<std::pair<std::string, int>> getVall() const { return vall; }

private:
    std::vector<Quadruple> quar;
    std::vector<Symbol> symb;
    std::stack<std::pair<std::string, int>> vall;
    std::stack<std::string> P_computing_stack;
    std::stack<std::string> P_comparing_stack;
    std::stack<std::string> Kstack;
    std::stack<std::string> Kstack_temp;
    std::stack<std::pair<std::string, std::string>> CandIstack;
    std::stack<std::string> Assignmentstack;
    std::vector<int> if_loop{0, 0, 0, 0, 0};
    std::vector<int> while_loop{0, 0, 0, 0, 0};
    int if_count = 0;
    int while_count = 0;
    int count_quar = 0;
    int count_symb = 0;
    int address_temp = 0;

    void add_computing_quarter(int temp, std::string com_w);
    void add_comparing_quarter(int temp, std::string com_w);
    void add_assignment_quarter();

};


#endif//COMPILEFINALWORK_IR_GENERATOR_H
