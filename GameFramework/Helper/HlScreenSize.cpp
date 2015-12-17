////////////////////////////////////////////////////////////////////////////////
// HlScreenSize

// Includes
#include "HlScreenSize.h"
#include "BtTime.h"
#include "BtString.h"
#include "RsUtil.h"

// statics

////////////////////////////////////////////////////////////////////////////////
// Refactor

MtVector2 HlScreenSize::Refactor( const MtVector2 &v2OriginalScreenSize,
                                  const MtVector2 &v2Coord )
{
	BtFloat x = (BtFloat)RsUtil::GetWidth()  / v2OriginalScreenSize.x;
	BtFloat y = (BtFloat)RsUtil::GetHeight() / v2OriginalScreenSize.y;

	MtVector2 v2NewDimension = MtVector2(v2Coord.x * x, v2Coord.y * y);

	return v2NewDimension;
}

////////////////////////////////////////////////////////////////////////////////
// Refactor

MtVector2 HlScreenSize::Refactor( const MtVector2 &v2OriginalScreenSize,
                                  const MtVector2 &v2Coord,
                                  const MtVector2 &v2ScreenDimension )
{
	BtFloat x = (BtFloat)v2ScreenDimension.x / v2OriginalScreenSize.x;
	BtFloat y = (BtFloat)v2ScreenDimension.y / v2OriginalScreenSize.y;

	MtVector2 v2NewDimension = MtVector2( v2Coord.x * x, v2Coord.y * y);

	return v2NewDimension;
}
