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
#include "SbDepthCharges.h"
#include "HlFont.h"

SbDepthCharges *SbDepthCharges::m_pInstance = BtNull;

////////////////////////////////////////////////////////////////////////////////
// Constructor

SbDepthCharges::SbDepthCharges()
{
	m_pInstance = this;
}

////////////////////////////////////////////////////////////////////////////////
// Setup

void SbDepthCharges::Setup( BaArchive *pGameArchive )
{
	for (BtU32 i = 0; i < NumCharges; i++)
	{
		m_charges[i].Setup(pGameArchive, i);
	}
}

////////////////////////////////////////////////////////////////////////////////
// Reset

void SbDepthCharges::Reset()
{
	for (BtU32 i = 0; i < NumCharges; i++)
	{
		m_charges[i].Reset();
	}
}

////////////////////////////////////////////////////////////////////////////////
// Fire

void SbDepthCharges::Fire( SbShip *pShip )
{
	for(BtU32 i = 0; i < NumCharges; i++)
	{
		SbDepthCharge &depthCharge = m_charges[i];

		if( depthCharge.IsActive() == BtFalse )
		{
			m_charges[i].Fire( pShip );
			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// Update

void SbDepthCharges::Update()
{
	for (BtU32 i = 0; i < NumCharges; i++)
	{
		m_charges[i].Update();
	}
}

////////////////////////////////////////////////////////////////////////////////
// Render

void SbDepthCharges::Render()
{
	for (BtU32 i = 0; i < NumCharges; i++ )
	{
		m_charges[i].Render();
	}
}
