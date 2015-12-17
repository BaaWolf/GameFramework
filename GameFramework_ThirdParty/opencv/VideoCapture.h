////////////////////////////////////////////////////////////////////////////////
// VideoCapture.h

// Include guard
#pragma once
#include "BtTypes.h"

// Class definition
class CVVideoCapture
{
public:

	// Public functions
	static BtU32						   Start();
	static void							   Stop();
	static void							   Update();

	// Accessors
	static BtU8						      *GetMemory();
    static void                            SetMemory( BtU8 *pMemory, BtU32 numBytes );

private:

	// Private functions

	// Private members
};
