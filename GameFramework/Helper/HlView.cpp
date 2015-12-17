////////////////////////////////////////////////////////////////////////////////
// HlView.cp

// Includes
#include "HlView.h"
#include "BtTime.h"
#include "BtString.h"
#include "RsUtil.h"
#include "ApConfig.h"
#include "RsTexture.h"
#include "RsMaterial.h"
#include "HlTimer.h"
#include "BaArchive.h"
#include "ErrorLog.h"
#include "FsFile.h"
#include "BaUserData.h"
#include "TinyXML2.h"
#include "RsSprite.h"
#include "MtVector2.h"
#include "HlScreenSize.h"
#include "ShJoystick.h"
#include "ShTouch.h"
#include "UiKeyboard.h"
#include "RsRenderTarget.h"
#include "HlKeyboard.h"
#include "HlUserData.h"

// statics
BaArchive HlView::m_archive;
BtBool    HlView::m_isLoaded;
BtChar	  HlView::m_lastArchiveName[32];
BtArray<HlMenuItems, 64> HlView::m_items;
BtU32     HlView::m_alignment;
HlView *HlView::m_pLast = BtNull;
HlMenuItems *HlView::m_pCurrentMenuItem = BtNull;
HlMenuItems *HlView::m_pCurrentMenuItemSelected = BtNull;

////////////////////////////////////////////////////////////////////////////////
// Setup

void HlView::Setup( const BtChar *archiveName,  const BtChar *screenName, BtBool isBackground )
{
	BtStrCopy( m_archiveName, 32, archiveName );
	BtStrCopy( m_screenName,  32, screenName );
	BtStrCopy( m_lastArchiveName, 32, "" );
    m_isLoaded = BtFalse;

	// Set whether we want to render a background
	m_isBackground = isBackground;

	// Null the current menu item
	m_pCurrentMenuItem = BtNull;

	// Used to decide what size we are rendering to
	m_v2RenderDimension = RsUtil::GetDimension();

	m_backgroundSortOrder = MaxSortOrders-1;
}

////////////////////////////////////////////////////////////////////////////////
// GetNumItems

BtU32 HlView::GetNumItems()
{
	return m_items.GetNumItems();
}

////////////////////////////////////////////////////////////////////////////////
// SetBackground

void HlView::SetBackground(BtBool isBackground)
{
	m_isBackground = isBackground;
}

////////////////////////////////////////////////////////////////////////////////
// IsEqual

BtBool HlView::IsEqual( const BtChar *string )
{
	if( strcmp( m_pCurrentMenuItemSelected->m_id, string ) == 0 )
	{
		return BtTrue;
	}
	return BtFalse;
}

////////////////////////////////////////////////////////////////////////////////
// LoadMenu

void HlView::LoadMenu()
{
	// Empty the items
	m_items.Empty();

	m_pCurrentMenuItemSelected = BtNull;
	m_pCurrentMenuItem = BtNull;

	tinyxml2::XMLNode *pNode;
	tinyxml2::XMLDocument doc;
	BtBool isParsed = HlUserData::GetXML(doc, &m_archive, m_screenName );
	if(isParsed)
	{
       tinyxml2::XMLNode *pNode = doc.FirstChildElement( "MENU" );

		// Is there a child?
		while( pNode )
		{
			tinyxml2::XMLNode *pItem = pNode->FirstChildElement( "ITEM" );
	
			while( pItem )
			{
				HlMenuItems item;

				item.m_alignment = 0;

				BtStrCopy( item.m_id, 64, "unset_id" );

				// Set the default sort order
				item.m_sortOrder = MaxSortOrders-1;

				// Set the default fade
				item.m_fade = 1.0f;

				// We model menu items with a specific resolution in mind. Set this here.
				// Eventually we can pull this from the XML

				item.m_v2OriginalScreenSize = MtVector2( 800.0f, 600.0f );

				// The default for each menu item is that it doesn't matter if it's a joystick or not
				item.m_joystick = HlMenuJoystick_Any;

				item.m_joystickButtonID = JoystickButton_MAX;

				// Find the fade
				tinyxml2::XMLElement *pFade = pItem->FirstChildElement( "FADE" );
				if( pFade )
				{
					const BtChar *pFadeString = pFade->GetText();
					item.m_fade = atof( pFadeString );
				}

				// Get the joystick
				tinyxml2::XMLElement *pJoystickButton = pItem->FirstChildElement( "JOYSTICKBUTTON" );
				if( pJoystickButton )
				{
					const BtChar *pJoystickButtonString = pJoystickButton->GetText();
					if( BtStrCompareNoCase( pJoystickButtonString, "PAUSE" ) )
					{
						item.m_joystickButtonID = JoystickButton_Pause;
					}
					if( BtStrCompareNoCase( pJoystickButtonString, "A" ) )
					{
						item.m_joystickButtonID = JoystickButton_A;
					}
					else if( BtStrCompareNoCase( pJoystickButtonString, "B" ) )
					{
						item.m_joystickButtonID = JoystickButton_B;
					}
					else if( BtStrCompareNoCase( pJoystickButtonString, "Y" ) )
					{
						item.m_joystickButtonID = JoystickButton_Y;
					}
					else if( BtStrCompareNoCase( pJoystickButtonString, "X" ) )
					{
						item.m_joystickButtonID = JoystickButton_X;
					}
                    else if( BtStrCompareNoCase( pJoystickButtonString, "LEFTSHOULDER" ) )
					{
						item.m_joystickButtonID = JoystickButton_LeftShoulder;
					}
                    else if( BtStrCompareNoCase( pJoystickButtonString, "RIGHTSHOULDER" ) )
					{
						item.m_joystickButtonID = JoystickButton_RightShoulder;
					}
                    else if( BtStrCompareNoCase( pJoystickButtonString, "DPADLEFT" ) )
					{
						item.m_joystickButtonID = JoystickButton_Left;
					}
                    else if( BtStrCompareNoCase( pJoystickButtonString, "DPADRIGHT" ) )
					{
						item.m_joystickButtonID = JoystickButton_Right;
					}
				}

				// Get the screen width
				tinyxml2::XMLElement *pScreenWidth = pItem->FirstChildElement( "SCREENWIDTH" );
				if( pScreenWidth )
				{
					const BtChar *pScreenWidthString = pScreenWidth->GetText();
					item.m_v2OriginalScreenSize.x = atoi( pScreenWidthString );
				}
		
				// Get the screen height
				tinyxml2::XMLElement *pScreenHeight = pItem->FirstChildElement( "SCREENHEIGHT" );
				if( pScreenHeight )
				{
					const BtChar *pScreenWidthString = pScreenHeight->GetText();
					item.m_v2OriginalScreenSize.y = atoi( pScreenWidthString );
				}

				// Get the sprite name
				const BtChar *pSpriteName = pItem->FirstChildElement( "SPRITE" )->GetText();
				item.m_pSprite = m_archive.GetSprite( pSpriteName );

				// Get the index
                tinyxml2::XMLElement *pElement = pItem->FirstChildElement( "SORTORDER" );
                if( pElement )
                {
                    const BtChar *sortOrder = pElement->GetText();
                    if( sortOrder )
                    {
                        item.m_sortOrder = atoi(sortOrder);
                    }
                }

				// Get the index 
				GetIntFromXML( pItem, "INDEX", item.m_index );

				// Get the selected index
				GetIntFromXML( pItem, "SELECTEDINDEX", item.m_selectedIndex );

				// Get the background sort order
				GetUIntFromXML( pItem, "BACKGROUNDSORTORDER", m_backgroundSortOrder );

				// Find the sprite dimension
				item.m_v2OriginalDimension = item.m_pSprite->GetDimension( item.m_index );
							
				// Get the width
                GetFloatFromXML( pItem, "WIDTH", item.m_v2OriginalDimension.x );

				// Get the height
                pElement = pItem->FirstChildElement( "HEIGHT" );
                if( pElement )
                {
                    const BtChar *heightstring = pItem->FirstChildElement( "HEIGHT" )->GetText();

                    if( heightstring )
                    {
                        item.m_v2OriginalDimension.y = atoi( heightstring );
                    }
                }
				
				// Get the x 
				const BtChar *xstring = pItem->FirstChildElement( "X" )->GetText();

				// Get the y 
				const BtChar *ystring = pItem->FirstChildElement( "Y" )->GetText();

				BtFloat x, y;

				if( atof( xstring ) == 0 )
				{
					if( BtStrCompareNoCase( xstring, "c" ) )
					{
						item.m_alignment |= HlMenuItemAlignment_HorizontalCentre;
						x = (item.m_v2OriginalScreenSize.x * 0.5f) - (item.m_v2OriginalDimension.x * 0.5f);
					}
					else if( BtStrCompareNoCase( xstring, "l" ) )
					{
						x = 0;
						item.m_alignment |= HlMenuItemAlignment_Left;
					}
					else if( BtStrCompareNoCase( xstring, "r" ) )
					{
						item.m_alignment |= HlMenuItemAlignment_Right;
						x = item.m_v2OriginalScreenSize.x - item.m_v2OriginalDimension.x;
					}
                    else
                    {
                        x = 0;
                    }
				}
				else
				{
					x = atof(xstring);
				}
		
				if (atof(ystring) == 0)
				{
                	if( BtStrCompareNoCase( ystring, "c" ) )
					{
						item.m_alignment |= HlMenuItemAlignment_VerticalCentre;
						y = (item.m_v2OriginalScreenSize.y * 0.5f) - (item.m_v2OriginalDimension.y * 0.5f);
					}
					else if ( BtStrCompareNoCase( ystring, "t") )
					{
						y = 0;
						item.m_alignment |= HlMenuItemAlignment_Top;
					}
					else if ( BtStrCompareNoCase( ystring, "b") )
					{
						y = item.m_v2OriginalScreenSize.y - item.m_v2OriginalDimension.y;
						item.m_alignment |= HlMenuItemAlignment_Bottom;
					}
                    else
                    {
                        y = 0;
                    }
				}
				else
				{
					y = atof(ystring);
				}

				// Set the final position
				item.m_v2OriginalPosition = MtVector2( x, y );

				// Refactor the position
				item.m_v2Position  = HlScreenSize::Refactor( item.m_v2OriginalScreenSize, item.m_v2OriginalPosition );

				// Refactor the size
				item.m_v2Dimension = HlScreenSize::Refactor( item.m_v2OriginalScreenSize, item.m_v2OriginalDimension );

				// Get the ID
				const BtChar *pID = pItem->FirstChildElement( "ID" )->GetText();
				BtStrCopy( item.m_id, 64, pID );

				item.m_isVisible = BtTrue;
				tinyxml2::XMLElement *pIsVisible = pItem->FirstChildElement( "VISIBLE" );
				if( pIsVisible )
				{
					const BtChar *pIsVisibleText = pIsVisible->GetText();

					if( BtStrCompareNoCase( pIsVisibleText, "TRUE" ) )
					{
						item.m_isVisible = BtTrue;
					}
					else if( BtStrCompareNoCase( pIsVisibleText, "FALSE" ) )
					{
						item.m_isVisible = BtFalse;
					}
				}

				// Get the fade when selected
				tinyxml2::XMLElement *pFadeWhenSelected = pItem->FirstChildElement( "FADEWHENSELECTED" );
				if( pFadeWhenSelected )
				{
					const BtChar *pFadeWhenSelectedString = pFadeWhenSelected->GetText();
					item.m_fadeWhenSelected = atof( pFadeWhenSelectedString );
				}
				else
				{
					item.m_fadeWhenSelected = 0.5f;
				}

				// Is this menu item selectable
				item.m_isSelectable = BtFalse;
				tinyxml2::XMLElement *pIsSlectable = pItem->FirstChildElement( "SELECTABLE" );
				if( pIsSlectable )
				{
					const BtChar *pIsSlectableText = pIsSlectable->GetText();

					if( BtStrCompareNoCase( pIsSlectableText, "TRUE" ) )
					{
						item.m_isSelectable = BtTrue;
					}
				}

				// Get whether this is joystick only
				tinyxml2::XMLElement *pJoystickOnly = pItem->FirstChildElement( "JOYSTICK" );
				if( pJoystickOnly )
				{
					const BtChar *pJoystickOnlyText = pJoystickOnly->GetText();

					if( BtStrCompareNoCase( pJoystickOnlyText, "TRUE" ) )
					{
						item.m_joystick = HlMenuJoystick_Yes;
					}
					else
					{
						item.m_joystick = HlMenuJoystick_No;
					}
				}

				// Add each item
				m_items.Add( item );

				// Move to the next child
				pItem = pItem->NextSiblingElement();
			}

			// Move to the next child
			pNode = pNode->NextSiblingElement();
		}
	}
	
	int a=0;
	a++;
}

////////////////////////////////////////////////////////////////////////////////
// UpdateMenu

void HlView::UpdateMenu()
{
	// Re-load the menu
    if( ApConfig::IsWin() && UiKeyboard::pInstance()->IsPressed( UiKeyCode_F2 ) )
	{
		int a=0;
		a++;
	}

	m_pCurrentMenuItem = BtNull;

	BtU32 numItems = m_items.GetNumItems();

	for (BtU32 i = 0; i < numItems; i++)
	{
		// Cache each item
		HlMenuItems &item = m_items[i];

		item.m_isPressed = BtFalse;
		item.m_isReleased = BtFalse;
		item.m_isHeld = BtFalse;
	}

	// Refactor these for the current viewport
	for(BtU32 i = 0; i < numItems; i++)
	{
		// Cache each item
		HlMenuItems &item = m_items[i];

		// Refactor the position
		item.m_v2Position = HlScreenSize::Refactor(item.m_v2OriginalScreenSize, item.m_v2OriginalPosition, m_v2RenderDimension );

		// Refactor the size
		item.m_v2Dimension = HlScreenSize::Refactor(item.m_v2OriginalScreenSize, item.m_v2OriginalDimension, m_v2RenderDimension );
	}

	for (BtU32 touch = 0; touch < MaxTouches; touch++)
	{
		MtVector2 v2MousePosition = ShTouch::GetPosition( touch );
		v2MousePosition.x = v2MousePosition.x / RsUtil::GetDimension().x * m_v2RenderDimension.x;
		v2MousePosition.y = v2MousePosition.y / RsUtil::GetDimension().y * m_v2RenderDimension.y;

		BtU32 numItems = m_items.GetNumItems();

		for (BtU32 i = 0; i < numItems; i++)
		{
			// Cache each item
			HlMenuItems &item = m_items[i];
	
			if( item.m_joystickButtonID != JoystickButton_MAX )
			{
				if( ShJoystick::IsReleased( item.m_joystickButtonID ) )
				{
					item.m_isReleased = BtTrue;
					m_pCurrentMenuItem = &item;
				}
				if( ShJoystick::IsHeld( item.m_joystickButtonID ) )
				{
					item.m_isHeld = BtTrue;
				}
				if( ShJoystick::IsPressed( item.m_joystickButtonID ) )
				{
					item.m_isPressed = BtTrue;
				}
			}

			if( item.m_isSelectable )
			{
				if( v2MousePosition.x > item.m_v2Position.x )
				{
					if( v2MousePosition.x < item.m_v2Position.x + item.m_v2Dimension.x )
					{
						if( v2MousePosition.y > item.m_v2Position.y )
						{
							if( v2MousePosition.y < item.m_v2Position.y + item.m_v2Dimension.y )
							{
								if( ShTouch::IsPressed( touch ) )
								{
									m_pCurrentMenuItemSelected = &item;
									item.m_isPressed = BtTrue;
								}
								if( ShTouch::IsHeld( touch ) )
								{
									if( &item == m_pCurrentMenuItemSelected )
									{
										m_pCurrentMenuItemSelected = &item;
										item.m_isHeld = BtTrue;
									}
								}
								if( ShTouch::IsReleased( touch ) )
								{
									if( &item == m_pCurrentMenuItemSelected )
									{
										m_pCurrentMenuItem = m_pCurrentMenuItemSelected;
										item.m_isReleased = BtTrue;
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// Load

void HlView::Load()
{
	Unload();

	// Load the archive and flag the archive as loaded
	m_archive.Load( m_archiveName );
    
	// Flag the archive as loaded
	m_isLoaded = BtTrue;

	// Make a note of the archive name
    BtStrCopy( m_lastArchiveName, 32, m_archiveName );

	// Load the menu now we have the archive
	LoadMenu();

	if( m_isBackground )
	{
		// The pointer to the material needs to be cached at runtime rather than at setup
		// Get the material which has the same name as the archive for simplicity
		m_pBackgroundMaterial = m_archive.GetMaterial( m_screenName );
	}
}

////////////////////////////////////////////////////////////////////////////////
// Update

void HlView::Update()
{
	if( BtStrCompare( m_lastArchiveName, m_archiveName ) == BtFalse )
	{
		if( m_pLast )
		{
			m_pLast->OnExit();
		}

		Unload();
		Load();
		OnEntry();
	}
 
	// Update the menu
	UpdateMenu();

	m_pLast = this;
}

////////////////////////////////////////////////////////////////////////////////
// OnEntry

void HlView::OnEntry()
{
	int a=0;
	a++;
}

////////////////////////////////////////////////////////////////////////////////
// OnExit

void HlView::OnExit()
{
	int a = 0;
	a++;
}

////////////////////////////////////////////////////////////////////////////////
// Unload

void HlView::Unload()
{
	if( m_isLoaded )
	{	
		// Unload the menu items
		m_items.Empty();

		// Unload the archive
		m_archive.Unload();

		// Unload the archive
		m_isLoaded = BtFalse;
	}	
}

////////////////////////////////////////////////////////////////////////////////
// FindItem

HlMenuItems *HlView::FindItem( const BtChar *name )
{
	BtU32 numItems = m_items.GetNumItems();

	for( BtU32 i=0; i<numItems; i++ )
	{
		// Cache each item
		HlMenuItems &item = m_items[i];

		if( BtStrCompareNoCase( item.m_id, name ) )
		{
			return &item;
		}
	}
	return BtNull;
}

////////////////////////////////////////////////////////////////////////////////
// FindItem

HlMenuItems *HlView::FindItem( BtU32 id )
{
	// Cache each item
	HlMenuItems &item = m_items[id];

	return &item;
}

////////////////////////////////////////////////////////////////////////////////
// RenderMenu

void HlView::RenderMenu()
{
	BtU32 numItems = m_items.GetNumItems();

	for( BtU32 i=0; i<numItems; i++ )
	{
		// Cache each item
		HlMenuItems &item = m_items[i];

		if( item.m_isVisible )
		{
			// Setup the colour
			RsColour colour( RsColour::WhiteColour() );
			colour.Alpha( item.m_fade );

			// Get the index of the sprite
			BtU32 index = item.m_index;

			if( item.m_isHeld == BtTrue )
			{
				if( item.m_selectedIndex != -1 )
				{
					index = item.m_selectedIndex;
				}
				else
				{
					colour = RsColour( item.m_fadeWhenSelected, item.m_fadeWhenSelected, item.m_fadeWhenSelected, 1.0f );
				}
			}

			if( ShJoystick::IsConnected() == BtTrue )
			{
				if( item.m_joystick != HlMenuJoystick_No )
				{
					// Render each item
					item.m_pSprite->Render( BtFalse, item.m_v2Position, item.m_v2Dimension, index, 0, colour, MaxSortOrders-1 );
				}
			}
			else
			{
				if( item.m_joystick != HlMenuJoystick_Yes )
				{
					// Render each item
					item.m_pSprite->Render( BtFalse, item.m_v2Position, item.m_v2Dimension, index, 0, colour, MaxSortOrders-1 );
				}
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// Render

void HlView::Render()
{
//	if( BtStrCompare( m_lastArchiveName, m_archiveName ) == BtFalse )
//	{
//		Unload();
//		Load();
//	}

	if( UiKeyboard::pInstance()->IsPressed( ReloadMenuKey ) )
	{
		Unload();
        Load();
    }

	if( m_isBackground )
    {
		BtAssert( m_pBackgroundMaterial );

		// Render from the top left corner 0, 0 downward
        RsColour colour( RsColour::WhiteColour() );
		MtVector2 v2Position = MtVector2(0, 0);
        m_pBackgroundMaterial->Render( v2Position, m_v2RenderDimension, colour, m_backgroundSortOrder );
    }

	// Render the menu
	RenderMenu();
}

////////////////////////////////////////////////////////////////////////////////
// SetVisible

void HlView::SetVisible( BtU32 id, BtBool isVisible )
{
	HlMenuItems &item = m_items[id];
	item.m_isVisible = isVisible;
}

////////////////////////////////////////////////////////////////////////////////
// SetRenderDimension

void HlView::SetRenderDimension(MtVector2 v2Dimension)
{
	m_v2RenderDimension = v2Dimension;
}

////////////////////////////////////////////////////////////////////////////////
// SetVisible

void HlView::SetVisible( const BtChar *name, BtBool isVisible )
{
	BtU32 numItems = m_items.GetNumItems();

	for( BtU32 i=0; i<numItems; i++ )
	{
		// Cache each item
		HlMenuItems &item = m_items[i];

		if( BtStrCompareNoCase( item.m_id, name ) )
		{
			// Set the visible flag
			item.m_isVisible = isVisible;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// GetVisible

BtBool HlView::GetVisible( BtU32 id )
{
	HlMenuItems &item = m_items[id];
	return item.m_isVisible;
}