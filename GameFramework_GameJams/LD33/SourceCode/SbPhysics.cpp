////////////////////////////////////////////////////////////////////////////////
// SbPhysics.cpp

// Includes
#include "DyImpl.h"
#include "RsVertex.h"
#include "RsTexture.h"
#include "BaArchive.h"
#include "SbPhysics.h"
#include "RsMaterial.h"
#include "SgNode.h"
#include "UiKeyboard.h"
#include "RsShader.h"
#include "SbWorld.h"
#include "ApConfig.h"
#include "BtTime.h"
#include "HlDraw.h"
#include "RsColour.h"
#include "HlModel.h"

////////////////////////////////////////////////////////////////////////////////
// Setup

void SbPhysics::Setup( BaArchive *pArchive )
{
	DyImpl::Create();
	DyImpl::pWorld()->Gravity( MtVector3( 0, -9.8f, 0 ) );
	DyImpl::pWorld()->SetSubsteps( 0);

	m_pFloor = pArchive->GetNode( "floor" )->GetDuplicate();
	m_floor.CreateBox( 0.0f, MtVector3( (BtFloat)SeaSize, 1.0f, (BtFloat)SeaSize ));

 	HlModel::SetSortOrders(m_pFloor, ShipSortOrder);

	Reset();
}

////////////////////////////////////////////////////////////////////////////////
// Reset

void SbPhysics::Reset()
{
	MtMatrix4 m4Transform;
	m4Transform.SetTranslation( -3.0f, -(BtFloat)SeaDepth, 0.0f );
	m_floor.SetWorldTransform( m4Transform );
}

////////////////////////////////////////////////////////////////////////////////
// Update

void SbPhysics::Update()
{
	if( UiKeyboard::pInstance()->IsPressed( UiKeyCode_R ) )
	{
		Reset();
	}

	DyImpl::pWorld()->Update();
	m_floor.Update();
	
	MtMatrix4 m4Scale;
	m4Scale.SetScale(MtVector3( (BtFloat)SeaSize, 1.0f, (BtFloat)SeaSize));
	m_pFloor->SetLocalTransform(m4Scale * m_floor.GetWorldTransform() );
	m_pFloor->Update();
}

////////////////////////////////////////////////////////////////////////////////
// Render

void SbPhysics::Render()
{
	//m_pFloor->Render();
}

////////////////////////////////////////////////////////////////////////////////
// Destroy

void SbPhysics::Destroy()
{
	DyImpl::Destroy();
}