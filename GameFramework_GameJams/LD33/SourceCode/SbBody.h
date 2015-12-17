////////////////////////////////////////////////////////////////////////////////
// SbBody.h

// Include guard
#pragma once
#include "BtTypes.h"
#include "RsCamera.h"
#include "MtMatrix4.h"
#include "SbWorld.h"
#include "DyBody.h"

class RsModel;
class BaArchive;
class SgAnimator;

// Class definition
class SbBody
{
public:

	// Public functions
	void							Setup( BtFloat diam, BtFloat mass, BtFloat buoyancy);
	void							Setup( MtVector3 v3Dimension, BtFloat mass, BtFloat buoyancy);
	
	MtVector3						GetLocalPosition( MtVector3 v3RelativePosition );

	void							Reset();
	void							Update();
	void							Render();

	// Accessors

protected:

	DyBody							m_body;
	BtFloat							m_top;
	BtFloat							m_bottom;
	BtFloat							m_buoyancy;
	MtVector3						m_v3Dimension;
	MtVector3						m_v3HalfDimension;

private:

	MtVector3						m_v3BodyVertex[8];
};
