//
// Created by wolala on 24-6-12.
//

#ifndef COMPILEFINALWORK_INCLUDE_GRAMMAR_H
#define COMPILEFINALWORK_INCLUDE_GRAMMAR_H

#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using FirstSet = std::map<std::string, std::set<std::string>>;
using FollowSet = std::map<std::string, std::set<std::string>>;
using SelectSet = std::map<std::pair<std::string, std::vector<std::string>>, std::set<std::string>>;
using ParseTable = std::map<std::string, std::map<std::string, std::string>>;
using Productions = std::map<std::string, std::vector<std::vector<std::string>>>;

class Grammar {
public:
    Grammar() = default;
    void initGrammar(const std::string &filename);
    void compute();
    void write_sets_to_file(const std::string &filename);

    [[nodiscard]] inline FirstSet getFirstSet() const { return first; }
    [[nodiscard]] inline FollowSet getFollowSet() const { return follow; }
    [[nodiscard]] inline SelectSet getSelectSet() const { return select; }
    [[nodiscard]] inline ParseTable getParseTable() const { return parseTable; }
    [[nodiscard]] inline std::set<std::string> getNonTerminals() const { return non_terminals; }
    [[nodiscard]] inline std::set<std::string> getTerminals() const { return terminals; }

private:
    Productions productions;
    std::set<std::string> non_terminals;
    std::set<std::string> terminals;
    FirstSet first;
    FollowSet follow;
    SelectSet select;
    ParseTable parseTable;

    void read_grammar_from_file(const std::string &filename);
    void compute_first();
    void compute_follow();
    void compute_select();
    void construct_parseTable();

};

#endif//COMPILEFINALWORK_INCLUDE_GRAMMAR_H
