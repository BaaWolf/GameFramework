////////////////////////////////////////////////////////////////////////////////
// SbMain.h

// Include guard
#pragma once
#include "BtTypes.h"
#include "BaArchive.h"
#include "MtVector3.h"
#include "SbCamera.h"
#include "SdSound.h"
#include "SbRenderSkybox.h"
#include "SbSprite.h"
#include "SbMaterial.h"
#include "SbHealthBar.h"
#include "SbModel.h"
#include "SbPhysics.h"
#include "GaProject.h"
#include "SdSound.h"
#include "Game.h"
#include "SbSea.h"
#include "SbShips.h"
#include "SbKraken.h"
#include "SbDepthCharges.h"
#include "SbMenu.h"

class SbMain;
class RsTexture;
class RsShader;
class RsMaterial;
class SgNode;

// Class definition
class SbMain : public GaProject
{
public:

	// Public functions
	void							Init();
	void							Reset();
	void							Create();
	void							Update();
	void							Render();
	void							PreRender();
	void							PostRender();
	void							Destroy();
    void                            Resize();
	
	// Accessors
	void							SetClosing();
	BtBool							IsClosed();
	BtBool							IsClosing();
	static BtBool					IsFlyCam() { return m_isCameraMode; }
	
	static void						SetState(GameState gameState);
	static GameState				GetState();
	
	static void						SetScore( BtU32 score);
	static BtU32					GetScore();

	BtFloat							GetFarPlane();

private:

	// Private functions

	void							RestoreRenderTarget();
	void							RenderScene();

	void							SetupRenderToTexture( RsTexture *pTexture, RsCamera camera );
	void							SetupRender();
	void							PlaySound();
	void							UpdateTest();
	void							RenderTests();
	void							RenderFont();
	void							DrawRenderTarget();
	void							RenderUpsideDown(RsMaterial *pMaterial, const MtVector2 &v2Position, const MtVector2 &v2Dimension, const MtVector2 &v2ScreenDimension );
	
	// Private members
	BaArchive						m_gameArchive;
	BaArchive						m_hmdArchive;
	BaArchive						m_utilityArchive;
	
	// Resources
	RsMaterial					   *m_pTouch;
	RsShader					   *m_pShader;
	MtVector3						m_v3Position;
	SbCamera						m_camera;
	RsMaterial					   *m_pWhite2;
	RsMaterial					   *m_pWhite3;
	RsShader					   *m_pBloom;
	SdSound						   *m_pSound;

	// Test classes
	SbSprite						m_sprite;
	SbMaterial						m_wolf2;
	SbHealthBar						m_healthBar;
	SbModel							m_model;
	SbRenderSkybox					m_skybox;
	SbPhysics						m_physics;
	SbSea							m_sea;
	SbShips							m_ships;
	SbKraken						m_kraken;
	SbDepthCharges					m_charges;
	SbMenu							m_menu;

	static BtU32					m_score;

	static BtBool					m_isCameraMode;

	static GameState				m_nextState;
	static GameState				m_gameState;

	BtBool							m_isPaused;
};

inline void SbMain::SetClosing()
{
	m_isClosing = BtTrue;
}

inline BtBool SbMain::IsClosed()
{
	return m_isClosed;
}

inline BtBool SbMain::IsClosing()
{
	return m_isClosing;
}
