#include <vector>
#include <random>
#include <algorithm>
#include <numeric>
#include <limits>
#include "Parser.h"

struct Individual {
    std::vector<int> genes;  // Amount of every item 
    int fitness;             // Total value
};

struct Result {
    int totalValue;
    int remainingCapacity;
    std::vector<int> itemCounts;
};

class GeneticSolver {
public:
    GeneticSolver(const ItemList& items, int capacity, int populationSize = 50, int generations = 500, double mutationRate = 0.01);
    Result solve();

private:
    ItemList items;
    int capacity;
    int populationSize;
    int generations;
    double mutationRate;
    std::vector<Individual> population;

    void initializePopulation();
    void evaluateFitness(Individual& individual);
    void selection();
    void crossover(Individual &parent1, Individual &parent2, Individual &child);
    void mutate(Individual &individual);
    int calculateTotalWeight(const Individual& individual);
    int calculateTotalValue(const Individual& individual);
};
