////////////////////////////////////////////////////////////////////////////////
// SbMenu.cpp

// Includes
#include "BaArchive.h"
#include "MtVector2.h"
#include "RsColour.h"
#include "SbMenu.h"
#include "RsUtil.h"
#include "RsTexture.h"
#include "UiKeyboard.h"
#include "Ui360.h"
#include "ShTouch.h"
#include "SbMain.h"
#include "ShHMD.h"

////////////////////////////////////////////////////////////////////////////////
// Setup

void SbMenu::Setup(BaArchive *pArchive)
{
	m_pStart = pArchive->GetTexture("start");
	m_pWin = pArchive->GetTexture("win");
	m_pLose = pArchive->GetTexture("lose");
}

////////////////////////////////////////////////////////////////////////////////
// Update

void SbMenu::Update()
{
	if( UiKeyboard::pInstance()->IsPressed( UiKeyCode_SPACE ) )
	{
		SbMain::SetState( GameState_Playing );
	}

	for(BtU32 controller = 0; controller < MaxControllers; controller++)
	{
		if(Ui360::IsConnected(controller) == BtTrue)
		{
			if(Ui360::ButtonPressed(controller, Ui360_Start))
			{
				SbMain::SetState( GameState_Playing );
			}
		}
	}

	if( ShTouch::GetNumberOfTouches() > 0 )
	{
		//SbMain::SetState( GameState_Playing );
	}
}

////////////////////////////////////////////////////////////////////////////////
// Render

void SbMenu::Render()
{
	MtVector2 v2Dimension = RsUtil::GetDimension();

	if( ShHMD::IsHMD() )
	{
		v2Dimension = ShHMD::GetDimension();
	}

	if( SbMain::GetState() == GameState_Start )
	{
		m_pStart->Render( MtVector2( 0, 0 ), v2Dimension, RsColour::WhiteColour(), MaxSortOrders - 1 );
	}
	if(SbMain::GetState() == GameState_Win )
	{
		m_pWin->Render(MtVector2(0, 0), v2Dimension, RsColour::WhiteColour(), MaxSortOrders - 1);
	}
	if(SbMain::GetState() == GameState_Lose )
	{
		m_pLose->Render(MtVector2(0, 0), v2Dimension, RsColour::WhiteColour(), MaxSortOrders - 1);
	}
}	
