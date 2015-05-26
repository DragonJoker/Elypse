#ifndef ___GENERAL_UTILS_TIMING_H___
#define ___GENERAL_UTILS_TIMING_H___

#include <map>
#include <string>

#include "Utils.h"
#include "Mutex.h"

#define GENLIB_INITIALISE_TIMER_MANAGER()\
	TimingManager * TimingManager::sm_singleton = new TimingManager( false)

#define GENLIB_INITIALISE_THREADED_TIMER_MANAGER()\
	TimingManager * TimingManager::sm_singleton = new TimingManager( true)

namespace General
{
	namespace Utils
	{
		typedef __int64 TimeType;
		typedef double PrecisionType;

		class TimeHolder
		{
		private:
			TimeType m_currentTime;
			TimeType m_averagingTotal;
			unsigned int m_numAveraging;

		public:
			TimeHolder( const TimeType & p_currentTime )
				:	m_currentTime( p_currentTime ),
					m_averagingTotal( 0 ),
					m_numAveraging( 0 )
			{
			}
			~TimeHolder() {}

		public:
			inline void SetTime( const TimeType & p_currentTime )
			{
				m_currentTime = p_currentTime;
			}
			inline TimeType GetAverage()
			{
				return ( m_averagingTotal / m_numAveraging );
			}
			inline const TimeType & GetTime()
			{
				return m_currentTime;
			}
			inline void ResetAverage()
			{
				m_averagingTotal = 0;
				m_numAveraging = 0;
			}

		public:
			void AddAverage( const TimeType & p_currentTime )
			{
				m_averagingTotal += ( p_currentTime - m_currentTime );
				m_currentTime = p_currentTime;
				m_numAveraging ++;
			}
		};

		typedef std::map <std::string, TimeHolder> TimeTypeMap;

		class TimingManager
		{
		private:
			TimeTypeMap m_timeMap;
			Mutex m_mutex;
			PrecisionType m_precision;
			bool m_threaded;

		private:
			static TimingManager * sm_singleton;

		public:
			TimingManager( bool p_threaded )
				:	m_threaded( p_threaded )
			{
				TimeType l_frequency;
				sm_singleton = this;
				QueryPerformanceFrequency( & l_frequency );
				m_precision = 1.0 / ( static_cast <PrecisionType>( l_frequency ) );
			}
			~TimingManager()
			{
				m_timeMap.clear();
				sm_singleton = NULL;
			}

		private:
			inline PrecisionType _getRealTime( const TimeType & p_startTime, const TimeType & p_endTime )const
			{
				return ( static_cast <PrecisionType>( p_endTime - p_startTime ) ) * m_precision;
			}

			static inline TimeType _getCurrentTime()
			{
				TimeType l_startTime;
				QueryPerformanceCounter( & l_startTime );
				return l_startTime;
			}

			inline void _lock()
			{
//			if (m_threaded) { GENLIB_AUTO_LOCK_MUTEX(); }
			}
			inline void _unlock()
			{
//			if (m_threaded) { GENLIB_AUTO_LOCK_MUTEX(); }
			}

		public:
			static inline PrecisionType GetPrecision()
			{
				return sm_singleton->m_precision;
			}
			static inline TimeType GetFrequency()
			{
				return static_cast <TimeType>( 1.0 / sm_singleton->m_precision );
			}
			static inline TimingManager * GetSingleton()
			{
				return sm_singleton;
			}

		public:
			static inline PrecisionType GetTimeForSection( const std::string & p_sectionName )
			{
				PrecisionType l_returnValue = 0.0;
				sm_singleton->_lock();
				const TimeTypeMap::iterator & ifind = sm_singleton->m_timeMap.find( p_sectionName );

				if ( ifind != sm_singleton->m_timeMap.end() )
				{
					TimeType l_startTime = ifind->second.GetTime();
					ifind->second.SetTime( _getCurrentTime() );
					l_returnValue = sm_singleton->_getRealTime( l_startTime, ifind->second.GetTime() );
				}

				sm_singleton->_unlock();
				return l_returnValue;
			}

			static inline PrecisionType StopSection( const std::string & p_sectionName )
			{
				PrecisionType l_returnValue = 0.0;
				sm_singleton->_lock();
				const TimeTypeMap::iterator & ifind = sm_singleton->m_timeMap.find( p_sectionName );

				if ( ifind != sm_singleton->m_timeMap.end() )
				{
					TimeType l_startTime = ifind->second.GetTime();
					sm_singleton->m_timeMap.erase( ifind );
					l_returnValue = sm_singleton->_getRealTime( l_startTime, _getCurrentTime() );
				}

				sm_singleton->_unlock();
				return l_returnValue;
			}

			static inline void StartSection( const std::string & p_sectionName )
			{
				sm_singleton->_lock();
				sm_singleton->m_timeMap.insert( TimeTypeMap::value_type( p_sectionName, TimeHolder( _getCurrentTime() ) ) );
				sm_singleton->_unlock();
			}

			static inline void StartAverage( const std::string & p_sectionName )
			{
				sm_singleton->_lock();
				const TimeTypeMap::iterator & ifind = sm_singleton->m_timeMap.find( p_sectionName );

				if ( ifind != sm_singleton->m_timeMap.end() )
				{
					ifind->second.ResetAverage();
					ifind->second.SetTime( _getCurrentTime() );
				}

				sm_singleton->_unlock();
			}

			static inline void TimeAverage( const std::string & p_sectionName )
			{
				sm_singleton->_lock();
				const TimeTypeMap::iterator & ifind = sm_singleton->m_timeMap.find( p_sectionName );

				if ( ifind != sm_singleton->m_timeMap.end() )
				{
					ifind->second.AddAverage( _getCurrentTime() );
				}

				sm_singleton->_unlock();
			}

			static inline PrecisionType GetAverage( const std::string & p_sectionName )
			{
				PrecisionType l_returnValue = 0.0;
				sm_singleton->_lock();
				const TimeTypeMap::iterator & ifind = sm_singleton->m_timeMap.find( p_sectionName );

				if ( ifind != sm_singleton->m_timeMap.end() )
				{
					l_returnValue = ifind->second.GetAverage() * sm_singleton->m_precision;
				}

				sm_singleton->_unlock();
				return l_returnValue;
			}
		};
	}
}

#endif
