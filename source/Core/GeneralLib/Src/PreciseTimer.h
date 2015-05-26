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
