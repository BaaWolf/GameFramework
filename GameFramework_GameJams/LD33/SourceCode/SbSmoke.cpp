////////////////////////////////////////////////////////////////////////////////
// SbParticleDesigner.cpp

// Includes
#include <stdio.h>
#include "SbSmoke.h"
#include "BtTime.h"
#include "ApConfig.h"
#include "RsFont.h"
#include "RsUtil.h"
#include "RsSprite.h"
#include "RsTexture.h"
#include "RsShader.h"
#include "SgMesh.h"
#include "SgNode.h"
#include "ShTouch.h"
#include "UiKeyboard.h"
#include "RdRandom.h"
#include "RsUtil.h"
#include "SbWorld.h"

// http://fxhome.com/support/tutorials/view/39/Creating+realistic+fire+with+particle+effects

////////////////////////////////////////////////////////////////////////////////
// Create

void SbSmoke::Setup( BaArchive *pArchive )
{
	m_pParticle = pArchive->GetMaterial( "smoke" );
	m_pParticle->SetTechniqueName( "RsShaderZA" );
	//m_pParticle->SetTechniqueName("RsShaderZL");
	m_pShader = pArchive->GetShader("shader");
	m_v3Position = MtVector3( 0, 0, 0 );
	m_emitterOn = BtFalse;
}

////////////////////////////////////////////////////////////////////////////////
// Reset

void SbSmoke::Reset()
{
	m_emitterCount = 1;
}

////////////////////////////////////////////////////////////////////////////////
// SetPosition

void SbSmoke::SetPosition( MtVector3 v3Position )
{
	m_v3Position = v3Position;
}

///////////////////////////////////////////////////////////////////////////////
// Update

void SbSmoke::Update()
{
	BtFloat lifeSpan = 0.5f;
	BtFloat currentTime = BtTime::GetElapsedTimeInSeconds();

	if( m_emitterOn )
	{
		//m_emitterCount += 4.0f;
		m_emitterCount += 1.5f;
	}

	while( m_emitterCount >= 1.0f )
	{
		// Make a new particle
		SbParticle *pParticle = m_particles.AddHead();

		BtFloat width  = 0.1f;
		BtFloat height = 0.1f;
		pParticle->m_v3Position = MtVector3( RdRandom::GetFloat( -width, width ), 0, RdRandom::GetFloat( -height, height ) ) + m_v3Position;
		pParticle->m_time = 0;		
		pParticle->m_opacityRandom = RdRandom::GetFloat( 0.54f, 1.0f );
		pParticle->m_speedRandom = RdRandom::GetFloat( 0.7f, 1.3f );

		BtFloat sizeRandom = RdRandom::GetFloat( 0.1f, 0.05f );
		pParticle->m_v2Dimension = sizeRandom;

		pParticle->m_v3Velocity = MtVector3( 0, 2.0f, 0 );

		BtU32 MaxSmokeTextures = 1;
		pParticle->m_frame = RdRandom::GetNumber( 0, 65536 ) % MaxSmokeTextures;

		BtAssert( pParticle->m_frame < MaxSmokeTextures );

		m_emitterCount -= 1.0f;
	}

	// Update the particles
	BtCollectionIterator< SbParticle, MaxSmoke > iterator( m_particles );

	for( SbParticle* pParticle = iterator.GetFirstElement(); pParticle; pParticle = iterator.GetNextElement() )
	{
		pParticle->m_time += BtTime::GetTick();

		if( pParticle->m_time > lifeSpan )
		{
			iterator.DeleteElement();
		}
		else
		{
			BtFloat lifeSpanRatio = pParticle->m_time / lifeSpan;  

			//pParticle->m_colour = RsColour( 1.0f, 1.0f, 1.0f, 1.0f - lifeSpanRatio );
			BtFloat col = 1.0f - lifeSpanRatio;
			//pParticle->m_colour = RsColour(col, col, col, 1.0f );
			pParticle->m_colour = RsColour(col, col, col, 1.0f);

			pParticle->m_v3Position += pParticle->m_v3Velocity * pParticle->m_speedRandom * BtTime::GetTick();
			pParticle->m_v3Position += m_v3Gravity * BtTime::GetTick();
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// Render

void SbSmoke::Render()
{
	m_pShader->Apply();

	MtMatrix3 m3Rotation = RsRenderTarget::GetCurrent()->GetCamera().GetRotation().GetInverse();
	MtVector3 v3AxisX = m3Rotation.XAxis();
	MtVector3 v3AxisY = m3Rotation.YAxis();

	BtCollectionIterator< SbParticle, MaxSmoke > iterator( m_particles );

	BtU32 count = 0;

	for( SbParticle* pParticle = iterator.GetFirstElement(); pParticle; pParticle = iterator.GetNextElement() )
	{
		BtU32 col = pParticle->m_colour.asWord();

		MtVector3 v3AxisXScaled = v3AxisX * pParticle->m_v2Dimension.x;
		MtVector3 v3AxisYScaled = v3AxisY * pParticle->m_v2Dimension.y;

		m_v3Vertex[count].m_v3Position.x = pParticle->m_v3Position.x - v3AxisXScaled.x + v3AxisYScaled.x;
		m_v3Vertex[count].m_v3Position.y = pParticle->m_v3Position.y - v3AxisXScaled.y + v3AxisYScaled.y;
		m_v3Vertex[count].m_v3Position.z = pParticle->m_v3Position.z - v3AxisXScaled.z + v3AxisYScaled.z;
		m_v3Vertex[count].m_v2UV.x = 0;
		m_v3Vertex[count].m_v2UV.y = 1;
		m_v3Vertex[count].m_colour = col;
		count++;

		m_v3Vertex[count].m_v3Position.x = pParticle->m_v3Position.x + v3AxisXScaled.x + v3AxisYScaled.x;
		m_v3Vertex[count].m_v3Position.y = pParticle->m_v3Position.y + v3AxisXScaled.y + v3AxisYScaled.y;
		m_v3Vertex[count].m_v3Position.z = pParticle->m_v3Position.z + v3AxisXScaled.z + v3AxisYScaled.z;
		m_v3Vertex[count].m_v2UV.x = 1;
		m_v3Vertex[count].m_v2UV.y = 1;
		m_v3Vertex[count].m_colour = col;

		count++;
		m_v3Vertex[count].m_v3Position.x = pParticle->m_v3Position.x - v3AxisXScaled.x - v3AxisYScaled.x;
		m_v3Vertex[count].m_v3Position.y = pParticle->m_v3Position.y - v3AxisXScaled.y - v3AxisYScaled.y;
		m_v3Vertex[count].m_v3Position.z = pParticle->m_v3Position.z - v3AxisXScaled.z - v3AxisYScaled.z;
		m_v3Vertex[count].m_v2UV.x = 0;
		m_v3Vertex[count].m_v2UV.y = 0;
		m_v3Vertex[count].m_colour = col;
		count++;

		// Second triangle
		m_v3Vertex[count].m_v3Position.x = pParticle->m_v3Position.x - v3AxisXScaled.x - v3AxisYScaled.x;
		m_v3Vertex[count].m_v3Position.y = pParticle->m_v3Position.y - v3AxisXScaled.y - v3AxisYScaled.y;
		m_v3Vertex[count].m_v3Position.z = pParticle->m_v3Position.z - v3AxisXScaled.z - v3AxisYScaled.z;
		m_v3Vertex[count].m_v2UV.x = 0;
		m_v3Vertex[count].m_v2UV.y = 0;
		m_v3Vertex[count].m_colour = col;

		count++;
		m_v3Vertex[count].m_v3Position.x = pParticle->m_v3Position.x + v3AxisXScaled.x + v3AxisYScaled.x;
		m_v3Vertex[count].m_v3Position.y = pParticle->m_v3Position.y + v3AxisXScaled.y + v3AxisYScaled.y;
		m_v3Vertex[count].m_v3Position.z = pParticle->m_v3Position.z + v3AxisXScaled.z + v3AxisYScaled.z;
		m_v3Vertex[count].m_v2UV.x = 1;
		m_v3Vertex[count].m_v2UV.y = 1;
		m_v3Vertex[count].m_colour = col;
		count++;

		m_v3Vertex[count].m_v3Position.x = pParticle->m_v3Position.x + v3AxisXScaled.x - v3AxisYScaled.x;
		m_v3Vertex[count].m_v3Position.y = pParticle->m_v3Position.y + v3AxisXScaled.y - v3AxisYScaled.y;
		m_v3Vertex[count].m_v3Position.z = pParticle->m_v3Position.z + v3AxisXScaled.z - v3AxisYScaled.z;
		m_v3Vertex[count].m_v2UV.x = 1;
		m_v3Vertex[count].m_v2UV.y = 0;
		m_v3Vertex[count].m_colour = col;
		count++;
	}

	// Render these triangles
	if( count )
	{
		for( BtU32 i=0; i<count; i++ )
		{
			m_v3Vertex[i].m_v3Normal = MtVector3( 0, 1, 0 );
			//m_v3Vertex[i].m_colour = 0xffFFffFF;
		}
		m_pParticle->Render(RsPT_TriangleList, m_v3Vertex, count, ParticleFXSortOrder, BtFalse );
	}
}