////////////////////////////////////////////////////////////////////////////////
// SbModel.cpp

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
#include "SgBone.h"
#include "HlModel.h"
#include "SbWorld.h"

////////////////////////////////////////////////////////////////////////////////
// Setup

void SbModel::Setup( BaArchive *pGameArchive, BaArchive *pAnimArchive )
{
	// Cache our model
	//m_pSphere = pGameArchive->GetNode( "sphere" );

	// Cache the main shader
	m_pShader = pGameArchive->GetShader( "shader" );
}

////////////////////////////////////////////////////////////////////////////////
// Update

void SbModel::Update()
{
}

////////////////////////////////////////////////////////////////////////////////
// Render

void SbModel::Render()
{
}
