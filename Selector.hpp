#ifndef SELECTOR_HPP
#define SELECTOR_HPP

#include "DataTypes.h"

template <typename Individual>
class Population;

template <typename Individual>
class Selector {
public:

	Selector() { }

	virtual void update(Population<Individual>& population) = 0;

	virtual uint select(scalar randVal) = 0;

	Selector(const Selector<Individual>&) = delete;
	void operator = (const Selector<Individual>&) = delete;

};

#endif