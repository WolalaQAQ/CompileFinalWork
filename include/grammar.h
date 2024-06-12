//
// Created by wolala on 24-6-12.
//

#ifndef COMPILEFINALWORK_INCLUDE_GRAMMAR_H
#define COMPILEFINALWORK_INCLUDE_GRAMMAR_H

#include <map>
#include <set>
#include <vector>
#include <string>
#include <utility>
#include <iostream>
#include <fstream>
#include <sstream>

class Grammar {
public:
  std::map<std::string, std::vector<std::vector<std::string>>> productions;
  std::set <std::string> non_terminals;
  std::set <std::string> terminals;
  std::map <std::string, std::set<std::string>> first;
  std::map <std::string, std::set<std::string>> follow;
  std::map <std::pair<std::string, std::vector <std::string>>, std::set<std::string>> select;
  std::map <std::string, std::map<std::string, std::string>> parseTable;

  explicit Grammar(const std::string &filename);

  void get_sets(const std::string& filename);

private:
  void read_grammar_from_file(const std::string &filename);

  void compute_first();

  void compute_follow();

  void compute_select();

  void construct_parseTable();

  void write_sets_to_file(const std::string &filename);

};

#endif //COMPILEFINALWORK_INCLUDE_GRAMMAR_H
