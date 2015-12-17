////////////////////////////////////////////////////////////////////////////////
// HlTrueType

#include "HlTrueType.h"
#include "RsMaterial.h"
#include "RsTexture.h"
#include "BtMemory.h"
#include "BaUserData.h"
#include "HlDraw.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb.h"
#include "stb_truetype.h"

// https://github.com/nothings/stb

////////////////////////////////////////////////////////////////////////////////
// Setup

void HlTrueType::Setup( RsMaterial *pMaterial, BaUserData *pUserData )
{
	m_pRenderTarget = pMaterial;
	m_pUserData = pUserData;

	/* prepare font */
	if(!stbtt_InitFont(&m_stbFontInfo, pUserData->GetData(), 0))
	{
		printf("failed\n");
	}
}

////////////////////////////////////////////////////////////////////////////////
// Draw

MtVector2 HlTrueType::Draw( const BtChar *text )
{
	BtU32 textLength = (BtU32)strlen(text);

	m_width  = m_pRenderTarget->GetTexture(0)->GetWidth();
	m_height = m_pRenderTarget->GetTexture(0)->GetHeight();

	// Get the font metrics
	int ascent, descent, lineGap;
	stbtt_GetFontVMetrics(&m_stbFontInfo, &ascent, &descent, &lineGap);

    /* create a bitmap for the phrase */
    BtU8* bitmap = (BtU8*)BtMemory::Allocate( m_width * m_height * 4 );

	BtMemory::Set( bitmap, 0, m_width * m_height * 4 );

	// Calculate the width of the font

	MtVector2 v2UnscaledDimension(0, 0);
	int screenx = 0;
	int screeny = 0;

	// Get the size of the 
	for(BtU32 i = 0; i < textLength; ++i)
	{
		BtChar current = text[i];

		if(current == '\n')
		{
			screenx = 0;
			screeny += ascent - descent;
		}
		else
		{
			/* how wide is this character */
			int advance;
			stbtt_GetCodepointHMetrics(&m_stbFontInfo, current, &advance, 0);
			screenx += advance;

			/* get bounding box for character (may be offset to account for chars that dip above or below the line */
			int c_x1, c_y1, c_x2, c_y2;
			stbtt_GetCodepointBitmapBox(&m_stbFontInfo, current, 1, 1, &c_x1, &c_y1, &c_x2, &c_y2);

			/* compute y (different characters have different heights */
			int y = screeny + ( c_y2 - c_y1 );

			/* add kerning */
			int kern;

			if(text[i + 1])
			{
				BtChar next = text[i + 1];
				kern = stbtt_GetCodepointKernAdvance(&m_stbFontInfo, current, next);
				screenx += kern;
			}

			v2UnscaledDimension.x = MtMax(v2UnscaledDimension.x, (BtFloat)screenx );
			v2UnscaledDimension.y = MtMax(v2UnscaledDimension.y, (BtFloat)y );
		}
	}

	BtFloat biggestDimension = MtMax( v2UnscaledDimension.x, v2UnscaledDimension.y );
	BtFloat scale = m_width / biggestDimension;
	
	// Get the length of each text row
	screenx = 0;
	BtFloat rowStart[64];
	BtU32 row = 0;
	for(BtU32 i = 0; i < textLength; ++i)
	{
		BtChar current = text[i];

		if(current == '\n')
		{
			++row;
			screenx = 0;
		}
		else
		{
			int c_x1, c_y1, c_x2, c_y2;
			stbtt_GetCodepointBitmapBox(&m_stbFontInfo, current, scale, scale, &c_x1, &c_y1, &c_x2, &c_y2);

			int advance;
			stbtt_GetCodepointHMetrics(&m_stbFontInfo, current, &advance, 0);
			screenx += (int)(advance * scale);

			if(text[i + 1])
			{
				BtChar next = text[i + 1];
				BtFloat kern = (BtFloat) stbtt_GetCodepointKernAdvance(&m_stbFontInfo, current, next);
				screenx += (int)(kern * scale);
			}
			rowStart[row] = (512 - screenx) / 2;
		}
	}

	// Render the font into the bitmap
	MtVector2 v2Dimension(0, 0);
	ascent = (int)((float)ascent * scale);
	descent = (int)((float)descent * scale);
	row = 0;
	screenx = rowStart[0];
	screeny = 0;
	for( BtU32 i = 0; i < textLength; ++i )
    {
		BtChar current = text[i];

		if( current == '\n' )
		{
			row++;
			screenx = rowStart[row];
			screeny += ascent - descent;
		}
		else
		{
			/* get bounding box for character (may be offset to account for chars that dip above or below the line */
			int c_x1, c_y1, c_x2, c_y2;
			stbtt_GetCodepointBitmapBox(&m_stbFontInfo, current, scale, scale, &c_x1, &c_y1, &c_x2, &c_y2);
        
			/* compute y (different characters have different heights */
			int y = screeny + ascent + c_y1;
        
			/* render character (stride and offset is important here) */
			int byteOffset = screenx + (y  * m_width);
			stbtt_MakeCodepointBitmap(&m_stbFontInfo, bitmap + byteOffset, c_x2 - c_x1, c_y2 - c_y1, m_width, scale, scale, current );
        
			/* how wide is this character */
			int advance;
			stbtt_GetCodepointHMetrics(&m_stbFontInfo, current, &advance, 0);
			screenx += (int)( advance * scale );
        
			/* add kerning */
			int kern;
		
			if( text[i + 1] )
			{
				BtChar next = text[i + 1];
				kern = stbtt_GetCodepointKernAdvance(&m_stbFontInfo, current, next );
				screenx += (int)( kern * scale );
			}

			y = screeny + ascent - descent + c_y1;
			v2Dimension.x = MtMax( v2Dimension.x, (BtFloat)screenx );
			v2Dimension.y = MtMax( v2Dimension.y, (BtFloat)y );
		}
    }
    
    /* save out a 1 channel image */
    //stbi_write_png("out.png", b_w, b_h, 1, bitmap, b_w);
    
	// Copy to the image
	RsTexture *pDestinationTexture = m_pRenderTarget->GetTexture(0);
	BtU32* bitmap2 = (BtU32*)BtMemory::Allocate(512 * 512 * 4, 16);
	BtMemory::Set( bitmap2, 0, 512 * 512 * 4 );
	BtU32 index = 0;

	for( BtU32 j=0; j<m_height; j++ )
	{
		for( BtU32 i=0; i<m_width; i++ )
		{
			BtU8 bw = bitmap[i + (j * m_height)];

			if( bw > 0 )
			{
				RsColour colour = RsColour( 255, 255, 255, bw );
				bitmap2[index] = colour.asWord();
			}
			++index;
		}
	}

	// Draw a horizontal line to test the extents of our font
	BtBool isBorder = BtFalse;
	if( isBorder )
	{
		for(BtU32 i = 0; i < m_width; i++)
		{
			BtU32 j = (BtU32)v2Dimension.y;
			bitmap2[i + (j * m_height)] = 0xffffffff;
			bitmap2[i + (1 * m_height)] = 0xffffffff;
		}
		for(BtU32 j = 0; j < v2Dimension.y; j++)
		{
			BtU32 i = 0;
			bitmap2[i + (j * m_height)] = 0xffffffff;
			bitmap2[i + (j * m_height)] = 0xffffffff;
		}
	}
	pDestinationTexture->WriteMemory( (BtU8*)bitmap2, 512 * 512 * 4);

	BtMemory::Free( bitmap2 );
    BtMemory::Free( bitmap );

	return v2Dimension;
}

////////////////////////////////////////////////////////////////////////////////
// GetMaterial

RsMaterial *HlTrueType::GetMaterial()
{
	return m_pRenderTarget;
}
