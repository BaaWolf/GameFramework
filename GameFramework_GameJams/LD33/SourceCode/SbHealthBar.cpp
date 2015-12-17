////////////////////////////////////////////////////////////////////////////////
// SbHealthBar.cpp

// Includes
#include "BaArchive.h"
#include "SbHealthBar.h"
#include "MtVector2.h"
#include "RsColour.h"
#include "RsMaterial.h"
#include "RsRenderTarget.h"
#include "RsUtil.h"
#include "SbKraken.h"
#include "HlDraw.h"
#include "SbShips.h"
#include "SbMain.h"

////////////////////////////////////////////////////////////////////////////////
// Setup

void SbHealthBar::Setup( BaArchive *pArchive )
{
	m_pWhite = pArchive->GetMaterial( "white2" );
	m_pShip  = pArchive->GetMaterial( "ship" );
}

////////////////////////////////////////////////////////////////////////////////
// Update

void SbHealthBar::Update()
{
}

////////////////////////////////////////////////////////////////////////////////
// Render

void SbHealthBar::Render()
{
	BtFloat height = (BtFloat)RsUtil::GetHeight();
	BtFloat width = (BtFloat)RsUtil::GetWidth();

	BtFloat hudHeight = height / 10.0f;
	BtFloat hudWidth  = width  / 5.0f;
	
	BtFloat borderX = hudWidth  / 10.0f;
	BtFloat borderY = hudHeight / 10.0f;

	BtFloat healthHeight = ( hudHeight - borderY - borderY ) / 3.0f;

	m_pWhite->Render( MtVector2( 0, 0 ),
					  MtVector2( hudWidth, hudHeight ),
					  RsColour( 1.0f, 1.0f, 1.0f, 0.25f ),
					  MaxSortOrders-1 );

	BtFloat maxWidth = hudWidth - borderX - borderX;
	m_pWhite->Render( MtVector2(borderX, borderY),
				      MtVector2(maxWidth, healthHeight ),
					  RsColour(1.0f, 1.0f, 1.0f, 0.25f),
					  MaxSortOrders - 1);

	// Render the health
	BtFloat health = maxWidth / 100.0f * SbKraken::GetInstance()->GetHealth();
	m_pWhite->Render( MtVector2(borderX, borderY),
				      MtVector2(health, healthHeight),
				      RsColour(0.6f, 1.0f, 0.4f, 0.5f),
					  MaxSortOrders - 1);

	// Render the score
	
	BtFloat shipWidth = maxWidth / NumShips / 2;
	MtVector2 v2Pos( borderX + shipWidth / 2.0f, healthHeight + healthHeight + borderY );

	for( BtU32 i=0; i<NumShips; i++ )
	{
		RsColour colour = RsColour::WhiteColour();
		if( i >= SbMain::GetScore() )
		{
			colour = RsColour( 0, 0, 0, 0.25f );
		}

		m_pShip->Render( v2Pos,
						 MtVector2(shipWidth, shipWidth),
						 colour,
						 MaxSortOrders - 1);
		v2Pos.x += shipWidth * 2.0f;
	}
}
