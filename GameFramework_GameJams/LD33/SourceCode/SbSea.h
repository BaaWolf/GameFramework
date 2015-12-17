////////////////////////////////////////////////////////////////////////////////
// SbSea.h

// Include guard
#pragma once
#include "BtTypes.h"
#include "RsCamera.h"
#include "MtMatrix4.h"
#include "SbWorld.h"

class RsModel;
class BaArchive;
class SgAnimator;

// Class definition
class SbSea
{
public:

	// Public functions
	SbSea();
	static SbSea				   *GetInstance() { return m_pInstance; }

	void							Init();
	void							Setup( BaArchive *pArchive );
	void							Update();
	void							Render();

	void							MakeSplash(MtVector3 v3Position, BtFloat force);

	// Accessors

private:

	// Private functions
	void							Reset();
	
	// Private members
	SgNode						   *m_pSphere;
	RsFont						   *m_pFont;
	RsShader					   *m_pShader;
	RsMaterial					   *m_pSea;

	RsVertex3						m_heightmap[SeaSize][SeaSize];
	RsVertex3						m_heightmap2[SeaSize][SeaSize];
	BtFloat							u[SeaSize][SeaSize];
	BtFloat							v[SeaSize][SeaSize];
	BtFloat							n[SeaSize][SeaSize];

	static SbSea				   *m_pInstance;
};
