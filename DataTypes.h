#ifndef DATATYPES_H
#define DATATYPES_H

#ifdef USE_DOUBLE_PRECISION
#define scalar double
#else
#define scalar float
#endif

#define uint unsigned int

#define ZERO ((scalar)0)
#define ONE ((scalar)1)
#define TWO ((scalar)2)

#endif