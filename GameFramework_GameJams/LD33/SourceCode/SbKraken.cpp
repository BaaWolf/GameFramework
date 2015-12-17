////////////////////////////////////////////////////////////////////////////////
// SbKraken.cpp

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
#include "SbKraken.h"
#include "HlModel.h"
#include "SbMain.h"
#include "Ui360.h"
#include "SbShips.h"

//static
SbKraken *SbKraken::m_pInstance;

////////////////////////////////////////////////////////////////////////////////
// Constructor

SbKraken::SbKraken()
{
	m_pInstance = this;
}

////////////////////////////////////////////////////////////////////////////////
// Setup

void SbKraken::Setup(BaArchive *pGameArchive)
{
	// Cache our model
	m_pSphere = pGameArchive->GetNode("sphere");

	// Cache the font
	m_pFont = pGameArchive->GetFont("vera20");

	// Cache the main shader
	m_pShader = pGameArchive->GetShader("shader");

	BtFloat diam = 1.0f;
	BtFloat mass = 1.0f;
	BtFloat buoyancy = 1.0f;
	SbBody::Setup( diam, mass, buoyancy);
	m_v3Dimension = MtVector3(diam, diam, diam);

	// Set the singleton
	m_pInstance = this;
}

////////////////////////////////////////////////////////////////////////////////
// GetInstance

SbKraken *SbKraken::GetInstance()
{
	return m_pInstance;
}

////////////////////////////////////////////////////////////////////////////////
// Reset

void SbKraken::Reset()
{
	MtVector3 v3Position = MtVector3(5.0f, -m_v3Dimension.y * 2.0f, 5.0f);
	MtVector3 v3To = SbShips::GetInstance()->GetShip(0).GetPosition();

	MtMatrix4 m4Rotation;
	m4Rotation.LookAt(v3Position, v3To, MtVector3(0, 1.0f, 0));
	m4Rotation = m4Rotation.GetInverse();

	MtMatrix4 m4Transform;
	m4Transform.SetTranslation(v3Position);
	m_body.SetWorldTransform(m4Rotation * m4Transform);
	m_body.SetWorldVelocity(MtVector3(0, 0, 0));
	m_body.SetWorldAngularVelocity(MtVector3(0, 0, 0));
	m_body.SetDamping(0.7f, 0.8f);

	m_v3LastPosition = MtVector3( 0, 0, 0);
	m_health = 100.0f;
}

////////////////////////////////////////////////////////////////////////////////
// GetPosition

MtVector3 SbKraken::GetPosition()
{
	return m_body.GetPosition();
}

////////////////////////////////////////////////////////////////////////////////
// GetLastPosition

MtVector3 SbKraken::GetLastPosition()
{
	return m_v3LastPosition;
}

////////////////////////////////////////////////////////////////////////////////
// IsVisible

BtBool SbKraken::IsVisible()
{
	BtFloat depth = GetPosition().y;

	if(depth + EyeHeight > SeaLevel )
	{
		return BtTrue;
	}
	return BtFalse;
}

////////////////////////////////////////////////////////////////////////////////
// GetRotation

MtMatrix3 SbKraken::GetRotation()
{
	return m_body.GetRotation();
}

////////////////////////////////////////////////////////////////////////////////
// UpdatePlayerControl

void SbKraken::UpdatePlayerControl()
{
	BtFloat speed = 7.0f * m_body.GetMass();
	BtFloat turn = 2.0f * m_body.GetMass();

	if( !SbMain::IsFlyCam()  )
	{
		if (m_bottom < SeaLevel)
		{
			if (UiKeyboard::pInstance()->IsHeld(UiKeyCode_W))
			{
				m_body.ApplyLocalForce(MtVector3(0, 0, speed), MtVector3(0, 0, 0));
			}

			if (UiKeyboard::pInstance()->IsHeld(UiKeyCode_S))
			{
				m_body.ApplyLocalForce(MtVector3(0, 0, -speed), MtVector3(0, 0, 0));
			}
			if (UiKeyboard::pInstance()->IsHeld(UiKeyCode_D))
			{
				m_body.ApplyLocalForce(MtVector3(speed, 0, 0), MtVector3(0, 0, 0));
			}
			if (UiKeyboard::pInstance()->IsHeld(UiKeyCode_A))
			{
				m_body.ApplyLocalForce(MtVector3(-speed, 0, 0), MtVector3(0, 0, 0));
			}

			if (UiKeyboard::pInstance()->IsHeld(UiKeyCode_LEFT))
			{
				m_body.ApplyWorldForce(MtVector3(-turn, 0, 0), MtVector3(0, 0, 0.1f));
				m_body.ApplyWorldForce(MtVector3(turn, 0, 0), MtVector3(0, 0, -0.1f));
			}

			if (UiKeyboard::pInstance()->IsHeld(UiKeyCode_RIGHT))
			{
				m_body.ApplyWorldForce(MtVector3(+turn, 0, 0), MtVector3(0, 0, 0.1f));
				m_body.ApplyWorldForce(MtVector3(-turn, 0, 0), MtVector3(0, 0, -0.1f));
			}

			if (UiKeyboard::pInstance()->IsHeld(UiKeyCode_UP))
			{
				m_body.ApplyLocalForce(MtVector3(0, 0, -turn), MtVector3(0, -0.1f, 0));
				m_body.ApplyLocalForce(MtVector3(0, 0, turn), MtVector3(0, 0.1f, 0));
			}
			if (UiKeyboard::pInstance()->IsHeld(UiKeyCode_DOWN))
			{
				m_body.ApplyLocalForce(MtVector3(0, 0, turn), MtVector3(0, -0.1f, 0));
				m_body.ApplyLocalForce(MtVector3(0, 0, -turn), MtVector3(0, 0.1f, 0));
			}
		}
	}

	for (BtU32 controller = 0; controller < MaxControllers; controller++)
	{
		if (Ui360::IsConnected(controller) == BtTrue)
		{
			MtVector2 v2LeftThumbStick = Ui360::LeftThumb(controller);
			m_body.ApplyLocalForce(MtVector3(0, 0, speed * v2LeftThumbStick.y), MtVector3(0, 0, 0));
			m_body.ApplyLocalForce(MtVector3(speed * v2LeftThumbStick.x, 0, 0), MtVector3(0, 0, 0));

			MtVector2 v2RightThumbStick = Ui360::RightThumb(controller);
			m_body.ApplyWorldForce(MtVector3(-turn * v2RightThumbStick.x, 0, 0), MtVector3(0, 0, -0.1f));
			m_body.ApplyWorldForce(MtVector3(turn * v2RightThumbStick.x, 0, 0), MtVector3(0, 0, 0.1f));

			m_body.ApplyLocalForce(MtVector3(0, 0, turn * v2RightThumbStick.y), MtVector3(0, 0.1f, 0));
			m_body.ApplyLocalForce(MtVector3(0, 0, -turn * v2RightThumbStick.y), MtVector3(0, -0.1f, 0));
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// KeepUpright

void SbKraken::KeepUpright()
{
	// Self right the Kraken
	MtVector3 v3XAxis = -m_body.GetLocalXInWorld().GetNormalise();
	MtVector3 v3Up = MtVector3(0, 1, 0);

	m_angleToUpright = v3XAxis.DotProduct(v3Up);

	BtFloat smallForce = MtAbs( m_angleToUpright ) * m_body.GetMass() * 4.0f;

	if (m_angleToUpright < 0)
	{
		m_body.ApplyLocalForce(MtVector3(-smallForce, 0, 0), MtVector3(0, -0.1f, 0));
		m_body.ApplyLocalForce(MtVector3(smallForce, 0, 0), MtVector3(0, 0.1f, 0));
	}
	else
	{
		m_body.ApplyLocalForce(MtVector3(smallForce, 0, 0), MtVector3(0, -0.1f, 0));
		m_body.ApplyLocalForce(MtVector3(-smallForce, 0, 0), MtVector3(0, 0.1f, 0));
	}
}

////////////////////////////////////////////////////////////////////////////////
// Update

void SbKraken::Update()
{
	// Make visible to the ships
	BtFloat depth = GetPosition().y;

	if (depth + EyeHeight > SeaLevel)
	{
		m_v3LastPosition = GetPosition();
	}

	UpdatePlayerControl();
	KeepUpright();

	// Update the physics model
	SbBody::Update();

	// Update our avatar
	m_pSphere->SetLocalTransform(m_body.GetWorldTransform());
	m_pSphere->Update();
}

////////////////////////////////////////////////////////////////////////////////
// Render

void SbKraken::DamageHealth(BtFloat points, MtVector3 v3Position )
{
	MtVector3 v3Delta = v3Position - GetPosition();

	BtFloat distance = v3Delta.GetLength();

	if( distance > DangerousDistance )
	{
		return;
	}

	// Get this as a ratio
	distance = distance / DangerousDistance;

	BtFloat healthDamage = MtLerp( distance, 0.0f, points );

	m_health -= healthDamage;

	if( m_health <= 0 )
	{
		m_health = 0;
		GameOver();
	}
}

////////////////////////////////////////////////////////////////////////////////
// GameOver

void SbKraken::GameOver()
{
	SbMain::SetState( GameState_Lose );
}

////////////////////////////////////////////////////////////////////////////////
// Render

void SbKraken::Render()
{
	//HlDraw::RenderCross( m_v3LastPosition, 1.0f, MaxSortOrders - 1);

	if (ApConfig::IsDebug())
	{
		SbBody::Render();
	}

	if (SbMain::IsFlyCam())
	{
		m_pSphere->Render();
	}
}
