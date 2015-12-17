////////////////////////////////////////////////////////////////////////////////
// SbPhysics.h

// Include guard
#pragma once
#include "BtTypes.h"
#include "DyBody.h"
#include "DySoftBody.h"

class BaArchive;
class SgNode;
class RsShader;

// Class definition
class SbPhysics
{
public:

	// Public functions
	void							Init();
	void							Setup( BaArchive *pArchive );
	void							Update();
	void							Render();
	void							Destroy();

	void							Reset();

	// Accessors

private:

	// Private function

	// Private members
	SgNode						   *m_pCube1;
	SgNode						   *m_pCube2;
	DyBody							m_cube1;
	DyBody							m_cube2;
	DySoftBody						m_softBody;
};
