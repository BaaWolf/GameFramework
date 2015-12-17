////////////////////////////////////////////////////////////////////////////////
// SbSea.cpp

// Includes
#include "BaArchive.h"
#include "SbModel.h"
#include "MtVector2.h"
#include "RsCamera.h"
#include "RsColour.h"
#include "RsFont.h"
#include "RsShader.h"
#include "RsUtil.h"
#include "RsRenderTarget.h"
#include "SgNode.h"
#include "MtAABB.h"
#include "SgMesh.h"
#include "SgAnimator.h"
#include "BtTime.h"
#include <stdio.h>
#include "UiKeyboard.h"
#include "HlDraw.h"
#include "ApConfig.h"
#include "SbSea.h"
#include "HlModel.h"
#include "SbKraken.h"

RsColour SeaColour = RsColour(0.2f, 0.3f, 0.4f, 0.7f);

////////////////////////////////////////////////////////////////////////////////
// Constructor

SbSea::SbSea()
{
	m_pInstance = this;
}

////////////////////////////////////////////////////////////////////////////////
// Setup

void SbSea::Setup( BaArchive *pGameArchive )
{
	// Cache our model
	m_pSphere = pGameArchive->GetNode( "sphere" );

	// Cache the font
	m_pFont = pGameArchive->GetFont( "vera20" );

	// Cache the main shader
	m_pShader = pGameArchive->GetShader( "shader" );

	// Cache the material for the sea
	m_pSea = pGameArchive->GetMaterial("sea");
	m_pSea->SetTechniqueName( "RsShaderLC" );

	// Set the heightmap
	MtVector3 v3Centre(SeaSize / 2, 0, SeaSize / 2);

	BtU32 i = 0;
	for (BtU32 x = 0; x < SeaSize; x++)
	{
		for (BtU32 y = 0; y < SeaSize; y++)
		{
			RsVertex3 &vertex = m_heightmap[x][y];
			vertex.m_v3Normal = MtVector3(0, 1.0f, 0);
			vertex.m_v3Position = MtVector3( (BtFloat)x, 0, (BtFloat)y) - v3Centre;
			vertex.m_v2UV = MtVector2((BtFloat)x / SeaSize, (BtFloat)y / SeaSize);	
			vertex.m_colour = SeaColour.asWord();

			int a = 0;
			a++;
		}
	}

	Reset();
}

////////////////////////////////////////////////////////////////////////////////
// Reset

void SbSea::Reset()
{
	for (BtU32 x = 0; x < SeaSize; x++)
	{
		for (BtU32 y = 0; y < SeaSize; y++)
		{
			u[x][y] = 0;
			v[x][y] = 0;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// MakeSplash

void SbSea::MakeSplash( MtVector3 v3Position, BtFloat force )
{
	MtVector3 v3Centre(SeaSize / 2, 0, SeaSize / 2);

	int x = v3Position.x + v3Centre.x;
	int y = v3Position.z + v3Centre.z;

	u[x][y] = RdRandom::GetFloat( force / 2, force );
}

////////////////////////////////////////////////////////////////////////////////
// Update

void SbSea::Update()
{
	// http://freespace.virgin.net/hugo.elias/graphics/x_water.htm

	if( UiKeyboard::pInstance()->IsPressed( UiKeyCode_R ) )
	{
		Reset();
	}

	for (BtU32 i = 0; i < 10; i++)
	{
		BtU32 x = RdRandom::GetNumber(0, SeaSize-1);
		BtU32 y = RdRandom::GetNumber(0, SeaSize-1);
		SbSea::GetInstance()->MakeSplash( MtVector3( x, 0, y ), RdRandom::GetFloat( 0.1f, 0.2f ) );
	}

	for (BtU32 i = 1; i < SeaSize - 1; i++)
	{
		for (BtU32 j = 1; j < SeaSize - 1; j++)
		{
			n[i][j] = (u[i-1][j] + u[i+1][j] + u[i][j-1] + u[i][j+1] ) / 2.0f - v[i][j];
			n[i][j] -= n[i][j] / 64.0f;
		}
	}

	for (BtU32 i = 1; i < SeaSize - 1; i++)
	{
		for (BtU32 j = 1; j < SeaSize - 1; j++)
		{
			m_heightmap[i][j].m_v3Position.y = u[i][j];
			m_heightmap[i][j].m_v2UV = MtVector2( (BtFloat)i / (BtFloat)SeaSize, (BtFloat)j / (BtFloat)SeaSize );
			
			MtVector3 v3First  = m_heightmap[i][j].m_v3Position - m_heightmap[i + 1][j].m_v3Position;
			MtVector3 v3Second = m_heightmap[i][j].m_v3Position - m_heightmap[i][j+1].m_v3Position;
			m_heightmap[i][j].m_v3Normal = -v3First.CrossProduct(v3Second).Normalise();

			v[i][j] = n[i][j];
			BtSwap(u[i][j], v[i][j]);
		}
	}
}

const BtU32 num = SeaSize * SeaSize * 2;
RsVertex3 lineVertexX[num];
RsVertex3 lineVertexY[num];
RsVertex3 stripVertexY[num];
RsVertex3 stripVertexY2[num];

////////////////////////////////////////////////////////////////////////////////
// Render

void SbSea::Render()
{
	BtFloat width = SeaSize / 2;
	BtFloat min = 1.0f / width;
	BtFloat max = 1.0f - min;

	RsColour col = SeaColour;
	BtFloat alpha = SeaColour.Alpha();

	BtFloat depth = SbKraken::GetInstance()->GetPosition().y;
	if(depth < 0)
	{
		BtFloat depthRatio = depth / -SeaDepth;
		BtFloat ambColour = MtLerp(depthRatio, 1.0f, 0.1f);

		BtFloat transparency = MtLerp(depthRatio, alpha, 0.8f);

		col = RsColour(0.15f * ambColour, 0.3f * ambColour, 0.5f * ambColour, transparency );
	}
	else
	{
		col.Alpha( alpha );
	}

	if( 0 )
	{
		// Draw the columns
		int i = 0;
		for (BtU32 x = 0; x < SeaSize; x++ )
		{
			for (BtU32 y = 0; y < SeaSize - 1; y++)
			{
				lineVertexX[i] = m_heightmap[x][y];
				i++;
				lineVertexX[i] = m_heightmap[x][y+1];
				i++;
			}
		}
		m_pSea->Render(RsPT_LineList, lineVertexX, i, SeaSortOrder, BtFalse);

		// Draw the rows
		i = 0;
		for (BtU32 x = 0; x < SeaSize - 1; x++)
		{
			for (BtU32 y = 0; y < SeaSize; y++)
			{
				lineVertexY[i] = m_heightmap[x][y];
				i++;
				lineVertexY[i] = m_heightmap[x+1][y];
				i++;
			}
		}
		m_pSea->Render(RsPT_LineList, lineVertexY, i, SeaSortOrder, BtFalse);
	}

	// Draw the triangle strips for above the waves
	int i=0;
	for (BtU32 x = 0; x < SeaSize - 1; x++)
	{
		int s = i;
		int t = i;
		for (BtU32 y = 0; y < SeaSize; y++)
		{
			stripVertexY2[i] = m_heightmap[x][y];
			stripVertexY2[i].m_colour = col.asWord();
			i++;
			stripVertexY2[i] = m_heightmap[x + 1][y];
			stripVertexY2[i].m_colour = col.asWord();
			i++;
			t += 2;
		}
		m_pSea->Render(RsPT_TriangleStrip, stripVertexY2 + s, t - s, SeaSortOrder, BtFalse);
	}

	BtFloat big = 10000;

	RsVertex3 vertex[4];

	vertex[0].m_v3Normal = MtVector3(0, 1, 0);
	vertex[1].m_v3Normal = MtVector3(0, 1, 0);
	vertex[2].m_v3Normal = MtVector3(0, 1, 0);
	vertex[3].m_v3Normal = MtVector3(0, 1, 0);
	vertex[0].m_v2UV = MtVector2(0, 0);
	vertex[1].m_v2UV = MtVector2(0, 0);
	vertex[2].m_v2UV = MtVector2(0, 0);
	vertex[3].m_v2UV = MtVector2(0, 0);

	vertex[0].m_colour = col.asWord();
	vertex[0].m_v3Position = MtVector3( -HalfSeaSize, 0, -HalfSeaSize+1 );
	
	vertex[1].m_colour = col.asWord();
	vertex[1].m_v3Position = MtVector3(  HalfSeaSize, 0, -HalfSeaSize+1 );
	
	vertex[2].m_colour = col.asWord();
	vertex[2].m_v3Position = MtVector3( -HalfSeaSize, 0, -big );

	vertex[3].m_colour = col.asWord();
	vertex[3].m_v3Position = MtVector3(  HalfSeaSize, 0, -big );
	m_pSea->Render(RsPT_TriangleStrip, vertex, 4, SeaSortOrder, BtTrue);

	vertex[0].m_colour = col.asWord();
	vertex[0].m_v3Position = MtVector3( HalfSeaSize, 0,  HalfSeaSize-1);

	vertex[1].m_colour = col.asWord();
	vertex[1].m_v3Position = MtVector3( -HalfSeaSize, 0,  HalfSeaSize-1);

	vertex[2].m_colour = col.asWord();
	vertex[2].m_v3Position = MtVector3( HalfSeaSize, 0,  big);

	vertex[3].m_colour = col.asWord();
	vertex[3].m_v3Position = MtVector3( -HalfSeaSize, 0, big);
	m_pSea->Render(RsPT_TriangleStrip, vertex, 4, SeaSortOrder, BtTrue);

	// Sides
	BtFloat s = HalfSeaSize - 1.0f;
	vertex[0].m_colour = col.asWord();
	vertex[0].m_v3Position = MtVector3( s, 0, big);

	vertex[1].m_colour = col.asWord();
	vertex[1].m_v3Position = MtVector3( s + big, 0, big);

	vertex[2].m_colour = col.asWord();
	vertex[2].m_v3Position = MtVector3( s, 0, -big);

	vertex[3].m_colour = col.asWord();
	vertex[3].m_v3Position = MtVector3( s + big, 0, -big);
	m_pSea->Render(RsPT_TriangleStrip, vertex, 4, SeaSortOrder, BtTrue);

	// Sides
	s = -s ;
	vertex[0].m_colour = col.asWord();
	vertex[0].m_v3Position = MtVector3( s - big, 0, big);

	vertex[1].m_colour = col.asWord();
	vertex[1].m_v3Position = MtVector3( s, 0, big);
	
	vertex[2].m_colour = col.asWord();
	vertex[2].m_v3Position = MtVector3( s - big, 0, -big);

	vertex[3].m_colour = col.asWord();
	vertex[3].m_v3Position = MtVector3( s, 0, -big);
	m_pSea->Render(RsPT_TriangleStrip, vertex, 4, SeaSortOrder, BtTrue);
}
