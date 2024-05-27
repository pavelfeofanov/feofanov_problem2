#include <fstream>
#include <iostream>
#include "Parser.h"

ItemList Parser::parseItems(const char* filename, int& capacity) {
    std::ifstream file(filename);
    ItemList items;
    if (!file) {
        std::cerr << "Cannot open input file.\n";
        return items;
    }

    int numItems;
    file >> numItems >> capacity;

    int value, weight;
    while (file >> value >> weight) {
        items.emplace_back(value, weight);
    }

    return items;
}
