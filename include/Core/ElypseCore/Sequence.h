/*
See LICENSE file in root folder
*/
#ifndef ___SEQUENCE_H___
#define ___SEQUENCE_H___

#include "Module_Sequences.h"

#include "BaseEvent.h"

namespace Elypse
{
	namespace Sequences
	{
		class d_dll_export Sequence : public named, noncopyable
		{
		public:
			Sequence( String const & p_name );
			~Sequence();

		public:
			void AddPonctualEvent( BasePonctualEvent * p_event, Real p_when );
			void ChangePonctualEventTime( BasePonctualEvent * p_event, Real p_when );
			void AddContinuousEvent( BaseContinuousEvent * p_event );
			void ChangeContinuousEventTime( BaseContinuousEvent * p_event, Real p_when );

			void RemovePonctualEvent( BasePonctualEvent * p_event );
			void RemoveContinuousEvent( BaseContinuousEvent * p_event );

		public:
			void Start();
			void Pause( bool p_paused );
			void Stop();
			virtual void Update( Real p_deltaTime );

			template< typename T >
			inline void SetTarget( T * p_target )
			{
				m_target = make_target( p_target );
				multimap::cycle( m_ponctualEvents, &BasePonctualEvent::SetTarget< T >, p_target );
				multimap::cycle( m_continuousEvents, &BaseContinuousEvent::SetTarget< T >, p_target );
			}

		public:
			inline void	SetLooped( bool p_looped )
			{
				m_looped = p_looped;
			}

			inline Real GetTotalLength()const
			{
				return m_totalLength;
			}
			inline bool IsLooped()const
			{
				return m_looped;
			}
			inline PonctualEventMap GetPonctualEvents()const
			{
				return m_ponctualEvents;
			}
			inline ContinuousEventMap GetContinuousEvents()const
			{
				return m_continuousEvents;
			}

		protected:
			void * m_target;

			Real m_totalLength;
			Real m_currentTime;

			bool m_looped;

			SequenceInstanceStatus m_status;
			ContinuousEventSet m_currentEvents;

			PonctualEventMap m_ponctualEvents;
			ContinuousEventMap m_continuousEvents;

			PonctualEventMap::iterator m_ponctualIterator;
			ContinuousEventMap::iterator m_continuousIterator;
		};
	}
}

#endif
