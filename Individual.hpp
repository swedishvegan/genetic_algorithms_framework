#ifndef INDIVIDUAL_HPP
#define INDIVIDUAL_HPP

#include "DataTypes.h"

class Individual {
public:

	virtual scalar fitness() = 0;

	virtual void mutate() = 0;

	// Please implement the multiplication
	// operator in your derived class.

};

#endif