////////////////////////////////////////////////////////////////////////////////
// SbDepthCharge.h

// Include guard
#pragma once
#include "BtTypes.h"
#include "RsCamera.h"
#include "MtMatrix4.h"
#include "DyBody.h"
#include "SbTrail.h"

class SbShip;

// Class definition
class SbDepthCharge : protected SbBody
{
public:

	// Public functions
	void							Setup( BaArchive *pArchive, BtU32 index );
	void							Reset();
	void							Update();
	void							Render();
	void							Fire( SbShip *pShip );

	// Accessors
	BtBool							IsActive() { return m_isActive; }
	void							SetActive( BtBool isActive );

protected:

	// Private functions

	// Private members
	RsShader					   *m_pShader;
	SgNode						   *m_pAvatar;
	BtU32							m_index;
	BtBool							m_isActive;
	BtFloat							m_timeElapsed;
	BtFloat							m_timeExplode;
	SdSound						   *m_pDepthChargeExplode;
	SdSound						   *m_pFireDepthCharge;
	MtVector3						m_v3LastPosition;
	SbTrail							m_trail;
};
