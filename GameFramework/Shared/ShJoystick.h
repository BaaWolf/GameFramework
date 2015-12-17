////////////////////////////////////////////////////////////////////////////////
// ShKeyboard

// Include guard
#pragma once

// Includes
#include "BtBase.h"
#include "BtTypes.h"
#include "MtVector2.h"

enum ShJoystickButton
{
	JoystickButton_Pause,
	JoystickButton_Y,
	JoystickButton_B,
	JoystickButton_A,
	JoystickButton_X,
    JoystickButton_LeftShoulder,
	JoystickButton_RightShoulder,
    JoystickButton_Up,
	JoystickButton_Down,
	JoystickButton_Left,
	JoystickButton_Right,
	JoystickButton_MAX,
};

// Class definition
class ShJoystick
{
public:
    
    ShJoystick();

	static void								Update();

    static void                             RegisterButton( ShJoystickButton button, BtBool value );
    
    static BtBool							IsHeld( ShJoystickButton button );
	static BtBool							IsPressed( ShJoystickButton button );
	static BtBool							IsReleased( ShJoystickButton button );
    
    static BtBool                           IsButtonPressed();
    
    static void                             SetLeftThumbStick( MtVector2 v2Position );
    static void                             SetRightThumbStick( MtVector2 v2Position );
	static MtVector2						GetLeftThumbStick();
	static MtVector2						GetRightThumbStick();

	// Accessors
    static BtBool							IsConnected();
    static BtBool							IsUsed();
    static void                             SetUsed( BtBool isUsed );
    static void                             SetConnected( BtBool isConnected );

private:

	static BtBool						    m_isConnected;
	static MtVector2						m_v2LeftThumbStick;
	static MtVector2						m_v2RightThumbStick;

	static BtBool							m_isUsed;
	static BtBool							m_isHeld[JoystickButton_MAX];
	static BtBool							m_isReleased[JoystickButton_MAX];
	static BtBool							m_isPressed[JoystickButton_MAX];
	static BtBool							m_lastPressed[JoystickButton_MAX];
};
