#ifndef POPULATION_HPP
#define POPULATION_HPP

#include <vector>
#include <cstdlib>
#include <time.h>
#include "DataTypes.h"
#include "Selector.hpp"

template <typename Individual>
class Population {
public:

	Population(uint numIndividuals, uint generation = 0u);
	
	Population(std::vector<Individual>& individuals, uint generation = 0u);

	void simulateGeneration(Selector<Individual>& selector, uint numIndividuals = 0u);

	Individual& operator [] (uint index);

	uint getGenerationSize() const;

	uint getGeneration() const;

	scalar getMaxFitness() const;

	scalar getAvgFitness() const;

	Individual& getBestIndividual();

	Population(const Population<Individual>&) = delete;
	void operator = (const Population<Individual>&) = delete;

protected:

	std::vector<Individual> individualsVec;
	uint curGen;

	scalar maxFitness = ZERO;
	scalar avgFitness = ZERO;
	uint bestIndividual = 0u;

	void repopulate(Selector<Individual>& selector, uint numIndividuals);

	void updateStats();

};

bool _GA_needsRandInit = true;

template <typename Individual>
Population<Individual>::Population(uint numIndividuals, uint generation) {

	if (_GA_needsRandInit) { srand(time(NULL)); _GA_needsRandInit = false; }

	if (numIndividuals == 0u) numIndividuals = 1u;
	individualsVec.resize(numIndividuals);
	curGen = generation;
	updateStats();

}

template <typename Individual>
Population<Individual>::Population(std::vector<Individual>& individuals, uint generation) {

	if (_GA_needsRandInit) { srand(time(NULL)); _GA_needsRandInit = false; }

	individualsVec = individuals;
	if (individualsVec.size() == 0u) individualsVec.resize(1u);
	curGen = generation;
	updateStats();

}

template <typename Individual>
void Population<Individual>::simulateGeneration(Selector<Individual>& selector, uint numIndividuals) {
	
	selector.update(*this);
	repopulate(selector, numIndividuals);
	updateStats();
	curGen++;

}

template <typename Individual>
Individual& Population<Individual>::operator [] (uint index) { return individualsVec[index % individualsVec.size()]; }

template <typename Individual>
uint Population<Individual>::getGenerationSize() const { return individualsVec.size(); }

template <typename Individual>
uint Population<Individual>::getGeneration() const { return curGen; }

template <typename Individual>
scalar Population<Individual>::getMaxFitness() const { return maxFitness; }

template <typename Individual>
scalar Population<Individual>::getAvgFitness() const { return avgFitness; }

template <typename Individual>
Individual& Population<Individual>::getBestIndividual() { return individualsVec[bestIndividual]; }

template <typename Individual>
void Population<Individual>::repopulate(Selector<Individual>& selector, uint numIndividuals) {

	if (numIndividuals == 0u) numIndividuals = getGenerationSize();
	std::vector<Individual> newIndividuals(numIndividuals);

	newIndividuals[0] = getBestIndividual();

	for (uint i = 1u; i < numIndividuals; i++) {

		scalar r1 = (scalar)rand() / (scalar)RAND_MAX;
		scalar r2 = (scalar)rand() / (scalar)RAND_MAX;

		uint parent1_idx = selector.select(r1);
		uint parent2_idx = selector.select(r2);

		newIndividuals[i] = individualsVec[parent1_idx] * individualsVec[parent2_idx];

	}

	individualsVec = newIndividuals;
	for (uint i = 1u; i < getGenerationSize(); i++) individualsVec[i].mutate();

}

template <typename Individual>
void Population<Individual>::updateStats() {

	maxFitness = individualsVec[0].fitness();
	avgFitness = ZERO;
	bestIndividual = 0u;

	for (uint i = 0u; i < getGenerationSize(); i++) {

		if (individualsVec[i].fitness() > maxFitness) {

			maxFitness = individualsVec[i].fitness();
			bestIndividual = i;

		}

		avgFitness += individualsVec[i].fitness();

	}

	avgFitness /= (scalar)getGenerationSize();

}

#endif