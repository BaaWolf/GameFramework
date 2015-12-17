////////////////////////////////////////////////////////////////////////////////
// HlTimer

// Includes
#include "HlTimer.h"
#include "BtTime.h"
#include "BtString.h"

// statics
BtArray<HlTimerEvent, 128> HlTimer::m_timers;

////////////////////////////////////////////////////////////////////////////////
// Empty

//static
void HlTimer::Empty()
{
	m_timers.Empty();
}

////////////////////////////////////////////////////////////////////////////////
// AddNewTimer

//static
void HlTimer::AddNewTimer( const BtChar *name, BtFloat seconds )
{
	HlTimerEvent event;
	BtStrCopy( event.m_name, 64, name );
	event.m_startTime = BtTime::GetElapsedTimeInSeconds();
	event.m_endTime = event.m_startTime + seconds;
	event.m_seconds = seconds;
	m_timers.Add( event );
}

///////////////////////////////////////////////////////////////////////////////
// GetTimer

//static
HlTimerEvent *HlTimer::GetTimer( const BtChar *name )
{
	// Cache the number of timers
	BtU32 count = m_timers.GetNumItems();

	for( BtU32 i=0; i<count; i++ )
	{
		HlTimerEvent &event = m_timers[i];

		if( BtStrCompareNoCase( name, event.m_name )  )
		{
			return &event;
		}
	}
	return BtNull;
}

////////////////////////////////////////////////////////////////////////////////
// IsTimer

//static
BtBool HlTimer::IsTimer( const BtChar *name )
{
	HlTimerEvent *pEvent = GetTimer(name);
    
	if (pEvent)
	{
		return BtTrue;
	}
	return BtFalse;
}

////////////////////////////////////////////////////////////////////////////////
// IsReady

//static
BtBool HlTimer::IsReady( const BtChar *name )
{
	BtFloat currentTime = BtTime::GetElapsedTimeInSeconds();

	HlTimerEvent *pEvent = GetTimer( name );

	if( pEvent )
	{
		if( currentTime >= pEvent->m_endTime )
		{
			return BtTrue;
		}
		else
		{
			return BtFalse;
		}
	}

	// Could not find timer
	BtAssert(0);
	return BtFalse;
}

////////////////////////////////////////////////////////////////////////////////
// GetCountdown

//static
BtFloat HlTimer::GetCountdown( const BtChar *name )
{
	BtFloat currentTime = BtTime::GetElapsedTimeInSeconds();

	HlTimerEvent *pEvent = GetTimer( name );

	if( pEvent )
	{
		// Calculate the number of seconds that have elapsed
		float seconds = pEvent->m_endTime - currentTime;

		// Return the number of seconds
		return seconds;
	}
	//BtAssert(0);
	return 0.0f;
}

////////////////////////////////////////////////////////////////////////////////
// GetElapsedTime

//static
BtFloat HlTimer::GetElapsedTime( const BtChar *name )
{
	BtFloat currentTime = BtTime::GetElapsedTimeInSeconds();

	HlTimerEvent *pEvent = GetTimer( name );

	if( pEvent )
	{
		// Calculate the number of seconds that have elapsed
		float seconds = currentTime - pEvent->m_startTime;

		// Return the number of seconds
		return seconds;
	}
	BtAssert(0);
	return 0.0f;
}

////////////////////////////////////////////////////////////////////////////////
// ResetTimer

//static
void HlTimer::ResetTimer(const BtChar *name, BtFloat seconds)
{
	BtFloat currentTime = BtTime::GetElapsedTimeInSeconds();

	HlTimerEvent *pEvent = GetTimer(name);

	if (pEvent)
	{
		pEvent->m_startTime = currentTime;
		pEvent->m_endTime   = currentTime + seconds;
	}
	else
	{
		// Could not find timer so create one
		AddNewTimer(name, seconds);
	}
}
