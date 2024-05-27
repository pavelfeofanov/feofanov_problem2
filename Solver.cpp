#include "Solver.h"
#include <iostream>

GeneticSolver::GeneticSolver(const ItemList &items, int capacity,
                             int populationSize, int generations,
                             double mutationRate)
    : items(items), capacity(capacity), populationSize(populationSize),
      generations(generations), mutationRate(mutationRate) {}

void GeneticSolver::initializePopulation() {
  std::random_device rd;
  std::mt19937 gen(rd());
  for (int i = 0; i < populationSize; ++i) {
    Individual individual;
    individual.genes.reserve(items.size());
    int currentWeight = 0;
    for (const auto &item : items) {
      std::uniform_int_distribution<> dis(0, (capacity - currentWeight) / item.second);
      int itemCount = dis(gen);
      individual.genes.push_back(itemCount);
      currentWeight += itemCount * item.second;
      if (currentWeight >= capacity) break;
    }
    population.push_back(individual);
  }
}


void GeneticSolver::evaluateFitness(Individual &individual) {
  int totalValue = 0;
  int totalWeight = 0;

  for (size_t i = 0; i < items.size(); ++i) {
    totalValue += individual.genes[i] * items[i].first;
    totalWeight += individual.genes[i] * items[i].second;
  }

  if (totalWeight <= capacity) {
    individual.fitness = totalValue;
  } else {
    individual.fitness = 0;
  }
}

void GeneticSolver::selection() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, populationSize - 1);
  std::vector<Individual> newPopulation;
  newPopulation.reserve(populationSize);

  for (int i = 0; i < populationSize; ++i) {
    int idx1 = dis(gen);
    int idx2 = dis(gen);
    newPopulation.push_back(population[idx1].fitness > population[idx2].fitness
                                ? population[idx1]
                                : population[idx2]);
  }

  population = std::move(newPopulation);
}

void GeneticSolver::crossover(Individual &parent1, Individual &parent2,
                              Individual &child) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, 1);

  child.genes.resize(items.size());
  for (size_t i = 0; i < items.size(); ++i) {
    child.genes[i] = dis(gen) ? parent1.genes[i] : parent2.genes[i];
  }
}

void GeneticSolver::mutate(Individual &individual) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(0, 1);
  std::uniform_int_distribution<> dis_gene(0, capacity / items[0].second);

  for (size_t i = 0; i < individual.genes.size(); ++i) {
    if (dis(gen) < mutationRate) {
      individual.genes[i] = dis_gene(gen) % (capacity / items[i].second + 1);
    }
  }
}

Result GeneticSolver::solve() {
  initializePopulation();
  for (int gen = 0; gen < generations; ++gen) {
    for (auto &individual : population) {
      evaluateFitness(individual);
    }
    selection();
    std::vector<Individual> newGeneration;
    newGeneration.reserve(populationSize);
    for (int i = 0; i < populationSize; i += 2) {
      Individual child1, child2;
      crossover(population[i], population[i + 1], child1);
      crossover(population[i + 1], population[i], child2);
      mutate(child1);
      mutate(child2);
      newGeneration.push_back(child1);
      newGeneration.push_back(child2);
    }
    population = std::move(newGeneration);
  }
  
  auto bestIt = std::max_element(population.begin(), population.end(),
                                 [](const Individual &a, const Individual &b) {
                                   return a.fitness < b.fitness;
                                 });
  Result result;
  result.itemCounts.resize(items.size());
  for (size_t i = 0; i < items.size(); ++i) {
    result.itemCounts[i] = bestIt->genes[i];
  }

  int totalWeight = calculateTotalWeight(*bestIt);
  result.remainingCapacity = capacity - totalWeight;

  result.totalValue = calculateTotalValue(*bestIt);
  return result;
}

int GeneticSolver::calculateTotalWeight(const Individual &individual) {
  int totalWeight = 0;
  for (size_t i = 0; i < items.size(); ++i) {
    totalWeight += individual.genes[i] * items[i].second;
  }
  return totalWeight;
}

int GeneticSolver::calculateTotalValue(const Individual &individual) {
  int totalValue = 0;
  for (size_t i = 0; i < items.size(); ++i){
    totalValue += individual.genes[i] * items[i].first;
  }
  return totalValue;
}
