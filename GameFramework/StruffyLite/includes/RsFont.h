////////////////////////////////////////////////////////////////////////////////
/// RsFont.h

#pragma once
#include "BaResource.h"
#include "MtVector2.h"
#include "RsColour.h"

// Class Declaration
class RsFont : public BaResource
{
	DECLARE_RESOURCETYPE( BaRT_Font );

public:

	virtual MtVector2 GetDimension( const BtChar* szText ) = 0;
	
	virtual void SetNewLineRatio( BtFloat ratio ) = 0;

	virtual MtVector2 Render( const MtVector2& v2Position,
						 	  const RsColour &colour,
							  const BtChar* szText,
							  BtU32 sortOrder ) = 0;

	virtual MtVector2 Render( const MtVector2& v2Position,
							  const MtVector2& v2Scale,
							  const RsColour &colour,
							  const BtChar* szText,
							  BtU32 sortOrder ) = 0;
};
