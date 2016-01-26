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
#include "DyImpl.h"
#include "ShTouch.h"

//const BtBool useRenderTarget = BtFalse;
//const BtBool useRenderTarget = BtTrue;

////////////////////////////////////////////////////////////////////////////////
// Init

void SbMain::Init()
{
	m_isClosing = BtFalse;
	m_isClosed  = BtFalse;

	ApConfig::SetTitle( "GameJamTemplate" );

    // Setup the camera
    MtVector2 v2Dimension = RsUtil::GetDimension();
    m_camera.SetDimension(MtVector2(v2Dimension.x, v2Dimension.y));
    m_camera.Init();
    //m_camera.SetPosition( MtVector3( 0, 2.0f, -5.0f ) );
    m_camera.SetPosition(MtVector3(0, 0, 0));
    m_camera.SetSpeed(10.0f);
    
	ApConfig::SetDebug( BtFalse );

	RdRandom::SetRandomSeed();

	// Setup the resource paths
    if( ApConfig::GetPlatform() == ApPlatform_WinGL )
    {
        ApConfig::SetResourcePath( "..\\GameJamTemplate\\release\\" );
        ApConfig::SetExtension(".winglz");
        ApConfig::CheckResourcePath( "game" );
    }
}

////////////////////////////////////////////////////////////////////////////////
// Create

void SbMain::Create()
{
	m_gameArchive.Load( "game" );
	m_utilityArchive.Load("utility");
    
	m_pWhite2 = m_utilityArchive.GetMaterial( "white2" );
	m_pWhite3 = m_utilityArchive.GetMaterial( "white3" );
	m_pShader = m_gameArchive.GetShader( "shader" );
	 
	m_skybox.Setup( &m_gameArchive );
	m_model.Setup( &m_gameArchive, BtNull);
	m_physics.Setup(&m_gameArchive);
	m_smoke.Setup(&m_gameArchive);
	m_menu.Setup();
	m_menu.SetRenderDimension( MtVector2(1024.0f, 1024.0f));

	RsMaterial *pMaterial2 = m_utilityArchive.GetMaterial("white2");
	RsMaterial *pMaterial3 = m_utilityArchive.GetMaterial("white3");
	RsMaterial *pMaterial3notest = m_utilityArchive.GetMaterial("white3noztest");
	m_pGUIRenderTarget = m_gameArchive.GetMaterial( "guirendertarget" );
	m_pGUIRenderTarget->SetTechniqueName( "RsShaderT" );

	HlFont::Setup( &m_utilityArchive, "vera" );
	HlMouse::Setup( &m_utilityArchive );
	HlDraw::Setup( pMaterial2, pMaterial3, pMaterial3notest );

	m_pSound = m_gameArchive.GetSound( "howl" );
}

////////////////////////////////////////////////////////////////////////////////
// Reset

void SbMain::Reset()
{
	m_smoke.SetEmitter(BtTrue);
}

////////////////////////////////////////////////////////////////////////////////
// UpdateTest

void SbMain::UpdateTest()
{
	m_camera.Update();
	m_skybox.Update( m_camera.GetCamera().GetPosition() );
	m_model.Update();
	m_physics.Update();
	m_smoke.Update();
	m_menu.Update();

	// Reset the physics
	if( m_menu.GetCurrentItem()->m_isPressed )
	{
		m_physics.Reset();
	}

	if(UiKeyboard::pInstance()->IsPressed(DebugKey))
	{
		ApConfig::SetDebug(!ApConfig::IsDebug());
	}

	if (UiKeyboard::pInstance()->IsPressed(PauseKey))
	{
		ApConfig::SetPaused( !ApConfig::IsPaused() );
	}
	
	if(UiKeyboard::pInstance()->IsPressed( CloseGameKey ))
	{
		m_isClosing = BtTrue;
	}

	if( UiKeyboard::pInstance()->IsPressed( UiKeyCode_Y ) )
	{
		m_pSound->Play();
	}

	m_lockMask = BtTrue;

	for(BtU32 i = 0; i < MaxTouches; i++)
	{
		if( ShTouch::IsHeld(i) )
		{
			m_lockMask = BtFalse;
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
	//pRenderTarget->SetClearColour( RsColour( 0.5f, 0.6f, 0.7f, 1.0f ) );
	pRenderTarget->SetClearColour( RsColour::NoColour() );

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
	pRenderTarget->SetClearColour( RsColour::NoColour() );

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
	pRenderTarget->SetClearColour( RsColour::NoColour() );

	// Apply this render target
	pRenderTarget->Apply();
}

////////////////////////////////////////////////////////////////////////////////
// DrawRenderTarget

void SbMain::DrawRenderTarget()
{
	/*
	if( ApConfig::GetPlatform() != ApPlatform_DX11 )
	{
		HlMaterial::RenderUpsideDown( m_pRenderTarget, MtVector2( 0, 0 ), MtVector2( 256.0f, 256.0f ) );
	}
	else
	{
		HlMaterial::RenderRightWayUp( m_pRenderTarget, MtVector2( 0, 0 ), MtVector2( 256.0f, 256.0f ) );
	}
	*/
}

////////////////////////////////////////////////////////////////////////////////
// RenderIntoHMD

void SbMain::RenderIntoHMD( RsMaterial *pMaterial,
                            const MtVector2 &v2Position,
                            MtVector2 &v2Dimension,
                            const MtVector2 &v2ScreenDimension,
                            BtU32 sortOrder
                          )
{
    RsVertex3 vertex[4];
    RsVertex3 *pVertex = &vertex[0];
    
    // Calculate the positions
    BtFloat xmax = v2Dimension.x;
    BtFloat ymax = v2Dimension.y;
    
    RsVertex3 *pQuad = pVertex;
    
    // Copy these into vertex
    pVertex->m_v3Position = MtVector3(v2Position.x, v2Position.y, 0.1f);
    pVertex->m_v2UV = MtVector2(1, 0);
    pVertex->m_colour = 0xFFFFFFFF;
    ++pVertex;
    
    pVertex->m_v3Position = MtVector3(v2Position.x, v2Position.y + ymax, 0.1f);
    pVertex->m_v2UV = MtVector2(1, 1);
    pVertex->m_colour = 0xFFFFFFFF;
    ++pVertex;
    
    pVertex->m_v3Position = MtVector3(v2Position.x + xmax, v2Position.y + 0, 0.1f);
    pVertex->m_v2UV = MtVector2(0, 0);
    pVertex->m_colour = 0xFFFFFFFF;
    ++pVertex;
    
    pVertex->m_v3Position = MtVector3(v2Position.x + xmax, v2Position.y + ymax, 0.1f);
    pVertex->m_v2UV = MtVector2(0, 1);
    pVertex->m_colour = 0xFFFFFFFF;
    ++pVertex;
    
    // Flip the V's
    pQuad[0].m_v2UV.y = 1.0f - pQuad[0].m_v2UV.y;
    pQuad[1].m_v2UV.y = 1.0f - pQuad[1].m_v2UV.y;
    pQuad[2].m_v2UV.y = 1.0f - pQuad[2].m_v2UV.y;
    pQuad[3].m_v2UV.y = 1.0f - pQuad[3].m_v2UV.y;
    
    // Cache the display width and height
    MtVector2 v2TargetDimension = RsRenderTarget::GetCurrent()->GetCameraDimension();
    BtFloat fScaleWidth = 1.0f / v2TargetDimension.x;
    BtFloat fScaleHeight = 1.0f / v2TargetDimension.y;
    
    // Scale the position to local screen space -1 to 1
    for(BtU32 i = 0; i < 4; i++)
    {
        // Flip the y
        pQuad[i].m_v3Position.y = v2TargetDimension.y - pQuad[i].m_v3Position.y;
        
        // Scale from 0..width to 0..1
        pQuad[i].m_v3Position.x *= fScaleWidth;
        pQuad[i].m_v3Position.y *= fScaleHeight;
        
        // Scale from 0..1 to 0..2
        pQuad[i].m_v3Position.x *= 2.0f;
        pQuad[i].m_v3Position.y *= 2.0f;
        
        // Translate from 0..2 to -1..1
        pQuad[i].m_v3Position.x -= 1.0f;
        pQuad[i].m_v3Position.y -= 1.0f;
    }
    
    pMaterial->Render(RsPT_TriangleStrip, pQuad, 4, sortOrder );
}


////////////////////////////////////////////////////////////////////////////////
// Render2DInto3D

void SbMain::Render2DInto3D( RsCamera &camera )
{
	// This code perfectly frames a billboard in front of the camera
	BtFloat completeFieldOfView = camera.GetFieldOfView();
    MtMatrix3 m3Orientation = camera.GetRotation().GetInverse();
	MtVector3 v3AxisXScaled = m3Orientation.XAxis() * 0.5f;
	MtVector3 v3AxisYScaled = m3Orientation.YAxis() * 0.5f;
	MtVector3 v3AxisZ       = m3Orientation.ZAxis();
	
	BtFloat fieldOfView = completeFieldOfView * 0.5f;
	BtFloat tanAngle = MtTan(fieldOfView);
	BtFloat distance = 0.5f / tanAngle;
  
	MtVector3 v3Position = camera.GetPosition() + ( v3AxisZ * distance );
	//

	m_lockMask = BtFalse;

	if( m_lockMask == BtFalse )
	{
		m_v3Vertex[0].m_v3Position.x = v3Position.x - v3AxisXScaled.x - v3AxisYScaled.x;
		m_v3Vertex[0].m_v3Position.y = v3Position.y - v3AxisXScaled.y - v3AxisYScaled.y;
		m_v3Vertex[0].m_v3Position.z = v3Position.z - v3AxisXScaled.z - v3AxisYScaled.z;
		m_v3Vertex[0].m_v2UV.x = 0;
		m_v3Vertex[0].m_v2UV.y = 0;

		m_v3Vertex[1].m_v3Position.x = v3Position.x + v3AxisXScaled.x - v3AxisYScaled.x;
		m_v3Vertex[1].m_v3Position.y = v3Position.y + v3AxisXScaled.y - v3AxisYScaled.y;
		m_v3Vertex[1].m_v3Position.z = v3Position.z + v3AxisXScaled.z - v3AxisYScaled.z;
		m_v3Vertex[1].m_v2UV.x = 1;
		m_v3Vertex[1].m_v2UV.y = 0;

		m_v3Vertex[2].m_v3Position.x = v3Position.x - v3AxisXScaled.x + v3AxisYScaled.x;
		m_v3Vertex[2].m_v3Position.y = v3Position.y - v3AxisXScaled.y + v3AxisYScaled.y;
		m_v3Vertex[2].m_v3Position.z = v3Position.z - v3AxisXScaled.z + v3AxisYScaled.z;
		m_v3Vertex[2].m_v2UV.x = 0;
		m_v3Vertex[2].m_v2UV.y = 1;

		// Second triangle
		m_v3Vertex[3].m_v3Position.x = v3Position.x - v3AxisXScaled.x + v3AxisYScaled.x;
		m_v3Vertex[3].m_v3Position.y = v3Position.y - v3AxisXScaled.y + v3AxisYScaled.y;
		m_v3Vertex[3].m_v3Position.z = v3Position.z - v3AxisXScaled.z + v3AxisYScaled.z;
		m_v3Vertex[3].m_v2UV.x = 0;
		m_v3Vertex[3].m_v2UV.y = 1;

		m_v3Vertex[4].m_v3Position.x = v3Position.x + v3AxisXScaled.x - v3AxisYScaled.x;
		m_v3Vertex[4].m_v3Position.y = v3Position.y + v3AxisXScaled.y - v3AxisYScaled.y;
		m_v3Vertex[4].m_v3Position.z = v3Position.z + v3AxisXScaled.z - v3AxisYScaled.z;
		m_v3Vertex[4].m_v2UV.x = 1;
		m_v3Vertex[4].m_v2UV.y = 0;

		m_v3Vertex[5].m_v3Position.x = v3Position.x + v3AxisXScaled.x + v3AxisYScaled.x;
		m_v3Vertex[5].m_v3Position.y = v3Position.y + v3AxisXScaled.y + v3AxisYScaled.y;
		m_v3Vertex[5].m_v3Position.z = v3Position.z + v3AxisXScaled.z + v3AxisYScaled.z;
		m_v3Vertex[5].m_v2UV.x = 1;
		m_v3Vertex[5].m_v2UV.y = 1;

		for(BtU32 i = 0; i < 6; i++)
		{
			m_v3Vertex[i].m_colour = RsColour::WhiteColour().asWord();
			m_v3Vertex[i].m_v3Normal = MtVector3(0, 1, 0);
		}
	}
	m_pGUIRenderTarget->Render(RsPT_TriangleList, m_v3Vertex, 6, MaxSortOrders - 1, BtTrue);
}

////////////////////////////////////////////////////////////////////////////////
// Render2DScene

void SbMain::Render2DScene()
{
    RsMaterial *pMaterial = m_utilityArchive.GetMaterial("white2");
    pMaterial->SetTechniqueName( "RsShaderT2" );
   // HlMaterial::RenderRightWayUp( pMaterial, MtVector2(0, 0), MtVector2(1024.0f, 200.0f), MtVector2(1024.0f, 1024.0f) );
	m_menu.Render();
}

////////////////////////////////////////////////////////////////////////////////
// Render3DScene

void SbMain::Render3DScene()
{
	m_model.Render();
	m_physics.Render();
	m_smoke.Render();
	//m_font.Render();
}

////////////////////////////////////////////////////////////////////////////////
// RenderTests

void SbMain::RenderTests()
{
	// Apply the shader
	m_pShader->Apply();

	// Empty the render targets at the start
	RsUtil::EmptyRenderTargets();
	
    RsCamera camera;
    camera.SetProjection(MtMatrix4::m_identity);
    camera.SetDimension(MtVector2(1024.0f, 1024.0f));
    camera.SetViewport(RsViewport(0, 0, 1024, 1024));
    camera.SetAspect(1.0f);
    camera.Update();
    SetupRenderToTexture( m_pGUIRenderTarget->GetTexture(0), camera );
    Render2DScene();
    
    RestoreRenderTarget();
    m_skybox.Render();
    Render3DScene();
    Render2DInto3D( m_camera.GetCamera() );
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
