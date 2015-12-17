////////////////////////////////////////////////////////////////////////////////
// SbShip.h

// Include guard
#pragma once
#include "BtTypes.h"
#include "RsCamera.h"
#include "MtMatrix4.h"
#include "SbBody.h"
#include "SbSmoke.h"
#include "SbBubbles.h"

class RsModel;
class BaArchive;
class SgAnimator;

// Class definition
class SbShip : public SbBody
{
public:

	// Public functions
	void							Setup( BaArchive *pArchive );
	void							Reset();
	void							Update();
	void							Render();

	void							MoveShip();
	void							SelfRight();
	void							CheckForDamage();

	// Accessors
	MtVector3						GetPosition() {	return m_body.GetPosition(); }
	void							SetMotor(bool isMotor) { m_hasMotor = isMotor; }
	void							SetDepthCharges(bool hasCharges) { m_hasDepthCharges = hasCharges; }
	BtBool							IsFiring() { return m_waitingToFire; }
	MtMatrix4						GetTransform();
	MtVector3						GetDimension() { return m_v3ShipDimension; }

protected:

	// Private functions
	void							UpdateTarget();
	MtVector3						GetDirToTarget();
	BtFloat							GetDistanceToTarget();

	// Private members
	RsFont						   *m_pFont;
	RsShader					   *m_pShader;
	SgNode						   *m_pAvatar;
	BtFloat							m_angle;

	BtBool							m_hasMotor;
	BtBool							m_isMoving;
	BtBool							m_hasDepthCharges;
	BtBool							m_isDead;

	MtVector3						m_v3ShipDimension;

	BtBool							m_waitingToFire;
	BtFloat							m_elapsedTime;

	MtVector3						m_v3Offset;
	MtVector3						m_v3LastSeen;
	MtVector3						m_v3Target;

	SbSmoke							m_boilerSmoke[2];
	MtVector3						m_v3SmokePosition[2];
	SbBubbles						m_bubbles;

	BtBool							m_isSinking;
};
