////////////////////////////////////////////////////////////////////////////////
// HlScreenSize.h

// Include guard
#pragma once

// Includes
#include "BtBase.h"
#include "BtArray.h"
#include "BtTypes.h"
#include "MtVector2.h"

// Class definition
class HlScreenSize
{
public:

	
	// Public functions
	static MtVector2 Refactor( const MtVector2 &v2OriginalDimension, const MtVector2 &v2Dimension );
	static MtVector2 Refactor( const MtVector2 &v2OriginalDimension, const MtVector2 &v2Dimension, const MtVector2 &v2ScreenDimension );
};
