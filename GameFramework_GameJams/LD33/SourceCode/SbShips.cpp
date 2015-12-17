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

SbShips *SbShips::m_pInstance = BtNull;

////////////////////////////////////////////////////////////////////////////////
// Constructor

SbShips::SbShips()
{
	m_pInstance = this;
}

////////////////////////////////////////////////////////////////////////////////
// Setup

void SbShips::Setup( BaArchive *pGameArchive )
{
	for (BtU32 i = 0; i < NumShips; i++)
	{
		m_ships[i].Setup(pGameArchive);
	}

	m_ships[0].SetMotor(BtTrue);
	m_ships[1].SetMotor(BtTrue);
	m_ships[2].SetMotor(BtTrue);
	m_ships[3].SetMotor(BtFalse);
	m_ships[4].SetMotor(BtFalse);
}

////////////////////////////////////////////////////////////////////////////////
// Reset

void SbShips::Reset()
{
	for (BtU32 i = 0; i < NumShips; i++)
	{
		m_ships[i].Reset();
	}
}

////////////////////////////////////////////////////////////////////////////////
// Update

void SbShips::Update()
{
	for (BtU32 i = 0; i < NumShips; i++)
	{
		m_ships[i].Update();
	}
}

////////////////////////////////////////////////////////////////////////////////
// Render

void SbShips::Render()
{
	for (BtU32 i = 0; i < NumShips; i++ )
	{
		m_ships[i].Render();
	}
}
