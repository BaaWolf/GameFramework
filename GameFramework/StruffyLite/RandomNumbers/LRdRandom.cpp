////////////////////////////////////////////////////////////////////////////////
// RdRandom.cpp

// Includes
#include <stdlib.h>
#include <time.h>
#include "RdRandom.h"
#include "MtMath.h"
#include "BtTime.h"

////////////////////////////////////////////////////////////////////////////////
// SetRandomSeed

void RdRandom::SetRandomSeed()
{
	srand( (unsigned int)time(NULL) );
}

////////////////////////////////////////////////////////////////////////////////
// SetSeed

void RdRandom::SetSeed(BtS32 seed)
{
	srand(seed);
}

////////////////////////////////////////////////////////////////////////////////
// GetNumber

BtS32 RdRandom::GetNumber(BtS32 lower, BtS32 upper)
{
	BtS32 range = upper - lower;
	BtS32 randomValue = lower + ( rand() % range );
	return randomValue;
}

////////////////////////////////////////////////////////////////////////////////
// GetFloat

BtFloat RdRandom::GetFloat(BtFloat lower, BtFloat upper)
{
	BtFloat ratio = (float)rand() / RAND_MAX;
	BtFloat randomValue = ((upper - lower) * ratio) + lower;
	return randomValue;
}
