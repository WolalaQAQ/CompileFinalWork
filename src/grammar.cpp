//
// Created by wolala on 24-6-12.
//
#include "grammar.h"

void Grammar::initGrammar(const std::string &filename) {
    read_grammar_from_file(filename);

    for (const auto &prod: productions) {
        non_terminals.insert(prod.first);
    }

    for (const auto &prod: productions) {
        for (const auto &rhs: prod.second) {
            for (const auto &symbol: rhs) {

                if (non_terminals.find(symbol) == non_terminals.end()) {
                    terminals.insert(symbol);
                }
            }
        }
    }

    for (const auto &nt: non_terminals) {
        first[nt] = std::set<std::string>();
        follow[nt] = std::set<std::string>();
    }

    follow[*non_terminals.begin()].insert("$");
}

void Grammar::compute() {
    compute_first();
    compute_follow();
    compute_select();
    construct_parseTable();
}

void Grammar::read_grammar_from_file(const std::string &filename) {
    std::ifstream file(filename);
    std::string line;
    int i = 1;

    while (getline(file, line)) {
        std::istringstream iss(line);
        std::string lhs;
        std::string arrow;
        iss >> lhs >> arrow;

        std::string rhs_str;
        getline(iss, rhs_str);
        std::istringstream rhs_stream(rhs_str);
        std::vector<std::string> rhs_symbols;
        std::string symbol;

        while (rhs_stream >> symbol) {
            if (symbol == "|") {
                productions[lhs].push_back(rhs_symbols);
                rhs_symbols.clear();
            } else {
                rhs_symbols.push_back(symbol);
            }
        }
        productions[lhs].push_back(rhs_symbols);
//        std::cout << "finish read " << i << std::endl;
        i++;
    }
}

void Grammar::compute_first() {
    bool changed = true;
    int num = productions.size();
    int counting_num = 0;
    while (changed) {
        changed = false;
        if (counting_num > 10 * num)//FKU
            break;
        for (const auto &prod: productions) {
            const std::string &lhs = prod.first;

            counting_num++;
            for (const auto &rhs: prod.second) {
                bool epsilon_in_rhs = true;
                for (const std::string &symbol: rhs) {
                    if (terminals.find(symbol) != terminals.end()) {
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

                        if (before != after) {
                            changed = true;
                        }

                        if (first[symbol].find("ε") == first[symbol].end()) {
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

void Grammar::compute_follow() {
    bool changed = true;
    while (changed) {
        changed = false;
        for (const auto &prod: productions) {
            const std::string &lhs = prod.first;
            for (const auto &rhs: prod.second) {
                std::set<std::string> follow_temp = follow[lhs];

                /*
				cout << "follow" << ' ' << lhs << ":\n";
				for (const auto& f_t : follow_temp)
				{
					cout << f_t << ' ';
				}
				cout << '\n';
				*/

                for (auto it = rhs.rbegin(); it != rhs.rend(); ++it) {
                    const std::string &symbol = *it;
                    //cout << "symbol " << symbol << '\n';

                    if (non_terminals.find(symbol) != non_terminals.end()) {
                        size_t before = follow[symbol].size();
                        follow[symbol].insert(follow_temp.begin(), follow_temp.end());
                        size_t after = follow[symbol].size();
                        if (first[symbol].find("ε") != first[symbol].end()) {
                            follow_temp.insert(first[symbol].begin(), first[symbol].end());
                            follow_temp.erase("ε");
                        } else {
                            follow_temp = first[symbol];
                        }
                        if (before != after) {
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

void Grammar::compute_select() {
    for (const auto &prod: productions) {
        const std::string &lhs = prod.first;
        for (const auto &rhs: prod.second) {
            std::set<std::string> first_rhs;
            size_t i = 0;
            while (i < rhs.size()) {
                const std::string &symbol = rhs[i];
                if (terminals.find(symbol) != terminals.end()) {
                    first_rhs.insert(symbol);
                    break;
                } else {
                    first_rhs.insert(first[symbol].begin(), first[symbol].end());
                    first_rhs.erase("ε");
                    if (first[symbol].find("ε") != first[symbol].end()) {
                        i++;
                    } else {
                        break;
                    }
                }
            }
            if (i == rhs.size())
                first_rhs.insert("ε");
            select[{lhs, rhs}].insert(first_rhs.begin(), first_rhs.end());
            if (first_rhs.find("ε") != first_rhs.end())
                select[{lhs, rhs}].insert(follow[lhs].begin(), follow[lhs].end());
        }
    }
}

void Grammar::construct_parseTable() {
    for (const auto &prod: select) {
        const std::string &lhs = prod.first.first;
        const std::vector<std::string> &rhs = prod.first.second;
        const std::set<std::string> &select_set = prod.second;
        std::string parseTable_first;

        for (const auto &single_aim: rhs) {
            parseTable_first.append(single_aim);
            parseTable_first.append(" ");
        }
        parseTable_first.erase(parseTable_first.end() - 1);

        for (const auto &terminal: select_set) {
            if (terminal != "ε")
                parseTable[lhs][terminal] = parseTable_first;
        }
    }
}

void Grammar::write_sets_to_file(const std::string &filename) {
    std::ofstream file(filename);

    file << "FIRST sets:\n";
    for (const auto &nt: first) {
        file << "FIRST(" << nt.first << ") = {";
        for (const auto &sym: nt.second) {
            file << sym << " ";
        }
        file << "}\n";
    }

    file << "\nFOLLOW sets:\n";
    for (const auto &nt: follow) {
        file << "FOLLOW(" << nt.first << ") = {";
        for (const auto &sym: nt.second) {
            file << sym << " ";
        }
        file << "}\n";
    }

    file << "\nSELECT sets:\n";
    for (const auto &prod: select) {
        const std::string &lhs = prod.first.first;
        const std::vector<std::string> &rhs = prod.first.second;
        file << "SELECT(" << lhs << " -> ";
        for (const auto &sym: rhs) {
            file << sym << " ";
        }
        file << ") = {";
        for (const auto &sym: prod.second) {
            file << sym << " ";
        }
        file << "}\n";
    }
}