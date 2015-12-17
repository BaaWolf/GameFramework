////////////////////////////////////////////////////////////////////////////////
// SbBody.cpp

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
#include "SbBody.h"

////////////////////////////////////////////////////////////////////////////////
// Setup

void SbBody::Setup( BtFloat diam, BtFloat mass, BtFloat buoyancy)
{
	//m_body.CreateSphere( mass, MtVector3(diam, diam, diam));
	m_body.CreateBox(mass, MtVector3(diam, diam, diam) );
	m_buoyancy = buoyancy;
	m_v3HalfDimension = diam * 0.5f;
}

////////////////////////////////////////////////////////////////////////////////
// Setup

void SbBody::Setup( MtVector3 v3Dimension, BtFloat mass, BtFloat buoyancy )
{
	m_body.CreateBox( mass, v3Dimension );
	m_buoyancy = buoyancy;
	m_v3Dimension = v3Dimension;
	m_v3HalfDimension = v3Dimension * 0.5f;
}

////////////////////////////////////////////////////////////////////////////////
// Reset

void SbBody::Reset()
{
	MtMatrix4 m4Transform;
	m4Transform.SetTranslation(-10.0f, -0.0f, 5.0f);
	m_body.SetWorldTransform(m4Transform);
	m_body.SetLocalAngularVelocity(MtVector3(0, 0, 0));
	m_body.SetLocalVelocity(MtVector3(0, 0, 0));
	m_body.SetDamping(0.1f, 0.5f);
}

////////////////////////////////////////////////////////////////////////////////
// GetLocalPosition

MtVector3 SbBody::GetLocalPosition(MtVector3 v3RelativePosition)
{
	return v3RelativePosition * m_body.GetWorldTransform();
}

////////////////////////////////////////////////////////////////////////////////
// Update

void SbBody::Update()
{
	// Create a vertex box for each body and transform it
	m_v3BodyVertex[0] = MtVector3(-m_v3HalfDimension.x, -m_v3HalfDimension.y, -m_v3HalfDimension.z);
	m_v3BodyVertex[1] = MtVector3(-m_v3HalfDimension.x, -m_v3HalfDimension.y,  m_v3HalfDimension.z);
	m_v3BodyVertex[2] = MtVector3(-m_v3HalfDimension.x,  m_v3HalfDimension.y, -m_v3HalfDimension.z);
	m_v3BodyVertex[3] = MtVector3(-m_v3HalfDimension.x,  m_v3HalfDimension.y,  m_v3HalfDimension.z);
	m_v3BodyVertex[4] = MtVector3( m_v3HalfDimension.x, -m_v3HalfDimension.y, -m_v3HalfDimension.z);
	m_v3BodyVertex[5] = MtVector3( m_v3HalfDimension.x, -m_v3HalfDimension.y,  m_v3HalfDimension.z);
	m_v3BodyVertex[6] = MtVector3( m_v3HalfDimension.x,  m_v3HalfDimension.y, -m_v3HalfDimension.z);
	m_v3BodyVertex[7] = MtVector3( m_v3HalfDimension.x,  m_v3HalfDimension.y,  m_v3HalfDimension.z);

	for (BtU32 i = 0; i < 8; i++)
	{
		m_v3BodyVertex[i] *= m_body.GetWorldTransform();
	}

	// Calculate the top and bottom extents
	m_bottom = m_v3BodyVertex[0].y;
	m_top = m_bottom;

	for (BtU32 i = 0; i < 8; i++)
	{
		m_bottom = MtMin(m_bottom, m_v3BodyVertex[i].y);
		m_top = MtMax(m_top, m_v3BodyVertex[i].y);
	}

	// We not in the water - ignore this case
	if( m_bottom > SeaLevel)
	{
		return;
	}

	// Make this body buoyant
	MtVector3 v3Position = m_body.GetWorldTransform().GetTranslation();

	BtFloat maxPossibleSubmerged = m_top - m_bottom;
	
	BtFloat actual = SeaLevel - m_bottom;

	actual = MtMin(maxPossibleSubmerged, actual);

	BtFloat ratio = actual / maxPossibleSubmerged;

	BtFloat force = 9.8f * m_buoyancy * ratio * m_body.GetMass();

	m_body.ApplyWorldForce(MtVector3(0, force, 0), MtVector3(0, 0, 0));

	// Update the physics model
	m_body.Update();
}

////////////////////////////////////////////////////////////////////////////////
// Render

void SbBody::Render()
{
	// Render the direction
	MtVector3 v3Offset = m_body.GetLocalZInWorld().GetNormalise() * 5.0f;
	MtVector3 v3Position = m_body.GetPosition();
	HlDraw::RenderLine( v3Position, v3Position + v3Offset, RsColour::GreenColour(), MaxSortOrders - 1);

	// Render the bound box
	/*
	HlDraw::RenderLine( m_v3BodyVertex[0], m_v3BodyVertex[1], RsColour::GreenColour(), MaxSortOrders - 1);
	HlDraw::RenderLine( m_v3BodyVertex[2], m_v3BodyVertex[0], RsColour::GreenColour(), MaxSortOrders - 1);
	HlDraw::RenderLine( m_v3BodyVertex[1], m_v3BodyVertex[3], RsColour::GreenColour(), MaxSortOrders - 1);
	HlDraw::RenderLine( m_v3BodyVertex[2], m_v3BodyVertex[3], RsColour::GreenColour(), MaxSortOrders - 1);
	HlDraw::RenderLine( m_v3BodyVertex[4], m_v3BodyVertex[5], RsColour::GreenColour(), MaxSortOrders - 1);
	HlDraw::RenderLine( m_v3BodyVertex[6], m_v3BodyVertex[4], RsColour::GreenColour(), MaxSortOrders - 1);
	HlDraw::RenderLine( m_v3BodyVertex[5], m_v3BodyVertex[7], RsColour::GreenColour(), MaxSortOrders - 1);
	HlDraw::RenderLine( m_v3BodyVertex[6], m_v3BodyVertex[7], RsColour::GreenColour(), MaxSortOrders - 1);
	HlDraw::RenderLine( m_v3BodyVertex[4], m_v3BodyVertex[0], RsColour::GreenColour(), MaxSortOrders - 1);
	HlDraw::RenderLine( m_v3BodyVertex[5], m_v3BodyVertex[1], RsColour::GreenColour(), MaxSortOrders - 1);
	HlDraw::RenderLine( m_v3BodyVertex[6], m_v3BodyVertex[2], RsColour::GreenColour(), MaxSortOrders - 1);
	HlDraw::RenderLine( m_v3BodyVertex[7], m_v3BodyVertex[3], RsColour::GreenColour(), MaxSortOrders - 1);
	*/

	/*
	// Render the topmost point
	HlDraw::SetCrossColour(RsColour::WhiteColour(), RsColour::WhiteColour(), RsColour::WhiteColour());
	v3Position = m_body.GetPosition();
	v3Position.y = m_top;
	HlDraw::RenderCross(v3Position, 0.5f, MaxSortOrders - 1);

	// Render the bottom most point
	v3Position = m_body.GetPosition();
	v3Position.y = m_bottom;
	HlDraw::SetCrossColour(RsColour::BlackColour(), RsColour::BlackColour(), RsColour::BlackColour());
	HlDraw::RenderCross(v3Position, 0.5f, MaxSortOrders - 1);
	*/
}
