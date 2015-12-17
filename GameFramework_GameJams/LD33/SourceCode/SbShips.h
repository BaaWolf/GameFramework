////////////////////////////////////////////////////////////////////////////////
// SbShips.h

// Include guard
#pragma once
#include "BtTypes.h"
#include "RsCamera.h"
#include "MtMatrix4.h"
#include "SbBody.h"
#include "SbShip.h"

const BtU32 NumShips = 5;

// Class definition
class SbShips
{
public:

	SbShips();

	// Public functions
	void							Setup( BaArchive *pArchive );
	void							Reset();
	void							Update();
	void							Render();

	SbShip						   &GetShip(BtU32 index) { return m_ships[index];  }
	static SbShips				   *GetInstance() { return m_pInstance; }

	//private

private:
	SbShip							m_ships[NumShips];
	static SbShips				   *m_pInstance;
};
