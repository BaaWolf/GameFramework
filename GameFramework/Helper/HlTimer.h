////////////////////////////////////////////////////////////////////////////////
// HlTimer

// Include guard
#pragma once

// Includes
#include "BtBase.h"
#include "BtArray.h"
#include "BtTypes.h"

struct HlTimerEvent
{
    BtChar			                        m_name[64];
    BtFloat									m_startTime;
	BtFloat									m_endTime;
	BtFloat									m_seconds;
};

// Class definition
class HlTimer
{
public:

	// Public functions
	static void								Empty();
	static BtBool                           IsTimer(const BtChar *name);
	static BtBool                           IsReady( const BtChar *name );
	static void								ResetTimer( const BtChar *name, BtFloat seconds = 0 );
	static BtFloat							GetElapsedTime( const BtChar *name );
	static BtFloat							GetCountdown( const BtChar *name );
	static BtFloat						   *GetTime( const BtChar *name );

private:

	static void                             AddNewTimer( const BtChar *name, BtFloat seconds );
	static HlTimerEvent					   *GetTimer( const BtChar *name );
    static BtArray<HlTimerEvent, 128>       m_timers;
};
