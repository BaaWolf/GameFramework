////////////////////////////////////////////////////////////////////////////////
// HlTrueType.h

// Include guard
#pragma once

// Includes
#include "BaArchive.h"
#include "BtBase.h"
#include "BtArray.h"
#include "BtTypes.h"
#include "MtVector2.h"
#include "stb.h"
#include "stb_truetype.h"

class BaUserData;

// Class definition
class HlTrueType
{
public:
	
	// Public functions		
	void						 Setup( RsMaterial *pMaterial, BaUserData *pUserData  );
	MtVector2					 Draw( const BtChar *text );

	// Accessors
	RsMaterial					*GetMaterial();

protected:

private:
	RsMaterial				    *m_pRenderTarget;
	BtU32						 m_width;
	BtU32						 m_height;
	BaUserData					*m_pUserData;
	stbtt_fontinfo				 m_stbFontInfo;
};
