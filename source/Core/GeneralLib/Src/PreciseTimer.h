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

namespace General
{
	namespace Utils
	{
		class d_dll PreciseTimer
		{
		private:
			static long long sm_frequency;
		private:
			long long m_previousTime;

		public:
			PreciseTimer();
			~PreciseTimer();

		public:
			double Time();
			double TimeDiff( long long p_time );

		public:
			inline long long SaveTime()const
			{
				return m_previousTime;
			}

			inline friend std::ostream & operator << ( std::ostream & p_stream, PreciseTimer & p_timer )
			{
				p_stream << p_timer.Time();
				return p_stream;
			}
		};

		class d_dll RepeatTimer
		{
		private:
			long long m_previousTime;
			double m_repeatTime;

		private:
			static long long sm_frequency;

		public:
			RepeatTimer( double p_time );
			~RepeatTimer();

		public:
			bool Time();

			double GetRepeatTime()const
			{
				return m_repeatTime;
			};
		};
	}
}

#endif
