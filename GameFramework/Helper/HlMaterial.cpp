////////////////////////////////////////////////////////////////////////////////
// HlMaterial.cpp

#include "HlMaterial.h"
#include "RsVertex.h"
#include "RsUtil.h"
#include "RsMaterial.h"


////////////////////////////////////////////////////////////////////////////////
// RenderFullscreen

void HlMaterial::RenderFullscreen( RsMaterial *pMaterial, BtU32 sortorder )
{
    pMaterial->Render( MtVector2( 0, 0 ), RsRenderTarget::GetCurrent()->GetCameraDimension(), RsColour::WhiteColour(), sortorder );
}

////////////////////////////////////////////////////////////////////////////////
// RenderUpsideDown

void HlMaterial::RenderUpsideDownXFlip( RsMaterial *pMaterial,
                                        const MtVector2 &v2Position,
                                        const MtVector2 &v2Dimension,
                                        const MtVector2 &v2ScreenDimension,
                                        BtU32 sortOrder
                                  )
{
    RsVertex3 vertex[4];
    RsVertex3 *pVertex = &vertex[0];
    
    // Calculate the positions
    BtFloat xmax = v2Dimension.x;
    BtFloat ymax = v2Dimension.y;
    
    RsVertex3 *pQuad = pVertex;
    
    // Copy these into vertex
    pVertex->m_v3Position = MtVector3(v2Position.x, v2Position.y, 0.1f);
    pVertex->m_v2UV = MtVector2(1, 0);
    pVertex->m_colour = 0xFFFFFFFF;
    ++pVertex;
    
    pVertex->m_v3Position = MtVector3(v2Position.x, v2Position.y + ymax, 0.1f);
    pVertex->m_v2UV = MtVector2(1, 1);
    pVertex->m_colour = 0xFFFFFFFF;
    ++pVertex;
    
    pVertex->m_v3Position = MtVector3(v2Position.x + xmax, v2Position.y + 0, 0.1f);
    pVertex->m_v2UV = MtVector2(0, 0);
    pVertex->m_colour = 0xFFFFFFFF;
    ++pVertex;
    
    pVertex->m_v3Position = MtVector3(v2Position.x + xmax, v2Position.y + ymax, 0.1f);
    pVertex->m_v2UV = MtVector2(0, 1);
    pVertex->m_colour = 0xFFFFFFFF;
    ++pVertex;
    
    // Flip the V's
    pQuad[0].m_v2UV.y = 1.0f - pQuad[0].m_v2UV.y;
    pQuad[1].m_v2UV.y = 1.0f - pQuad[1].m_v2UV.y;
    pQuad[2].m_v2UV.y = 1.0f - pQuad[2].m_v2UV.y;
    pQuad[3].m_v2UV.y = 1.0f - pQuad[3].m_v2UV.y;
    
    // Cache the display width and height
    MtVector2 v2TargetDimension = RsRenderTarget::GetCurrent()->GetCameraDimension();
    BtFloat fScaleWidth = 1.0f / v2TargetDimension.x;
    BtFloat fScaleHeight = 1.0f / v2TargetDimension.y;
    
    // Scale the position to local screen space -1 to 1
    for(BtU32 i = 0; i < 4; i++)
    {
        // Flip the y
        pQuad[i].m_v3Position.y = v2TargetDimension.y - pQuad[i].m_v3Position.y;
        
        // Scale from 0..width to 0..1
        pQuad[i].m_v3Position.x *= fScaleWidth;
        pQuad[i].m_v3Position.y *= fScaleHeight;
        
        // Scale from 0..1 to 0..2
        pQuad[i].m_v3Position.x *= 2.0f;
        pQuad[i].m_v3Position.y *= 2.0f;
        
        // Translate from 0..2 to -1..1
        pQuad[i].m_v3Position.x -= 1.0f;
        pQuad[i].m_v3Position.y -= 1.0f;
    }
    
    pMaterial->Render(RsPT_TriangleStrip, pQuad, 4, sortOrder );
}

////////////////////////////////////////////////////////////////////////////////
// RenderUpsideDown

void HlMaterial::RenderUpsideDown( RsMaterial *pMaterial,
								   const MtVector2 &v2Position,
								   const MtVector2 &v2Dimension,
								   const MtVector2 &v2ScreenDimension,
                                   BtU32 sortOrder
								  )
{
	RsVertex3 vertex[4];
	RsVertex3 *pVertex = &vertex[0];

	// Calculate the positions
	BtFloat xmax = v2Dimension.x;
	BtFloat ymax = v2Dimension.y;

	RsVertex3 *pQuad = pVertex;

	// Copy these into vertex
	pVertex->m_v3Position = MtVector3(v2Position.x, v2Position.y, 0.1f);
	pVertex->m_v2UV = MtVector2(0, 0);
	pVertex->m_colour = 0xFFFFFFFF;
	++pVertex;

	pVertex->m_v3Position = MtVector3(v2Position.x, v2Position.y + ymax, 0.1f);
	pVertex->m_v2UV = MtVector2(0, 1);
	pVertex->m_colour = 0xFFFFFFFF;
	++pVertex;

	pVertex->m_v3Position = MtVector3(v2Position.x + xmax, v2Position.y + 0, 0.1f);
	pVertex->m_v2UV = MtVector2(1, 0);
	pVertex->m_colour = 0xFFFFFFFF;
	++pVertex;

	pVertex->m_v3Position = MtVector3(v2Position.x + xmax, v2Position.y + ymax, 0.1f);
	pVertex->m_v2UV = MtVector2(1, 1);
	pVertex->m_colour = 0xFFFFFFFF;
	++pVertex;

	// Flip the V's
	pQuad[0].m_v2UV.y = 1.0f - pQuad[0].m_v2UV.y;
	pQuad[1].m_v2UV.y = 1.0f - pQuad[1].m_v2UV.y;
	pQuad[2].m_v2UV.y = 1.0f - pQuad[2].m_v2UV.y;
	pQuad[3].m_v2UV.y = 1.0f - pQuad[3].m_v2UV.y;

	// Cache the display width and height
	MtVector2 v2TargetDimension = RsRenderTarget::GetCurrent()->GetCameraDimension();
	BtFloat fScaleWidth = 1.0f / v2TargetDimension.x;
	BtFloat fScaleHeight = 1.0f / v2TargetDimension.y;

	// Scale the position to local screen space -1 to 1
	for(BtU32 i = 0; i < 4; i++)
	{
		// Flip the y
		pQuad[i].m_v3Position.y = v2TargetDimension.y - pQuad[i].m_v3Position.y;

		// Scale from 0..width to 0..1
		pQuad[i].m_v3Position.x *= fScaleWidth;
		pQuad[i].m_v3Position.y *= fScaleHeight;

		// Scale from 0..1 to 0..2
		pQuad[i].m_v3Position.x *= 2.0f;
		pQuad[i].m_v3Position.y *= 2.0f;

		// Translate from 0..2 to -1..1
		pQuad[i].m_v3Position.x -= 1.0f;
		pQuad[i].m_v3Position.y -= 1.0f;
	}

	pMaterial->Render(RsPT_TriangleStrip, pQuad, 4, sortOrder );
}

////////////////////////////////////////////////////////////////////////////////
// RenderSideWays

void HlMaterial::RenderSideWays( RsMaterial *pMaterial,
                                 const MtVector2 &v2Position,
                                 const MtVector2 &v2Dimension,
                                 const MtVector2 &v2ScreenDimension
                                )
{
    // Allocate vertex
    RsVertex3 vertex[4];
    RsVertex3 *pVertex = &vertex[0];
    
    // Calculate the positions
    BtFloat xmax = v2Dimension.x;
    BtFloat ymax = v2Dimension.y;
    
    RsVertex3 *pQuad = pVertex;
    
    // Copy these into vertex
    pVertex->m_v3Position = MtVector3( v2Position.x, v2Position.y, 0.1f );
    pVertex->m_v2UV = MtVector2( 0, 0 );
    pVertex->m_colour = 0xFFFFFFFF;
    ++pVertex;
    
    pVertex->m_v3Position = MtVector3( v2Position.x, v2Position.y + ymax, 0.1f );
    pVertex->m_v2UV = MtVector2( 1, 0 );
    pVertex->m_colour = 0xFFFFFFFF;
    ++pVertex;
    
    pVertex->m_v3Position = MtVector3( v2Position.x + xmax, v2Position.y + 0, 0.1f );
    pVertex->m_v2UV = MtVector2( 0, 1 );
    pVertex->m_colour = 0xFFFFFFFF;
    ++pVertex;
    
    pVertex->m_v3Position = MtVector3( v2Position.x + xmax, v2Position.y + ymax, 0.1f );
    pVertex->m_v2UV = MtVector2( 1, 1 );
    pVertex->m_colour = 0xFFFFFFFF;
    ++pVertex;
    
    // Flip the V's
    pQuad[0].m_v2UV.y = 1.0f - pQuad[0].m_v2UV.y;
    pQuad[1].m_v2UV.y = 1.0f - pQuad[1].m_v2UV.y;
    pQuad[2].m_v2UV.y = 1.0f - pQuad[2].m_v2UV.y;
    pQuad[3].m_v2UV.y = 1.0f - pQuad[3].m_v2UV.y;
    
    // Cache the display width and height
	MtVector2 v2TargetDimension = RsRenderTarget::GetCurrent()->GetCameraDimension();
	BtFloat fScaleWidth = 1.0f / v2TargetDimension.x;
	BtFloat fScaleHeight = 1.0f / v2TargetDimension.y;
    
    // Scale the position to local screen space -1 to 1
    for( BtU32 i=0; i<4; i++ )
    {
        // Flip the y
        pQuad[ i ].m_v3Position.y = v2TargetDimension.y - pQuad[ i ].m_v3Position.y;
        
        // Scale from 0..width to 0..1
        pQuad[ i ].m_v3Position.x *= fScaleWidth;
        pQuad[ i ].m_v3Position.y *= fScaleHeight;
        
        // Scale from 0..1 to 0..2
        pQuad[ i ].m_v3Position.x *= 2.0f;
        pQuad[ i ].m_v3Position.y *= 2.0f;
        
        // Translate from 0..2 to -1..1
        pQuad[ i ].m_v3Position.x -= 1.0f;
        pQuad[ i ].m_v3Position.y -= 1.0f;
    }
    
    pMaterial->Render( RsPT_TriangleStrip, pQuad, 4, MaxSortOrders-1 );
}

////////////////////////////////////////////////////////////////////////////////
// RenderRightWayUp

void HlMaterial::RenderRightWayUp( RsMaterial *pMaterial,
								   const MtVector2 &v3Position,
								   const MtVector2 &v2Dimension,
								   const MtVector2 &v2ScreenDimension
								  )
{
	// Allocate vertex
	RsVertex3 vertex[4];
	RsVertex3 *pVertex = &vertex[0];

	// Calculate the positions
	BtFloat xmax = v2Dimension.x;
	BtFloat ymax = v2Dimension.y;

	RsVertex3 *pQuad = pVertex;

	// Copy these into vertex
	pVertex->m_v3Position = MtVector3(0, 0, 0.1f );
	pVertex->m_v2UV = MtVector2( 0, 0 );
	pVertex->m_colour = 0xFFFFFFFF;
	++pVertex;

	pVertex->m_v3Position = MtVector3(0, ymax, 0.1f );
	pVertex->m_v2UV = MtVector2( 0, 1 );
	pVertex->m_colour = 0xFFFFFFFF;
	++pVertex;

	pVertex->m_v3Position = MtVector3(xmax, 0, 0.1f );
	pVertex->m_v2UV = MtVector2( 1, 0 );
	pVertex->m_colour = 0xFFFFFFFF;
	++pVertex;

	pVertex->m_v3Position = MtVector3(xmax, ymax, 0.1f );
	pVertex->m_v2UV = MtVector2( 1, 1 );
	pVertex->m_colour = 0xFFFFFFFF;
	++pVertex;

	// Cache the display width and height
	BtFloat fScaleWidth  = 1.0f / RsUtil::GetWidth();
	BtFloat fScaleHeight = 1.0f / RsUtil::GetHeight();

	// Scale the position to local screen space -1 to 1
	for( BtU32 i=0; i<4; i++ )
	{
		// Set the colour
		pQuad[ i ].m_v3Position.x -= 0.5f;
		pQuad[ i ].m_v3Position.y -= 0.5f;

		// Flip the y
		pQuad[ i ].m_v3Position.y = RsUtil::GetHeight() - pQuad[ i ].m_v3Position.y;

		// Scale from 0..width to 0..1
		pQuad[ i ].m_v3Position.x *= fScaleWidth;
		pQuad[ i ].m_v3Position.y *= fScaleHeight;

		// Scale from 0..1 to 0..2
		pQuad[ i ].m_v3Position.x *= 2.0f;
		pQuad[ i ].m_v3Position.y *= 2.0f;

		// Translate from 0..2 to -1..1
		pQuad[ i ].m_v3Position.x -= 1.0f;
		pQuad[ i ].m_v3Position.y -= 1.0f;
	}

	pMaterial->Render( RsPT_TriangleStrip, pQuad, 4, MaxSortOrders-1 );
}