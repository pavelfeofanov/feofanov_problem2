#include "Parser.h"
#include "Solver.h"
#include <iostream>

int main() {
  int capacity;
  ItemList items = Parser::parseItems("input.txt", capacity);
  GeneticSolver solver(items, capacity);
  Result result = solver.solve();

  std::cout << "Total value: " << result.totalValue << std::endl;
  std::cout << "Remaining capacity: " << result.remainingCapacity << std::endl;
  std::cout << "Items count: ";
  for (int count : result.itemCounts) {
    std::cout << count << " ";
  }
  std::cout << std::endl;

  return 0;
}
