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
#include "HlModel.h"

////////////////////////////////////////////////////////////////////////////////
// Setup

void SbPhysics::Setup( BaArchive *pArchive )
{
	DyImpl::Create();
	DyImpl::pWorld()->Gravity( MtVector3( 0, -9.8f, 0 ) );
	DyImpl::pWorld()->SetSubsteps( 0);

	MtVector3 v3CubeDimension(1.0f, 1.0f, 1.0f);
	BtFloat staticBodyMass = 0.0f;
	BtFloat reasonableMass = 10.0f;

	m_pCube1 = pArchive->GetNode( "cube" )->GetDuplicate();
	m_cube1.CreateBox(staticBodyMass, v3CubeDimension);
	m_pCube2 = pArchive->GetNode("cube")->GetDuplicate();
	m_cube2.CreateBox(reasonableMass, v3CubeDimension);
	
	// Set the sort orders on the materials recursively
	HlModel::SetSortOrders(m_pCube1, ModelSortOrder);
	HlModel::SetSortOrders(m_pCube2, ModelSortOrder);

	Reset();


//	m_softBody.C();
//	m_softBody.SetPosition(MtVector3(-3.0f, 5.0f, 0.0f));
//	m_softBody.AttachFirst(&m_cube1);
//	m_softBody.AttachLast(&m_cube2);
}

////////////////////////////////////////////////////////////////////////////////
// Reset

void SbPhysics::Reset()
{
	MtMatrix4 m4Transform;

	m4Transform.SetTranslation(  2.0f, -2.0f, 0.0f );
	m_cube1.Stop();
	m_cube1.SetWorldTransform(m4Transform);
	
	m4Transform.SetTranslation(  2.0f,  2.0f, 0.0f);
	m_cube2.Stop();
	m_cube2.SetWorldTransform(m4Transform);

//	m_softBody.Destroy();
//	m_softBody.Create();
//	//m_softBody.AttachFirst(&m_cube1);
//	m_softBody.AttachLast(&m_cube2);
//	a=1;
}

////////////////////////////////////////////////////////////////////////////////
// Update

void SbPhysics::Update()
{
	if(UiKeyboard::pInstance()->IsPressed(UiKeyCode_Z))
	{
		m_cube1.ApplyLocalForce(  MtVector3( -1000.0f, 0, 0 ), MtVector3( 0, 0, 0 ) );
	}

	if( UiKeyboard::pInstance()->IsPressed(UiKeyCode_R) )
	{
		Reset();
	}

	DyImpl::pWorld()->Update();
	m_cube1.Update();
	m_cube2.Update();
	m_pCube1->SetLocalTransform( m_cube1.GetWorldTransform() );
	m_pCube1->Update();
	m_pCube2->SetLocalTransform( m_cube2.GetWorldTransform() );
	m_pCube2->Update();
	m_softBody.Update();
}

////////////////////////////////////////////////////////////////////////////////
// Render

void SbPhysics::Render()
{
	m_pCube1->Render();
	m_pCube2->Render();
	m_softBody.Render();
}

////////////////////////////////////////////////////////////////////////////////
// Destroy

void SbPhysics::Destroy()
{
	DyImpl::Destroy();
}