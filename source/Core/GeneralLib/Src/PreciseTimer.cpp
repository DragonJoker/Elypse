/*
This source file is part of ElypsePlayer (https://sourceforge.net/projects/elypse/)

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
the program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
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
