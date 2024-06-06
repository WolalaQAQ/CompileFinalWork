//
// Created by wolala on 24-6-5.
//

#ifndef COMPILEFINALWORK_INCLUDE_SYMBOL_TABLE_H
#define COMPILEFINALWORK_INCLUDE_SYMBOL_TABLE_H

#include <string>
#include <unordered_map>

enum class SymbolType {
  INTEGER,
  FLOAT,
  CHAR,
  DOUBLE,
  STRUCT,
  FUNCTION,
  UNKNOWN
};

struct Symbol {
  SymbolType type;
  int address;
  int size;
};

struct StructSymbol : Symbol {
  std::unordered_map<std::string, Symbol> members;
};

class SymbolTable {
private:
  std::unordered_map<std::string, Symbol> table;
  std::unordered_map<std::string, StructSymbol> structTable;
  int currentAddress = 0;

public:
  int getSize(SymbolType type);
  std::string getTypeName(SymbolType type);
  void addSymbol(const std::string& name, SymbolType type);
  void addFunctionSymbol(const std::string& name);
  void addStructSymbol(const std::string& name, const std::unordered_map<std::string, Symbol>& members);
  Symbol* getSymbol(const std::string& name);
  StructSymbol* getStructSymbol(const std::string& name);
  void printTable();
};


#endif //COMPILEFINALWORK_INCLUDE_SYMBOL_TABLE_H
