/*
See LICENSE file in root folder
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
