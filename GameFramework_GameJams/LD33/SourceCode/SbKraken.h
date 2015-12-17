////////////////////////////////////////////////////////////////////////////////
// SbKraken.h

// Include guard
#pragma once
#include "BtTypes.h"
#include "RsCamera.h"
#include "MtMatrix4.h"
#include "SbBody.h"

class RsModel;
class BaArchive;
class SgAnimator;

// Class definition
class SbKraken : protected SbBody
{
public:

	SbKraken();

	// Public functions
	void							Setup(BaArchive *pArchive);
	void							Reset();
	void							Update();
	void							Render();

	void							DamageHealth(BtFloat points, MtVector3 v3Position );
	void							GameOver();

	// Accessors
	MtVector3						GetPosition();
	MtVector3						GetLastPosition();
	MtMatrix3						GetRotation();
	static SbKraken				   *GetInstance();
	BtBool							IsVisible();
	BtFloat							GetHealth() { return m_health; }
	
protected:

	// Private functions
	void							KeepUpright();
	void							UpdatePlayerControl();

	// Private members
	SgNode						   *m_pSphere;
	RsFont						   *m_pFont;
	RsShader					   *m_pShader;
	static SbKraken				   *m_pInstance;
	BtFloat							m_angleToUpright;
	MtVector3						m_v3LastPosition;
	BtFloat							m_health;
};
