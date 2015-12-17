////////////////////////////////////////////////////////////////////////////////
// SbMain.cpp

// Includes
#include <stdio.h>
#include "SbMain.h"
#include "ApConfig.h"
#include "RsUtil.h"
#include "RsShader.h"
#include "SbCamera.h"
#include "HlMaterial.h"
#include "HlFont.h"
#include "HlDraw.h"
#include "HlMouse.h"
#include "UiKeyboard.h"
#include "RsMaterial.h"
#include "ShHMD.h"
#include "UiKeyboard.h"
#include "HlKeyboard.h"
#include "Ui360.h"
#include "SbMenu.h"
#include "DyImpl.h"
#include "DyWorld.h"
#include "BtTime.h"

//const BtBool useRenderTarget = BtFalse;
//const BtBool useRenderTarget = BtTrue;
BtBool SbMain::m_isCameraMode = BtFalse;
GameState SbMain::m_gameState = GameState_Start;
GameState SbMain::m_nextState = GameState_Start;
BtU32 SbMain::m_score = 0;

////////////////////////////////////////////////////////////////////////////////
// Init

void SbMain::Init()
{
	m_isClosing = BtFalse;
	m_isClosed  = BtFalse;

	MtVector2 v2Dimension;

	ApConfig::SetTitle( "Tutorial 1" );

	if(ApConfig::IsWin() && !ShHMD::IsHMD())
	{
		v2Dimension = MtVector2(1024.0f, 768.0f );

		RsUtil::SetDimension(v2Dimension);
	}
    
    Resize();

	ApConfig::SetDebug( BtFalse );

	RdRandom::SetRandomSeed();

	// Load the game archive
	ApConfig::SetResourcePath( "\\GameFramework\\GavinWood_Games\\BeingAKraken\\Release\\" );

	if (ApConfig::GetPlatform() == ApPlatform_DX11 )
	{
		ApConfig::SetExtension(".windxz");
	}
	else
	{
		ApConfig::SetExtension(".winglz");
	}
	ApConfig::CheckResourcePath( "game" );

	m_gameState = GameState_Start;
	m_gameState = GameState_Playing;
}

////////////////////////////////////////////////////////////////////////////////
// SetScore

void SbMain::SetScore(BtU32 score)
{
	m_score = score;
}

////////////////////////////////////////////////////////////////////////////////
// GetState

BtU32 SbMain::GetScore()
{
	return m_score;
}

////////////////////////////////////////////////////////////////////////////////
// SetState

void SbMain::SetState( GameState gameState )
{
	m_nextState = gameState;
}

////////////////////////////////////////////////////////////////////////////////
// GetState

GameState SbMain::GetState()
{
	return m_gameState;
}

////////////////////////////////////////////////////////////////////////////////
// Resize

void SbMain::Resize()
{
    MtVector2 v2Dimension = RsUtil::GetDimension();
    m_camera.SetDimension( MtVector2( v2Dimension.x, v2Dimension.y ) );
    m_camera.Init();
    m_camera.SetPosition( MtVector3( 0, 2.0f, -5.0f ) );
    m_camera.SetSpeed( 10.0f );
}

////////////////////////////////////////////////////////////////////////////////
// Create

void SbMain::Create()
{
	m_gameArchive.Load( "game" );
	m_hmdArchive.Load( "oculus");
	m_utilityArchive.Load("utility");
    
	if( ShHMD::IsHMD() )
	{
		ShHMD::SetMaterial( 0, m_hmdArchive.GetMaterial( "ovrl" ) );
		ShHMD::SetMaterial( 1, m_hmdArchive.GetMaterial( "ovrr" ) );
	}

	m_pWhite2 = m_gameArchive.GetMaterial( "white2" );
	m_pWhite3 = m_gameArchive.GetMaterial( "white3" );
	m_pShader = m_gameArchive.GetShader( "shader" );
	 
	m_wolf2.Setup( &m_gameArchive );
	m_skybox.Setup( &m_gameArchive );
	m_model.Setup( &m_gameArchive, BtNull);
	m_healthBar.Setup( &m_gameArchive );
	m_physics.Setup(&m_gameArchive);
	DyWorld *pWorld = DyImpl::pWorld();

	m_sea.Setup(&m_gameArchive);
	m_ships.Setup(&m_gameArchive);
	m_kraken.Setup(&m_gameArchive);
	m_charges.Setup(&m_gameArchive);
	m_menu.Setup(&m_gameArchive);

	RsMaterial *pMaterial2 = m_gameArchive.GetMaterial("white2");
	RsMaterial *pMaterial3 = m_gameArchive.GetMaterial("white3");
	RsMaterial *pMaterial3notest = m_gameArchive.GetMaterial("white3noztest");

	HlFont::Setup( &m_utilityArchive, "vera" );
	HlMouse::Setup( &m_utilityArchive );
	HlDraw::Setup( pMaterial2, pMaterial3, pMaterial3notest );

	m_pSound = m_gameArchive.GetSound( "howl" );

	m_camera.Update();
	m_isPaused = BtFalse;
}

////////////////////////////////////////////////////////////////////////////////
// Reset

void SbMain::Reset()
{
	m_charges.Reset();
	m_physics.Reset();
	m_ships.Reset();
	m_kraken.Reset();
	m_score = 0;
}

////////////////////////////////////////////////////////////////////////////////
// UpdateTest

void SbMain::UpdateTest()
{
	if( m_gameState != m_nextState )
	{
		Reset();
		m_gameState = m_nextState;
	}

	if (m_isCameraMode)
	{
		m_camera.Update();
	}
	else
	{
		// Set view
		MtMatrix3 m3Rotation = m_kraken.GetRotation();
		MtMatrix3 m3HMDRotation;
		m3HMDRotation.SetIdentity();
		
		if( ShHMD::IsHMD() )
		{
			m3HMDRotation = MtMatrix3(ShHMD::GetQuaternion());
		}

		MtVector3 v3CameraPosition = m_kraken.GetPosition() + m3Rotation.Col2() / 2 + (m3Rotation.Col1() * EyeHeight);
		m_camera.GetCamera().SetPosition(v3CameraPosition);
		m_camera.GetCamera().SetRotation( m3Rotation.GetInverse() * m3HMDRotation );
		m_camera.GetCamera().Update();
	}

	if( UiKeyboard::pInstance()->IsPressed(ToggleViewKey) )
	{
		m_isCameraMode = !m_isCameraMode;
	}

	m_skybox.Update(m_camera.GetCamera().GetPosition());

	if( m_gameState == GameState_Playing )
	{
		if( UiKeyboard::pInstance()->IsPressed( UiKeyCode_P ) )
		{
			m_isPaused = !m_isPaused;
		}

		if( !m_isPaused )
		{
			m_kraken.Update();
			m_charges.Update();
			m_physics.Update();
			m_ships.Update();
			m_sea.Update();
			m_healthBar.Update();
		}
	}
	else
	{
		m_menu.Update();
	}

	for (BtU32 controller = 0; controller < MaxControllers; controller++)
	{
		if (Ui360::IsConnected(controller) == BtTrue)
		{
			if (Ui360::ButtonPressed(controller, Ui360_Back))
			{
				Reset();
			}
		}
	}

	if(UiKeyboard::pInstance()->IsPressed(UiKeyCode_ESCAPE))
	{
		SbMain::m_nextState = GameState_Start;
	}

	if (UiKeyboard::pInstance()->IsPressed(UiKeyCode_R))
	{
		Reset();
	}

//	if( ApConfig::IsDebugBuild() )
	{
		if (UiKeyboard::pInstance()->IsPressed(UiKeyCode_F1))
		{
			ApConfig::SetDebug(!ApConfig::IsDebug());
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// Update

void SbMain::Update()
{
	// Are we closing
	if( m_isClosing == BtTrue )
	{
		// Unload the archive
		m_gameArchive.Unload();

		// Unload the archive
		m_utilityArchive.Unload();

		// Unload the archive
		m_hmdArchive.Unload();
        
		// Read to close
		m_isClosed = BtTrue;
	}
	else
	{
		UpdateTest();
	}
}

//////////////////////////////////////////////////////////////////
// SetupRender

void SbMain::SetupRender()
{
	// Apply the shader
	m_pShader->Apply();

	// Make a new render target
	RsRenderTarget *pRenderTarget = RsUtil::GetNewRenderTarget();

	// Set the camera
	pRenderTarget->SetCamera( m_camera.GetCamera() );

	// Clear the render target
	pRenderTarget->SetCleared( BtTrue );

	// Clear the z buffer
	pRenderTarget->SetZCleared( BtTrue );

	// Set a good clear colour
	pRenderTarget->SetClearColour( RsColour( 0.5f, 0.6f, 0.7f, 1.0f ) );
	//pRenderTarget->SetClearColour( RsColour::WhiteColour() );

	// Apply this render target
	pRenderTarget->Apply();
}

//////////////////////////////////////////////////////////////////
// SetupRenderToTexture

void SbMain::SetupRenderToTexture( RsTexture *pTexture, RsCamera camera )
{
	// Apply the shader
	m_pShader->Apply();

	// Make a new render target
	RsRenderTarget *pRenderTarget = RsUtil::GetNewRenderTarget();

	// Set the texture
	pRenderTarget->SetTexture( pTexture );

	// Set the camera
	pRenderTarget->SetCamera( camera );

	// Clear the render target
	pRenderTarget->SetCleared( BtTrue );

	// Clear the z buffer
	pRenderTarget->SetZCleared( BtTrue );

	// Set a good clear colour
	pRenderTarget->SetClearColour( RsColour( 0, 0, 0, 1.0f ) );

	// Apply this render target
	pRenderTarget->Apply();
}

////////////////////////////////////////////////////////////////////////////////
// RestoreRenderTarget

void SbMain::RestoreRenderTarget()
{
	// Make a new render target
	RsRenderTarget *pRenderTarget = RsUtil::GetNewRenderTarget();

	// Set the camera
	pRenderTarget->SetCamera( m_camera.GetCamera() );

	// Clear the render target
	pRenderTarget->SetCleared( BtTrue );

	// Clear the z buffer
	pRenderTarget->SetZCleared( BtTrue );

	// Set a good clear colour
	pRenderTarget->SetClearColour( RsColour( 0.6f, 0.5f, 0.25, 1.0f ) );

	// Apply this render target
	pRenderTarget->Apply();
}

////////////////////////////////////////////////////////////////////////////////
// RenderScene

void SbMain::RenderScene()
{
	MtVector3 v3LightDirection(1.0f, 1.0f, 1.0f);
	v3LightDirection.Normalise();

	// Set the lights ambient level
	BtBool isVisible = SbKraken::GetInstance()->GetInstance()->IsVisible();
	if( isVisible )
	{
		// Set the light direction
		m_pShader->SetDirectionalLight(v3LightDirection);
		m_pShader->SetAmbient(RsColour(0.3f, 0.3f, 0.3f, 0));
	}
	else
	{
		BtFloat depth = SbKraken::GetInstance()->GetPosition().y;
		if( depth > 0 )
		{
			depth = 0;
		}
		BtFloat depthRatio = depth / -SeaDepth;
		BtFloat ambColour = MtLerp(depthRatio, 0.1f, 0.01f);
		m_pShader->SetAmbient(RsColour(ambColour, ambColour, ambColour, 0));

		BtFloat dirColour = MtLerp(depthRatio, 1.0f, 0.1f);
		v3LightDirection *= dirColour;
		m_pShader->SetDirectionalLight(v3LightDirection);
	}

	// Apply the shader
	m_pShader->Apply();

	if(m_gameState == GameState_Playing)
	{
		m_skybox.Render();
		m_model.Render();
		m_physics.Render();
		m_kraken.Render();
		m_ships.Render();
		m_sea.Render();
		m_charges.Render();
		m_healthBar.Render();
	}
	else
	{
		m_menu.Render();
	}
}

////////////////////////////////////////////////////////////////////////////////
// RenderTests

void SbMain::RenderTests()
{
	// Apply the shader
	m_pShader->Apply();

	// Empty the render targets at the start
	RsUtil::EmptyRenderTargets();
	
	if( ShHMD::IsHMD() )
	{
		BtFloat width = ShHMD::GetDimension().x;
		BtFloat height = ShHMD::GetDimension().y;
		RsCamera camera = m_camera.GetCamera();
		MtMatrix4 m4Temp;
		//camera.SetProjection(ShHMD::GetProjectionMatrix());

		MtVector2 v2Dimension = ShHMD::GetDimension();
		camera.SetDimension(v2Dimension);
		camera.SetViewport( RsViewport(0, 0, v2Dimension.x, v2Dimension.y ) );
		camera.SetAspect(1.0f);

		MtVector3 v3Position = camera.GetPosition();
		MtVector3 v3Offset = MtVector3(0, 0, 0); //camera.GetRotation().Col0() * 0.03f;

		camera.SetPosition(v3Position - v3Offset);
		camera.Update();

		RsTexture *pLeft  = m_hmdArchive.GetTexture( "ovrl" );
		RsTexture *pRight = m_hmdArchive.GetTexture( "ovrr" );

		SetupRenderToTexture( pLeft, camera );
		RenderScene();
	}
	else
	{
		RestoreRenderTarget();
		RenderScene();
	}

	if (ApConfig::IsDebug())
	{
		BtChar text[32];
		sprintf( text, "%.2f", RsUtil::GetFPS() );
		HlFont::Render( MtVector2( 0, 0 ), text, RsColour::WhiteColour(), MaxSortOrders-1 );
		/*
		BtFloat depth = SbKraken::GetInstance()->GetPosition().y + EyeHeight;
		BtU32 isVisible = SbKraken::GetInstance()->IsVisible();
		*/
	}
}

////////////////////////////////////////////////////////////////////////////////
// Render

void SbMain::Render()
{
	if( m_isClosing == BtTrue )
	{
		return;
	}

	// Render the tests
	RenderTests();

	// Render the mouse
	HlMouse::Render();
}

////////////////////////////////////////////////////////////////////////////////
// PreRender

void SbMain::PreRender()
{
}

////////////////////////////////////////////////////////////////////////////////
// PostRender

void SbMain::PostRender()
{
	// Render the mouse
	//m_renderMouse.Render();
}

////////////////////////////////////////////////////////////////////////////////
// Destroy

void SbMain::Destroy()
{
}
