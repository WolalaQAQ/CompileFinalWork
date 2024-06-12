#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

class Grammar {
public:
    map<string, vector<vector<string>>> productions;
    set<string> non_terminals;
    set<string> terminals;
    map<string, set<string>> first;
    map<string, set<string>> follow;
    map<pair<string, vector<string>>, set<string>> select;
    map<pair<string, string>, vector<string>> parsing_table;

    Grammar(const map<string, vector<vector<string>>>& productions) {
        this->productions = productions;

        // Initialize non_terminals and terminals
        for (const auto& prod : productions) {
            non_terminals.insert(prod.first);
            for (const auto& rhs : prod.second) {
                for (const auto& symbol : rhs) {
                    if (productions.find(symbol) == productions.end()) {
                        terminals.insert(symbol);
                    }
                }
            }
        }

        // Initialize FIRST and FOLLOW sets
        for (const auto& nt : non_terminals) {
            first[nt] = set<string>();
            follow[nt] = set<string>();
        }
        follow[*non_terminals.begin()].insert("$");  // Add $ to the start symbol FOLLOW set
    }

    void compute_first() {
        bool changed = true;
        while (changed) {
            changed = false;
            for (const auto& prod : productions) {
                const string& lhs = prod.first;
                for (const auto& rhs : prod.second) {
                    size_t i = 0;
                    while (i < rhs.size()) {
                        const string& symbol = rhs[i];
                        if (terminals.find(symbol) != terminals.end()) {
                            if (first[lhs].insert(symbol).second) {
                                changed = true;
                            }
                            break;
                        }
                        else {
                            size_t before = first[lhs].size();
                            first[lhs].insert(first[symbol].begin(), first[symbol].end());
                            first[lhs].erase("¦Å");
                            size_t after = first[lhs].size();
                            if (first[symbol].find("¦Å") != first[symbol].end()) {
                                i++;
                            }
                            else {
                                break;
                            }
                            if (before != after) {
                                changed = true;
                            }
                        }
                    }
                    if (i == rhs.size()) {
                        if (first[lhs].insert("¦Å").second) {
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
            for (const auto& prod : productions) {
                const string& lhs = prod.first;
                for (const auto& rhs : prod.second) {
                    set<string> follow_temp = follow[lhs];
                    for (auto it = rhs.rbegin(); it != rhs.rend(); ++it) {
                        const string& symbol = *it;
                        if (non_terminals.find(symbol) != non_terminals.end()) {
                            size_t before = follow[symbol].size();
                            follow[symbol].insert(follow_temp.begin(), follow_temp.end());
                            size_t after = follow[symbol].size();
                            if (first[symbol].find("¦Å") != first[symbol].end()) {
                                follow_temp.insert(first[symbol].begin(), first[symbol].end());
                                follow_temp.erase("¦Å");
                            }
                            else {
                                follow_temp = first[symbol];
                            }
                            if (before != after) {
                                changed = true;
                            }
                        }
                        else {
                            follow_temp = { symbol };
                        }
                    }
                }
            }
        }
    }

    void compute_select() {
        for (const auto& prod : productions) {
            const string& lhs = prod.first;
            for (const auto& rhs : prod.second) {
                set<string> first_rhs;
                size_t i = 0;
                while (i < rhs.size()) {
                    const string& symbol = rhs[i];
                    if (terminals.find(symbol) != terminals.end()) {
                        first_rhs.insert(symbol);
                        break;
                    }
                    else {
                        first_rhs.insert(first[symbol].begin(), first[symbol].end());
                        first_rhs.erase("¦Å");
                        if (first[symbol].find("¦Å") != first[symbol].end()) {
                            i++;
                        }
                        else {
                            break;
                        }
                    }
                }
                if (i == rhs.size()) {
                    first_rhs.insert("¦Å");
                }
                select[{lhs, rhs}].insert(first_rhs.begin(), first_rhs.end());
                if (first_rhs.find("¦Å") != first_rhs.end()) {
                    select[{lhs, rhs}].insert(follow[lhs].begin(), follow[lhs].end());
                }
            }
        }
    }

    void construct_parsing_table() {
        for (const auto& prod : select) {
            const string& lhs = prod.first.first;
            const vector<string>& rhs = prod.first.second;
            const set<string>& select_set = prod.second;

            for (const auto& terminal : select_set) {
                parsing_table[{lhs, terminal}] = rhs;
            }
        }
    }

    void write_sets_to_file(const string& filename) {
        ofstream file(filename);

        file << "FIRST sets:\n";
        for (const auto& nt : first) {
            file << "FIRST(" << nt.first << ") = {";
            for (const auto& sym : nt.second) {
                file << sym << " ";
            }
            file << "}\n";
        }

        file << "\nFOLLOW sets:\n";
        for (const auto& nt : follow) {
            file << "FOLLOW(" << nt.first << ") = {";
            for (const auto& sym : nt.second) {
                file << sym << " ";
            }
            file << "}\n";
        }

        file << "\nSELECT sets:\n";
        for (const auto& prod : select) {
            const string& lhs = prod.first.first;
            const vector<string>& rhs = prod.first.second;
            file << "SELECT(" << lhs << " -> ";
            for (const auto& sym : rhs) {
                file << sym << " ";
            }
            file << ") = {";
            for (const auto& sym : prod.second) {
                file << sym << " ";
            }
            file << "}\n";
        }
    }

    void write_parsing_table_to_file(const string& filename) {
        ofstream file(filename);

        file << "Parsing Table:\n";
        for (const auto& entry : parsing_table) {
            const string& non_terminal = entry.first.first;
            const string& terminal = entry.first.second;
            const vector<string>& production = entry.second;

            file << "M[" << non_terminal << ", " << terminal << "] = ";
            for (const auto& sym : production) {
                file << sym << " ";
            }
            file << "\n";
        }
    }
};

map<string, vector<vector<string>>> read_grammar_from_file(const string& filename) {
    map<string, vector<vector<string>>> productions;
    ifstream file(filename);
    string line;

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
            if (symbol == "|") {
                productions[lhs].push_back(rhs_symbols);
                rhs_symbols.clear();
            }
            else {
                rhs_symbols.push_back(symbol);
            }
        }
        productions[lhs].push_back(rhs_symbols);
    }

    return productions;
}

int main() {
    string input_filename = "test.txt";
    //string input_filename = "grammar.txt";
    string output_sets_filename = "output_sets.txt";
    string output_table_filename = "parsing_table.txt";

    map<string, vector<vector<string>>> productions = read_grammar_from_file(input_filename);
    Grammar grammar(productions);
    grammar.compute_first();
    grammar.compute_follow();
    grammar.compute_select();
    grammar.construct_parsing_table();
    grammar.write_sets_to_file(output_sets_filename);
    grammar.write_parsing_table_to_file(output_table_filename);

    return 0;
}
