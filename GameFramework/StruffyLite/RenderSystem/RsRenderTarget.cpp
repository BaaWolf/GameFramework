////////////////////////////////////////////////////////////////////////////////
// RsRenderTarget

// Includes
#include "BtString.h"
#include "RsRenderTarget.h"
#include "RsImpl.h"

//static

RsRenderTarget *RsRenderTarget::m_pCurrentRenderTarget = BtNull;

///////////////////////////////////////////////////////////////////////////////
// Constructor

RsRenderTarget::RsRenderTarget()
{
}

///////////////////////////////////////////////////////////////////////////////
// Apply

void RsRenderTarget::Apply()
{
	m_pCurrentRenderTarget = this;
}

///////////////////////////////////////////////////////////////////////////////
// GetCurrent

RsRenderTarget *RsRenderTarget::GetCurrent()
{
	return m_pCurrentRenderTarget;
}

///////////////////////////////////////////////////////////////////////////////
// SetName

void RsRenderTarget::SetName( const BtChar *name )
{
	BtStrCopy( m_name, 256, name, 256 );
}

///////////////////////////////////////////////////////////////////////////////
// SetTexture

void RsRenderTarget::SetTexture( RsTexture *pTexture )
{
	m_pTexture = pTexture;
}

///////////////////////////////////////////////////////////////////////////////
// GetTexture

RsTexture *RsRenderTarget::GetTexture()
{
	return m_pTexture;
}

///////////////////////////////////////////////////////////////////////////////
// SetCamera

void RsRenderTarget::SetCamera( const RsCamera &camera )
{
	m_camera = camera;
}

///////////////////////////////////////////////////////////////////////////////
// GetCamera

const RsCamera &RsRenderTarget::GetCamera()
{
	return m_camera;
}

///////////////////////////////////////////////////////////////////////////////
// GetCurrentDimension

MtVector2 RsRenderTarget::GetCameraDimension()
{
	return m_camera.GetDimension();
}

///////////////////////////////////////////////////////////////////////////////
// SetClearColour

void RsRenderTarget::SetClearColour( const RsColour &Colour )
{
	m_clearColour = Colour;
}

///////////////////////////////////////////////////////////////////////////////
// GetClearColour

RsColour RsRenderTarget::GetClearColour() const
{
	return m_clearColour;
}

///////////////////////////////////////////////////////////////////////////////
// SetCleared

void RsRenderTarget::SetCleared( const BtBool &isCleared )
{
	m_isCleared = isCleared;
}

///////////////////////////////////////////////////////////////////////////////
// IsCleared

BtBool RsRenderTarget::IsCleared() const
{
	return m_isCleared;
}

///////////////////////////////////////////////////////////////////////////////
// SetZCleared

void RsRenderTarget::SetZCleared( BtBool isZCleared )
{
	m_isZCleared = isZCleared;
}

///////////////////////////////////////////////////////////////////////////////
// IsZCleared

BtBool RsRenderTarget::IsZCleared() const
{
	return m_isZCleared;
}