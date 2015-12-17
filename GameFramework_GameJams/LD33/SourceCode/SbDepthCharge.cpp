////////////////////////////////////////////////////////////////////////////////
// SbShips.cpp

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
#include "SbShips.h"
#include "HlModel.h"
#include "SbDepthCharge.h"
#include "SbDepthCharges.h"
#include "HlFont.h"
#include "DyBody.h"
#include "SbKraken.h"
#include "SdSound.h"
#include "SbSea.h"

SbSea *SbSea::m_pInstance = NULL;

////////////////////////////////////////////////////////////////////////////////
// Setup

void SbDepthCharge::Setup( BaArchive *pGameArchive, BtU32 index  )
{
	// Cache the main shader
	m_pShader = pGameArchive->GetShader( "shader" );

	// Cache the sound
	m_pDepthChargeExplode = pGameArchive->GetSound( "depthchargeexplode" );
	m_pFireDepthCharge = pGameArchive->GetSound("firedepthcharge");

	// Get the ship
	m_pAvatar = pGameArchive->GetNode("depthcharge")->GetDuplicate();

	// Set the index
	m_index = index;

	// Setup the physics body
	MtAABB aabb = m_pAvatar->GetMesh()->GetAABB();
	MtVector3 v3Dimension;
	v3Dimension.x = aabb.Max().x - aabb.Min().x;
	v3Dimension.y = aabb.Max().y - aabb.Min().y;
	v3Dimension.z = aabb.Max().z - aabb.Min().x;

	BtFloat smallMass = 0.01f;
	SbBody::Setup( v3Dimension, smallMass, 1.0f );

	m_trail.Setup( pGameArchive->GetMaterial( "chargetrail" ) );

	m_isActive = BtFalse;
}

////////////////////////////////////////////////////////////////////////////////
// SetActive

void SbDepthCharge::SetActive( BtBool isActive )
{
	m_body.SetActive(isActive);
	m_isActive = isActive;
}

////////////////////////////////////////////////////////////////////////////////
// Reset

void SbDepthCharge::Reset()
{
	BtFloat useSize = SeaSize / 5.0f;
	BtFloat x = (BtFloat)m_index;
	BtFloat y = SeaLevel + 10.0f;
	BtFloat z = (BtFloat)m_index;
	MtVector3 v3Position(x, y, z);
	m_v3LastPosition = v3Position;
	m_timeExplode = 0;

	MtMatrix4 m4Transform;
	m4Transform.SetTranslation(v3Position);
	m_body.SetWorldTransform(m4Transform);
	m_body.SetWorldVelocity(MtVector3(0, 0, 0));
	m_body.SetWorldAngularVelocity(MtVector3(0, 0, 0));
	m_body.SetDamping(0.5f, 0.5f);
	SetActive( BtFalse );
}

////////////////////////////////////////////////////////////////////////////////
// Fire

void SbDepthCharge::Fire( SbShip *pShip )
{
	SetActive( BtTrue );

	MtVector3 v3Target = SbKraken::GetInstance()->GetLastPosition();
	MtVector3 v3From = pShip->GetPosition();
	MtVector3 v3Delta = v3Target - v3From;
	BtFloat length = v3Delta.GetLength();
	v3Delta = v3Delta.GetNormalise();

	MtVector3 v3Position = pShip->GetLocalPosition( MtVector3( 0, pShip->GetDimension().y / 2, pShip->GetDimension().y / 3 * 2 ) );
	MtMatrix4 m4Transform;
	m4Transform.SetTranslation( v3Position );
	m_body.SetWorldTransform( m4Transform );

	MtVector3 v3Dir = v3Delta * 0.5f;
	v3Dir.x += RdRandom::GetFloat( -0.1f, 0.1f );
	v3Dir.y = 1.0f;
	v3Dir.z += RdRandom::GetFloat( -0.1f, 0.1f);
	v3Dir.Normalise();
	v3Dir = v3Dir * m_body.GetMass() * length;
	
	m_body.ApplyWorldImpulse( v3Dir, MtVector3( 0, 0, 0 ) );
	m_body.ApplyTorque( MtVector3( RdRandom::GetFloat(0, 0.05f ), RdRandom::GetFloat( 0, 0.05f ), RdRandom::GetFloat( 0, 0.1f ) ) );
	m_body.Update();

	m_timeElapsed = 0;
	m_timeExplode = 1000.0f;
	m_pFireDepthCharge->Play( RdRandom::GetFloat( 0.8f, 1.2f) );

	m_trail.Reset();
}

////////////////////////////////////////////////////////////////////////////////
// Update

void SbDepthCharge::Update()
{
	if( m_isActive )
	{
		// Update the physics model
		SbBody::Update();

		if( m_isActive == BtTrue )
		{
			m_timeElapsed += BtTime::GetTick();

			if( m_timeElapsed > m_timeExplode )
			{
				m_timeElapsed = 0;
				m_pDepthChargeExplode->Play( RdRandom::GetFloat( 0.8f, 1.2f) );
				SetActive( BtFalse );

				SbKraken::GetInstance()->DamageHealth( MaxPointsLost, m_body.GetPosition() );
			}
		}

		// Show it in water
		if( m_top < SeaLevel )
		{
			if( m_timeExplode == 1000.0f )
			{
				m_timeExplode = RdRandom::GetFloat(MinChargeTime, MaxChargeTime);
				m_timeElapsed = 0;
			}
			m_body.SetDamping(0.9f, 0.8f);
		}
		else
		{
			m_body.SetDamping(0.1f, 0.8f);
		}

		// Update our avatar
		m_pAvatar->SetLocalTransform( m_body.GetWorldTransform());
		m_pAvatar->Update();

		MtVector3 currentPosition = m_body.GetPosition();

		if( ( m_v3LastPosition.y > SeaLevel ) && ( currentPosition.y <= SeaLevel ) )
		{
			SbSea::GetInstance()->MakeSplash( currentPosition, 5.0f );
		}

		m_v3LastPosition = currentPosition;

		m_trail.Add( 0, currentPosition, m_pAvatar->GetWorldTransform().GetRotation().Col0() );
		m_trail.Update();
	}
}

////////////////////////////////////////////////////////////////////////////////
// Render

void SbDepthCharge::Render()
{
	if(m_isActive)
	{
		if (ApConfig::IsDebug())
		{
			MtVector3 v3From = m_body.GetPosition();
		//	HlDraw::RenderCross(v3From, 5.0f, MaxSortOrders - 1);
		}

		if( m_body.GetActive() )
		{
			m_pAvatar->Render();
		}

		m_trail.Render();
	}
}
