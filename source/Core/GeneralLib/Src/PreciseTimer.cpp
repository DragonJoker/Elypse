/*
See LICENSE file in root folder
*/
#include "PreciseTimer.h"

#include <chrono>

using namespace General::Utils;


PreciseTimer::PreciseTimer()
{
	m_previousTime = Clock::now();
}

PreciseTimer::~PreciseTimer()
{
}

double PreciseTimer::Time()
{
	auto l_currentTime = Clock::now();
	auto l_diff = l_currentTime - m_previousTime;
	m_previousTime = l_currentTime;
	return std::chrono::duration_cast< std::chrono::milliseconds > ( l_diff ).count() / 1000.0;
}

double PreciseTimer::TimeDiff( Clock::time_point const & p_time )const
{
	return std::chrono::duration_cast< std::chrono::milliseconds > ( m_previousTime - p_time ).count() / 1000.0;
}


RepeatTimer::RepeatTimer( std::chrono::milliseconds const & p_time )
	: m_repeatTime( p_time )
{
	m_previousTime = Clock::now();
}

RepeatTimer::~RepeatTimer()
{
}

bool RepeatTimer::Time()
{
	auto l_currentTime = Clock::now();
	auto l_diff = l_currentTime - m_previousTime;

	if ( l_diff >= m_repeatTime )
	{
		m_previousTime = l_currentTime;
		return true;
	}

	return false;
}
