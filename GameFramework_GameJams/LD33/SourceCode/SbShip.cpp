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
#include "SbKraken.h"
#include "HlFont.h"
#include "SbDepthCharges.h"
#include "SbMain.h"
#include "DyWorld.h"
#include "DyImpl.h"

const BtFloat ShipBuoyancy = 3.0f;

////////////////////////////////////////////////////////////////////////////////
// Setup

void SbShip::Setup( BaArchive *pGameArchive )
{
	DyWorld *pWorld = DyImpl::pWorld();

	// Cache the font
	m_pFont = pGameArchive->GetFont( "vera20" );

	// Cache the main shader
	m_pShader = pGameArchive->GetShader( "shader" );
	
	// Get the ship
	m_pAvatar = pGameArchive->GetNode("ironclad")->GetDuplicate();
//	m_pAvatar = pGameArchive->GetNode("ironclad");

	// Setup the physics body
	MtAABB aabb = m_pAvatar->GetMesh()->GetAABB();
	MtVector3 v3Dimension;
	v3Dimension.x = aabb.Max().x - aabb.Min().x;
	v3Dimension.y = aabb.Max().y - aabb.Min().y;
	v3Dimension.z = aabb.Max().z - aabb.Min().x;

	m_v3SmokePosition[0] = MtVector3( 0, aabb.Max().y, -0.1f );
	m_v3SmokePosition[1] = MtVector3( 0, aabb.Max().y,  0.4f);
	
	m_v3ShipDimension = v3Dimension;

	BtFloat bigMass = 5.0f;
	SbBody::Setup( m_v3ShipDimension, bigMass, ShipBuoyancy );

	HlModel::SetSortOrders( m_pAvatar, ShipSortOrder );

	m_boilerSmoke[0].Setup( pGameArchive );
	m_boilerSmoke[1].Setup( pGameArchive );
	m_bubbles.Setup( pGameArchive );
}

////////////////////////////////////////////////////////////////////////////////
// Reset

void SbShip::Reset()
{
	BtFloat useSize = SeaSize / 5.0f;
	BtFloat x = RdRandom::GetFloat(-useSize, useSize);
	BtFloat y = SeaLevel;
	BtFloat z = RdRandom::GetFloat(-useSize, useSize);
	MtVector3 v3Position(x, y, z);

	MtMatrix4 m4Transform;
	m4Transform.SetTranslation(v3Position);
	m_body.SetWorldTransform(m4Transform);
	m_body.SetWorldVelocity(MtVector3(0, 0, 0));
	m_body.SetWorldAngularVelocity(MtVector3(0, 0, 0));
	m_body.SetDamping(0.5f, 0.5f);

	m_buoyancy = ShipBuoyancy;
	m_hasMotor = BtTrue;
	m_isDead = BtFalse;
	m_hasDepthCharges = BtTrue;
	m_elapsedTime = 0;

	m_v3LastSeen = MtVector3( RdRandom::GetFloat( (BtFloat)-HalfSeaSize, (BtFloat)HalfSeaSize ),
							  SeaLevel,
							  RdRandom::GetFloat( (BtFloat)-HalfSeaSize, (BtFloat)HalfSeaSize) );

	m_v3Offset = MtVector3(RdRandom::GetFloat(-1.0f, 1.0f), 0, RdRandom::GetFloat(-1.0f, 1.0f));

	m_boilerSmoke[0].Reset();
	m_boilerSmoke[1].Reset();
	m_bubbles.Reset();
	m_isSinking = BtFalse;
}

////////////////////////////////////////////////////////////////////////////////
// GetTransform

MtMatrix4 SbShip::GetTransform()
{
	return m_body.GetWorldTransform();
}

////////////////////////////////////////////////////////////////////////////////
// SelfRight

void SbShip::SelfRight()
{
	if( m_isSinking )
	{
		return;
	}

	// Self right the ship in roll
	MtVector3 v3XAxis = -m_body.GetLocalXInWorld().GetNormalise();
	MtVector3 v3Up = MtVector3(0, 1, 0);

	BtFloat smallForce = 0.1f * m_body.GetMass();
	BtFloat angle = v3XAxis.DotProduct(v3Up);
	if(angle < 0)
	{
		m_body.ApplyLocalForce(MtVector3(-smallForce, 0, 0), MtVector3(0, -0.1f, 0));
		m_body.ApplyLocalForce(MtVector3(smallForce, 0, 0), MtVector3(0, 0.1f, 0));
	}
	else
	{
		m_body.ApplyLocalForce(MtVector3(smallForce, 0, 0), MtVector3(0, -0.1f, 0));
		m_body.ApplyLocalForce(MtVector3(-smallForce, 0, 0), MtVector3(0, 0.1f, 0));
	}

	// Self right the ship in pitch
	MtVector3 v3ZAxis = -m_body.GetLocalZInWorld().GetNormalise();
	angle = v3ZAxis.DotProduct(v3Up);
	if(angle < 0)
	{
		m_body.ApplyLocalForce(MtVector3(0, 0, -smallForce), MtVector3(0, -0.1f, 0));
		m_body.ApplyLocalForce(MtVector3(0, 0, smallForce), MtVector3(0, 0.1f, 0));
	}
	else
	{
		m_body.ApplyLocalForce(MtVector3(0, 0, smallForce), MtVector3(0, -0.1f, 0));
		m_body.ApplyLocalForce(MtVector3(0, 0, -smallForce), MtVector3(0, 0.1f, 0));
	}
}

////////////////////////////////////////////////////////////////////////////////
// CheckForDamage

void SbShip::CheckForDamage()
{
	// test code
	BtFloat dp1 = MtVector3(0, 0,  1).DotProduct(MtVector3(0, 0, -1));
	BtFloat dp2 = MtVector3(0, 0, -1).DotProduct(MtVector3(0, 0, -1));
	BtFloat dp3 = MtVector3(0, 0,  1).DotProduct(MtVector3(0, 1,  0));
	BtFloat a = MtRadiansToDegrees( MtACos( dp1 ) );
	BtFloat b = MtRadiansToDegrees( MtACos( dp2 ) );
	BtFloat c = MtRadiansToDegrees( MtACos( dp3 ) );

	MtVector3 v3XAxis = -m_body.GetLocalXInWorld().GetNormalise();
	MtVector3 v3ZAxis = -m_body.GetLocalZInWorld().GetNormalise();
	MtVector3 v3Up = MtVector3(0, 1, 0);
	BtFloat angleX = MtRadiansToDegrees( MtACos( v3XAxis.DotProduct(v3Up) ) );
	BtFloat angleZ = MtRadiansToDegrees( MtACos( v3ZAxis.DotProduct(v3Up) ) );

	BtFloat maxAngle = 25;
	if( ( MtAbs(angleX - 90) > maxAngle ) || ( MtAbs(angleZ - 90) > maxAngle ) )
	{
		m_hasMotor = BtFalse;
		m_hasDepthCharges = BtFalse;
		m_buoyancy -= 0.1f;

		if( m_buoyancy < 0.9f )
		{
			m_buoyancy = 0.9f;
		}
	}

	if( m_bottom < -LostDepth )
	{
		if( m_isDead == BtFalse )
		{
			BtU32 score = SbMain::GetScore();
			++score;
			SbMain::SetScore( score );
			m_isDead = BtTrue;

			if( score >= NumShips )
			{
				SbMain::SetState( GameState_Win );
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// MoveShip

void SbShip::MoveShip()
{
	if( GetDistanceToTarget() < 20.0f )
	{
		m_isMoving = BtFalse;
	}
	else
	{
		m_isMoving = BtTrue;

		MtVector3 v3Position( 0, 0, -m_v3ShipDimension.z * 0.5f );
		MtVector3 v3Motor = GetLocalPosition( v3Position );
		SbSea::GetInstance()->MakeSplash( v3Motor, RdRandom::GetFloat( -0.5f, 0.5f ) );
	}

	if(m_hasMotor && m_isMoving )
	{
		// Speed it forward
		BtFloat speed = m_body.GetMass();
		m_body.ApplyLocalForce(MtVector3(0, 0, speed), MtVector3(0, 0, 0));

		// Find the kraken
		MtVector3 v3CurrentDirection = m_body.GetLocalXInWorld().GetNormalise();

		MtVector3 v3DeltaToTarget = GetDirToTarget();

		m_angle = v3CurrentDirection.DotProduct(v3DeltaToTarget);

		BtFloat turn = 0.01f * m_body.GetMass();
		BtFloat aft = m_v3ShipDimension.z * 0.5f;
		if(m_angle < 0)
		{
			m_body.ApplyLocalForce(MtVector3(-turn, 0, 0), MtVector3(0, 0,  aft));
			m_body.ApplyLocalForce(MtVector3( turn, 0, 0), MtVector3(0, 0, -aft));
		}
		else
		{
			m_body.ApplyLocalForce(MtVector3( turn, 0, 0), MtVector3(0, 0,  aft));
			m_body.ApplyLocalForce(MtVector3(-turn, 0, 0), MtVector3(0, 0, -aft));
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// Update

void SbShip::Update()
{
	UpdateTarget();
	MoveShip();
	SelfRight();
	CheckForDamage();

	BtFloat top    = m_body.GetPosition().y + MtMax( m_v3ShipDimension.z, MtMax( m_v3ShipDimension.x, m_v3ShipDimension.y ) );
	BtFloat bottom = m_body.GetPosition().y - MtMax( m_v3ShipDimension.z, MtMax( m_v3ShipDimension.x, m_v3ShipDimension.y ) );

	if( (bottom >= -SeaDepth ) && ( top < SeaLevel ) )
	{
		m_isSinking = BtTrue;
		m_bubbles.SetEmitter( BtTrue );
	}
	else
	{
 		m_bubbles.SetEmitter(BtFalse);
		m_isSinking = BtFalse;
	}

	m_bubbles.SetPosition( m_body.GetPosition() );
	m_bubbles.SetTransform( m_body.GetWorldTransform() );
	m_bubbles.SetDimension( m_v3ShipDimension );
	m_bubbles.Update();

	for( BtU32 i=0; i<2;i++ )
	{
		m_boilerSmoke[i].SetPosition( m_body.GetPosition() + ( m_v3SmokePosition[i] * m_body.GetRotation() ) );
		m_boilerSmoke[i].SetEmitter( m_hasMotor && m_isMoving );
		m_boilerSmoke[i].Update();
	}

	m_waitingToFire = BtFalse;
	if( m_hasDepthCharges )
	{
		m_elapsedTime += BtTime::GetTick();

		BtFloat distance = GetDistanceToTarget();

		if( ( distance < ChargeAimRange ) && ( m_elapsedTime > ReloadTime ) )
		{
			m_waitingToFire = BtTrue;

			SbDepthCharges::GetInstance()->Fire( this );
			m_elapsedTime = 0;
		}
	}

	// Update the physics model
	SbBody::Update();

	// Update our avatar
	m_pAvatar->SetLocalTransform( m_body.GetWorldTransform());
	m_pAvatar->Update();
}

////////////////////////////////////////////////////////////////////////////////
// UpdateTarget

void SbShip::UpdateTarget()
{
	MtVector3 v3LastSeen = SbKraken::GetInstance()->GetLastPosition();

	if(m_v3LastSeen != v3LastSeen)
	{
		m_v3LastSeen = v3LastSeen;
		m_v3Target = v3LastSeen + m_v3Offset;
	}
	else
	{
		BtFloat distance = RdRandom::GetFloat(5.0f, ChargeAimRange);
		m_v3Offset = MtVector3(RdRandom::GetFloat(-1.0f, 1.0f), 0, RdRandom::GetFloat(-1.0f, 1.0f)) * distance;
	}
}

////////////////////////////////////////////////////////////////////////////////
// GetDirToTarget

MtVector3 SbShip::GetDirToTarget()
{
	MtVector3 v3From = m_body.GetPosition();
	MtVector3 v3Delta = m_v3Target - v3From;
	v3Delta = v3Delta.GetNormalise();
	return v3Delta;
}

////////////////////////////////////////////////////////////////////////////////
// GetDistanceToTarget

BtFloat SbShip::GetDistanceToTarget()
{
	MtVector3 v3From = m_body.GetPosition();
	MtVector3 v3Delta = m_v3Target - v3From;
	return v3Delta.GetLength();
}

////////////////////////////////////////////////////////////////////////////////
// Render

void SbShip::Render()
{
	m_boilerSmoke[0].Render();
	m_boilerSmoke[1].Render();
	m_bubbles.Render();

	//HlDraw::RenderLine( m_body.GetPosition(), m_v3Target, RsColour::WhiteColour(), MaxSortOrders-1 );
	m_pAvatar->Render();
}
