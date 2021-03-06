////////////////////////////////////////////////////////////////////////////////
/// RsFontWin32GL.h

#ifndef __RsFontWin32GL_h__
#define __RsFontWin32GL_h__

#include "RsFont.h"
#include "RsTextureWinGL.h"

struct LBaFontFileData;
class BaArchive;
class RsFont;

class RsMaterial;

// Class Declaration
class RsFontWin32GL : public RsFont
{
public:

	void				FixPointers( BtU8 *pFileData, BaArchive *pArchive );

	MtVector2			GetDimension( const BtChar* szText );

	void				SetNewLineRatio( BtFloat ratio );

	MtVector2			Render( const MtVector2& v2Position,
								const RsColour &colour,
								const BtChar* szText,
								BtU32 sortOrder );

	MtVector2			Render( const MtVector2& v2Position,
								const MtVector2& v2Scale,
								const RsColour &colour,
								const BtChar* szText,
								BtU32 sortOrder );

	void				Render( RsFontRenderable *pFontRenderable );

private:

	friend class BaArchiveManager;
	friend class RcRenderPipeline;

	LBaFontFileData*	m_pFileData;
	RsTextureWinGL*		m_pTextures[MaxFontTexturePages];
	RsMaterial*			m_pMaterial;
	BtFloat				m_ratio;
};

#endif // __RsFontWin32GL_h__
