#ifndef RANKBASEDSELECTOR_HPP
#define RANKBASEDSELECTOR_HPP

#include <vector>
#include "GeneticAlgorithmsFramework.hpp"

template <typename Individual>
class RankBasedSelector : public Selector<Individual> {
public:

	RankBasedSelector(scalar s);

	void update(Population<Individual>& population);

	uint select(scalar randVal);

protected:

	std::vector<uint> sortedPerm;
	std::vector<scalar> cumulativeProbabilities;
	scalar s;

};

template <typename Individual>
RankBasedSelector<Individual>::RankBasedSelector(scalar s) : Selector<Individual>(), s(s) { }

template <typename Individual>
void RankBasedSelector<Individual>::update(Population<Individual>& population) {

	if (sortedPerm.size() != population.getGenerationSize()) {

		sortedPerm.resize(population.getGenerationSize());
		cumulativeProbabilities.resize(population.getGenerationSize());

	}
	
	scalar maxFitness = population[0].fitness();
	scalar minFitness = maxFitness;
	uint maxIndividual = 0u;

	for (uint j = 1u; j < population.getGenerationSize(); j++) {

		if (population[j].fitness() >= maxFitness) {

			maxFitness = population[j].fitness();
			maxIndividual = j;

		}

		if (population[j].fitness() < minFitness) minFitness = population[j].fitness();

	}

	sortedPerm[0] = maxIndividual;
	scalar lastMaxFitness = maxFitness;

	for (uint i = 1u; i < population.getGenerationSize(); i++) {
		
		maxFitness = minFitness;

		for (uint j = 0u; j < population.getGenerationSize(); j++) if (population[j].fitness() >= maxFitness && population[j].fitness() <= lastMaxFitness) {

			bool shouldContinue = false;
			for (uint k = 0u; k < i; k++) if (j == sortedPerm[k]) { shouldContinue = true; break; }
			if (shouldContinue) continue;

			maxFitness = population[j].fitness();
			maxIndividual = j;

		}
		
		sortedPerm[i] = maxIndividual;
		lastMaxFitness = maxFitness;

	}
	
	scalar mu = (scalar)population.getGenerationSize();
	scalar mu_triangular = mu * (mu - ONE) / TWO;

	for (uint i = 0u; i < population.getGenerationSize(); i++) cumulativeProbabilities[i] = (TWO - s) / mu + (scalar)(population.getGenerationSize() - i - 1u) * (s - ONE) / mu_triangular;

	scalar cumProb = ZERO;
	for (uint i = 0u; i < population.getGenerationSize(); i++) {

		cumProb += cumulativeProbabilities[i];
		cumulativeProbabilities[i] = cumProb;

	}

}

template <typename Individual>
uint RankBasedSelector<Individual>::select(scalar randVal) {

	for (uint i = 0u; i < cumulativeProbabilities.size(); i++) if (randVal <= cumulativeProbabilities[i]) return sortedPerm[i];
	return sortedPerm[sortedPerm.size() - 1u];

}

#endif