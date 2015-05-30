#include "PreciseTimer.h"

using namespace General::Utils;

#if GENLIB_WINDOWS
# include "Utils.h"
# include "Exception.h"

long long PreciseTimer::sm_frequency = 0;
long long RepeatTimer::sm_frequency = 0;

#else
#	include <sys/time.h>

long long PreciseTimer::sm_frequency = 1000000;
long long RepeatTimer::sm_frequency = 1000000;

#endif

PreciseTimer::PreciseTimer()
{
#if  GENLIB_WINDOWS

	if ( sm_frequency == 0 )
	{
		QueryPerformanceFrequency( & sm_frequency );

		if ( sm_frequency == 0 )
		{
			GENLIB_EXCEPTION( "Could not access the high precision timer" );
		}
	}

	QueryPerformanceCounter( & m_previousTime );
#else
	timeval l_time;
	gettimeofday( & l_time, NULL );
	m_previousTime = l_time.tv_sec * 1000000 + l_time.tv_usec;
#endif
}

PreciseTimer::~PreciseTimer()
{
}

double PreciseTimer::Time()
{
	long long l_currentTime;
#if  GENLIB_WINDOWS
	QueryPerformanceCounter( & l_currentTime );
#else
	timeval l_time;
	gettimeofday( & l_time, NULL );
	l_currentTime = l_time.tv_sec * 1000000 + l_time.tv_usec;
#endif
	double l_diff = static_cast <double>( l_currentTime - m_previousTime ) / static_cast <double>( sm_frequency );
	m_previousTime = l_currentTime;
	return l_diff;
}

double PreciseTimer::TimeDiff( long long p_time )
{
	return static_cast <double>( m_previousTime - p_time ) / static_cast <double>( sm_frequency );
}


RepeatTimer::RepeatTimer( double p_time )
	:	m_repeatTime( p_time )
{
#if  GENLIB_WINDOWS

	if ( sm_frequency == 0 )
	{
		QueryPerformanceFrequency( & sm_frequency );

		if ( sm_frequency == 0 )
		{
			GENLIB_EXCEPTION( "Could not access the high precision timer" );
		}
	}

	QueryPerformanceCounter( & m_previousTime );
#else
	timeval l_time;
	gettimeofday( & l_time, NULL );
	m_previousTime = l_time.tv_sec * 1000000 + l_time.tv_usec;
#endif
}

RepeatTimer::~RepeatTimer()
{
}

bool RepeatTimer::Time()
{
	long long l_currentTime;
#if  GENLIB_WINDOWS
	QueryPerformanceCounter( & l_currentTime );
#else
	timeval l_time;
	gettimeofday( & l_time, NULL );
	l_currentTime = l_time.tv_sec * 1000000 + l_time.tv_usec;
#endif
	double l_diff = static_cast <double>( l_currentTime - m_previousTime ) / static_cast <double>( sm_frequency );

	if ( l_diff >= m_repeatTime )
	{
		m_previousTime = l_currentTime;
		return true;
	}

	return false;
}
