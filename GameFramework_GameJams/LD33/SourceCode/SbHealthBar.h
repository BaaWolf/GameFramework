////////////////////////////////////////////////////////////////////////////////
// SbTexture.h

// Include guard
#pragma once
#include "BtTypes.h"

class RsMaterial;
class BaArchive;

// Class definition
class SbHealthBar
{
public:

	// Public functions
	void							Init();
	void							Setup( BaArchive *pArchive );
	void							Update();
	void							Render();

	// Accessors

private:

	// Private functions

	// Private members
	RsMaterial					   *m_pWhite;
	RsMaterial					   *m_pShip;
};
