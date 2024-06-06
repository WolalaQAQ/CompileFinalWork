#include "symbol_table.h"
#include <iostream>

int SymbolTable::getSize(SymbolType type) {
	switch (type) {
	case SymbolType::INTEGER:
		return 4;
	case SymbolType::FLOAT:
		return 4;
	case SymbolType::CHAR:
		return 1;
	case SymbolType::DOUBLE:
		return 8;
	case SymbolType::STRUCT:
		return 0; // Struct size will be calculated based on its members
	default:
		return 4; // Default size for unknown type
	}
}

std::string SymbolTable::getTypeName(SymbolType type) {
	switch (type) {
	case SymbolType::INTEGER:
		return "INTEGER";
	case SymbolType::FLOAT:
		return "FLOAT";
	case SymbolType::CHAR:
		return "CHAR";
	case SymbolType::DOUBLE:
		return "DOUBLE";
	case SymbolType::STRUCT:
		return "STRUCT";
	case SymbolType::FUNCTION:
		return "FUNCTION";
	case SymbolType::UNKNOWN:
	default:
		return "UNKNOWN";
	}
}

void SymbolTable::addSymbol(const std::string& name, SymbolType type) {
	if (table.find(name) == table.end()) {
		int size = getSize(type);
		table[name] = {type, currentAddress, size};
		currentAddress += size;
	}
}

void SymbolTable::addFunctionSymbol(const std::string& name) {
	table[name] = {SymbolType::FUNCTION, -1, 0}; // Functions don't occupy memory space
}

void SymbolTable::addStructSymbol(const std::string& name, const std::unordered_map<std::string, Symbol>& members) {
	int size = 0;
	for (const auto& member : members) {
		size += getSize(member.second.type);
	}
	structTable[name] = {SymbolType::STRUCT, currentAddress, size, members};
	currentAddress += size;
}

Symbol* SymbolTable::getSymbol(const std::string& name) {
	if (table.find(name) != table.end()) {
		return &table[name];
	}
	if (structTable.find(name) != structTable.end()) {
		return &structTable[name];
	}
	return nullptr;
}

StructSymbol* SymbolTable::getStructSymbol(const std::string& name) {
	if (structTable.find(name) != structTable.end()) {
		return &structTable[name];
	}
	return nullptr;
}

void SymbolTable::printTable() {
	std::cout << "Symbol Table:\n";
	for (const auto& entry : table) {
		std::cout << "Name: " << entry.first
				  << ", Type: " << getTypeName(entry.second.type)
				  << ", Address: " << entry.second.address
				  << ", Size: " << entry.second.size << "\n";
	}
	for (const auto& entry : structTable) {
		std::cout << "Struct: " << entry.first
				  << ", Type: " << getTypeName(entry.second.type)
				  << ", Address: " << entry.second.address
				  << ", Size: " << entry.second.size << "\n";
		for (const auto& member : entry.second.members) {
			std::cout << "  Member: " << member.first
					  << ", Type: " << getTypeName(member.second.type)
					  << ", Size: " << member.second.size << "\n";
		}
	}
}
