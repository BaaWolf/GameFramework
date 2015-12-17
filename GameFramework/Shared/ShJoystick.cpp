////////////////////////////////////////////////////////////////////////////////
// ShJoystick

// Includes
#include "ShJoystick.h"
#include "RsUtil.h"
#include "RsMaterial.h"
#include "UiKeyboard.h"
#include "BtString.h"

////////////////////////////////////////////////////////////////////////////////
// statics

MtVector2 ShJoystick::m_v2LeftThumbStick;
MtVector2 ShJoystick::m_v2RightThumbStick;

BtBool ShJoystick::m_isUsed = BtFalse;

//BtBool ShJoystick::m_isConnected = BtFalse;
BtBool ShJoystick::m_isConnected = BtFalse;

BtBool ShJoystick::m_isHeld[JoystickButton_MAX];
BtBool ShJoystick::m_isReleased[JoystickButton_MAX];
BtBool ShJoystick::m_isPressed[JoystickButton_MAX];
BtBool ShJoystick::m_lastPressed[JoystickButton_MAX];

ShJoystick joystick;

////////////////////////////////////////////////////////////////////////////////
// Constructor

ShJoystick::ShJoystick()
{
	for( BtU32 index = 0; index < JoystickButton_MAX; index++ )
	{
		m_lastPressed[index] = BtFalse;
		m_isHeld[index]      = BtFalse;
		m_isReleased[index]  = BtFalse;
	}
}

////////////////////////////////////////////////////////////////////////////////
// RegisterButton

void ShJoystick::RegisterButton( ShJoystickButton button, BtBool value )
{
    m_isHeld[button] = value;
	m_isUsed = BtTrue;
}

////////////////////////////////////////////////////////////////////////////////
// Update

//static
void ShJoystick::Update()
{
	for( BtU32 index = 0; index < JoystickButton_MAX; index++ )
	{
		m_isPressed[index] = BtFalse;
		m_isReleased[index] = BtFalse;
        
		if( m_isHeld[index] == BtTrue )
		{
			if( m_lastPressed[index] == BtFalse )
			{
				m_isPressed[index] = BtTrue;
			}
			m_lastPressed[index] = BtTrue;
		}
		else
		{
			if( m_lastPressed[index] == BtTrue )
			{
				m_isReleased[index] = BtTrue;
			}
			m_lastPressed[index] = BtFalse;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// IsButtonPressed

//static
BtBool ShJoystick::IsButtonPressed()
{
    for( BtU32 i=0; i<JoystickButton_MAX; i++ )
    {
        if( m_isPressed[i] )
        {
            return BtTrue;
        }
    }
    return BtFalse;
}

////////////////////////////////////////////////////////////////////////////////
// SetConnected

//static
void ShJoystick::SetConnected( BtBool isConnected )
{
	m_isUsed = BtTrue;
    m_isConnected = isConnected;
}

////////////////////////////////////////////////////////////////////////////////
// SetUsed

//static
void ShJoystick::SetUsed( BtBool isUsed )
{
	m_isUsed = isUsed;
}

////////////////////////////////////////////////////////////////////////////////
// IsUsed

//static
BtBool ShJoystick::IsUsed()
{
	return m_isUsed;
}

////////////////////////////////////////////////////////////////////////////////
// IsConnected

//static
BtBool ShJoystick::IsConnected()
{
	return m_isConnected;
}

////////////////////////////////////////////////////////////////////////////////
// IsReleased

//static
BtBool ShJoystick::IsReleased( ShJoystickButton button )
{
	return m_isReleased[button];
}

////////////////////////////////////////////////////////////////////////////////
// IsPressed

//static
BtBool ShJoystick::IsPressed( ShJoystickButton button )
{
	return m_isPressed[button];
}

////////////////////////////////////////////////////////////////////////////////
// IsHeld

//static
BtBool ShJoystick::IsHeld( ShJoystickButton button )
{
	return m_isHeld[button];
}

////////////////////////////////////////////////////////////////////////////////
// SetLeftThumbStick

void ShJoystick::SetLeftThumbStick( MtVector2 v2Position )
{
	m_isUsed = BtTrue;
	m_v2LeftThumbStick = v2Position;
}

////////////////////////////////////////////////////////////////////////////////
// SetRightThumbStick

void ShJoystick::SetRightThumbStick( MtVector2 v2Position )
{
	m_isUsed = BtTrue;
	m_v2RightThumbStick = v2Position;
}

////////////////////////////////////////////////////////////////////////////////
// GetLeftThumbStick

MtVector2 ShJoystick::GetLeftThumbStick()
{
	return m_v2LeftThumbStick;
}

////////////////////////////////////////////////////////////////////////////////
// GetRightThumbStick

MtVector2 ShJoystick::GetRightThumbStick()
{
	return m_v2RightThumbStick;
}
