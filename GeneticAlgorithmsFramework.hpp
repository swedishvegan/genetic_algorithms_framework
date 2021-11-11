#ifndef GENETICALGORITHMSFRAMEWORK_HPP
#define GENETICALGORITHMSFRAMEWORK_HPP

/*
	
	NOTE: Please define USE_DOUBLE_PRECISION before including this file in
	order to run the genetic algorithms with double precision.
	
	How to use this library:

		1) Implement the Individual class.

			* If your fitness function is expensive, design your class so that
			the value is stored and it is not recomputed every time the function 
			is called.

			* The multiplication operator is used to denote crossover between
			two parents. You must therefore implement this operator overload.

			* Note that the mutate function is called on every individual each
			generation, so please account for this in your design.

			* Write a default constructor that will be used for random initial-
			ization in the Population class.

			Note: you technically don't need to derive your class from Indiv-
			idual. The base class skeleton is just provided as a guide so that
			the user knows what methods need to be implemented.

		2) Implement the Selector class.

			* The update function should compute any metadata needed to imple-
			ment the select function.

			* The select function takes a random scalar in the range [0, 1] and
			outputs an index for an individual selected for mating.

		3) Create a Population object with your Individual class as the temp-
		late. It should be straightforward how to proceed from here.

*/

#include "DataTypes.h"
#include "Individual.hpp"
#include "Selector.hpp"
#include "Population.hpp"

#endif