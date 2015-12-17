////////////////////////////////////////////////////////////////////////////////
// RsUtil.cpp

// Includes
#include "BtMemory.h"
#include "BtTime.h"
#include "BtPrint.h"
#include "RsUtil.h"
#include "RsImpl.h"

////////////////////////////////////////////////////////////////////////////////
// GetHandle

void *RsUtil::GetHandle()
{
	return RsImpl::pInstance()->GetHandle();
}

////////////////////////////////////////////////////////////////////////////////
// GetCaps

RsCaps *RsUtil::GetCaps()
{
	return RsImpl::pInstance()->GetCaps();
}

////////////////////////////////////////////////////////////////////////////////
// EmptyRenderTargets

void RsUtil::EmptyRenderTargets()
{
	return RsImpl::pInstance()->EmptyRenderTargets();
}

////////////////////////////////////////////////////////////////////////////////
// GetNewRenderTarget

RsRenderTarget *RsUtil::GetNewRenderTarget()
{
	// Create a new render target
	RsRenderTarget *pRenderTarget = RsImpl::pInstance()->GetNewRenderTarget();

	// Make sure any new render target is set to the default back buffer
	pRenderTarget->SetTexture( BtNull );

	// Return the render target
	return pRenderTarget;
}

////////////////////////////////////////////////////////////////////////////////
// GetWidth

BtU32 RsUtil::GetWidth()
{
	return RsImpl::pInstance()->GetWidth();
}

////////////////////////////////////////////////////////////////////////////////
// GetHeight

BtU32 RsUtil::GetHeight()
{
	return RsImpl::pInstance()->GetHeight();
}

////////////////////////////////////////////////////////////////////////////////
// GetRefreshRate

BtU32 RsUtil::GetRefreshRate()
{
	return RsImpl::pInstance()->GetRefreshRate();
}

////////////////////////////////////////////////////////////////////////////////
// GetDimension

MtVector2 RsUtil::GetDimension()
{
	return RsImpl::pInstance()->GetDimension();
}

////////////////////////////////////////////////////////////////////////////////
// GetHalfDimension

MtVector2 RsUtil::GetHalfDimension()
{
	return RsImpl::pInstance()->GetDimension() * 0.5f;
}

////////////////////////////////////////////////////////////////////////////////
// SetDimension

void RsUtil::SetDimension( const MtVector2 &v2Dimension )
{
	RsImpl::pInstance()->SetDimension( v2Dimension );
}

//static
BtFloat RsUtil::GetAspect()
{
	return RsImpl::pInstance()->GetDimension().x / RsImpl::pInstance()->GetDimension().y;
}

////////////////////////////////////////////////////////////////////////////////
// GetScreenPosition

//static
MtVector2 RsUtil::GetScreenPosition( const MtVector2 &v2ScreenPosition )
{
	MtVector2 v2Original( 1024.0f, 768.0f );

	MtVector2 v2Position( v2ScreenPosition );

	// Scale by the screen dimension
	v2Position.x /= v2Original.x;
	v2Position.y /= v2Original.y;

	v2Position.x *= RsUtil::GetDimension().x;
	v2Position.y *= RsUtil::GetDimension().y;

	return v2Position;
}

////////////////////////////////////////////////////////////////////////////////
// GetOrientation

RsOrientation RsUtil::GetOrientation()
{
	return RsImpl::pInstance()->GetOrientation();
}

////////////////////////////////////////////////////////////////////////////////
// SetOrientation

void RsUtil::SetOrientation( RsOrientation orientation )
{
	return RsImpl::pInstance()->SetOrientation( orientation );
}

//static
void RsUtil::SetFullScreen( BtBool isFullScreen )
{
	RsImpl::pInstance()->SetFullScreen( isFullScreen );
}

//static
BtBool RsUtil::GetFullScreen()
{
	return RsImpl::pInstance()->IsFullScreen();
}

////////////////////////////////////////////////////////////////////////////////
// GetFPS

BtFloat RsUtil::GetFPS()
{
	return RsImpl::pInstance()->GetFPS();
}

////////////////////////////////////////////////////////////////////////////////
// GetScreenCenter

MtVector2 RsUtil::GetScreenCenter()
{
	return GetDimension() * 0.5f;
}
