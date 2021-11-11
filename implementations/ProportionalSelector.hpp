#ifndef PROPORTIONALSELECTOR_HPP
#define PROPORTIONALSELECTOR_HPP

#include <vector>
#include "GeneticAlgorithmsFramework.hpp"

template <typename Individual>
class ProportionalSelector : public Selector<Individual> {
public:

	void update(Population<Individual>& population);

	uint select(scalar randVal);

protected:
	
	std::vector<scalar> cumulativeProbabilities;

};

template <typename Individual>
void ProportionalSelector<Individual>::update(Population<Individual>& population) {

	if (cumulativeProbabilities.size() != population.getGenerationSize()) cumulativeProbabilities.resize(population.getGenerationSize());

	scalar minFitness = population[0].fitness();
	scalar maxFitness = minFitness;

	for (uint i = 1u; i < population.getGenerationSize(); i++) {

		if (population[i].fitness() < minFitness) minFitness = population[i].fitness();
		if (population[i].fitness() > maxFitness) maxFitness = population[i].fitness();

	}

	scalar totalFitness = ZERO;
	scalar fitnessRange = maxFitness - minFitness;
	if (fitnessRange < 0.001) fitnessRange = 0.001;

	for (uint i = 0u; i < population.getGenerationSize(); i++) { 
		
		cumulativeProbabilities[i] = (population[i].fitness() - minFitness) / fitnessRange; 
		totalFitness += cumulativeProbabilities[i];
	
	}

	scalar cumulativeFitness = ZERO;
	for (uint i = 0u; i < population.getGenerationSize(); i++) {

		cumulativeFitness += cumulativeProbabilities[i];
		cumulativeProbabilities[i] = cumulativeFitness / totalFitness;

	}

}

template <typename Individual>
uint ProportionalSelector<Individual>::select(scalar randVal) {

	for (uint i = 0u; i < cumulativeProbabilities.size(); i++) if (randVal <= cumulativeProbabilities[i]) return i;
	return cumulativeProbabilities.size() - 1u;

}

#endif