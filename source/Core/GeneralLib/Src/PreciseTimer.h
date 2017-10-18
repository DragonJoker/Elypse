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
#ifndef ___PRECISE_TIMER_H___
#define ___PRECISE_TIMER_H___

#include "Macros.h"

#include <ostream>
#include <chrono>

namespace General
{
	namespace Utils
	{
		class d_dll PreciseTimer
		{
		private:
			using Clock = std::chrono::high_resolution_clock;

		public:
			PreciseTimer();
			~PreciseTimer();

			double Time();
			double TimeDiff( Clock::time_point const & p_time )const;

			inline Clock::time_point const & SaveTime()const
			{
				return m_previousTime;
			}

		private:
			Clock::time_point m_previousTime;
		};

		inline std::ostream & operator<<( std::ostream & p_stream, PreciseTimer & p_timer )
		{
			p_stream << p_timer.Time();
			return p_stream;
		}

		class d_dll RepeatTimer
		{
		private:
			using Clock = std::chrono::high_resolution_clock;

		public:
			RepeatTimer( std::chrono::milliseconds const & p_time );
			~RepeatTimer();

			bool Time();

			std::chrono::milliseconds const & GetRepeatTime()const
			{
				return m_repeatTime;
			}

		private:
			Clock::time_point m_previousTime;
			std::chrono::milliseconds m_repeatTime;
		};
	}
}

#endif
