////////////////////////////////////////////////////////////////////////////////
// SbShips.h

// Include guard
#pragma once
#include "BtTypes.h"
#include "RsCamera.h"
#include "MtMatrix4.h"
#include "SbBody.h"
#include "SbDepthCharge.h"

const BtU32 NumCharges = 6;
const BtFloat ChargeAimRange = 25;				// Ships start shooting inside this range
const BtFloat ReloadTime = 7;
const BtFloat MinChargeTime = 3;
const BtFloat MaxChargeTime = 5;
const BtFloat DangerousDistance = 12;			// Distance that depth charges will hurt
const BtFloat MaxPointsLost = 50;

class SbShip;

// Class definition
class SbDepthCharges
{
public:

	// Public functions
	SbDepthCharges();
	void							Setup( BaArchive *pArchive );
	void							Reset();
	void							Update();
	void							Render();
	void							Fire( SbShip *ship );

	static SbDepthCharges		   *GetInstance() { return m_pInstance; }

private:
	SbDepthCharge					m_charges[NumCharges];
	static SbDepthCharges		   *m_pInstance;
};
