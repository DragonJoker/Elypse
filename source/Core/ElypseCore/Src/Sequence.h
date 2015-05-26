#ifndef ___SEQUENCE_H___
#define ___SEQUENCE_H___

#include "Module_Sequences.h"

namespace EMuse
{
	namespace Sequences
	{
		class d_dll_export Sequence : public named, noncopyable
		{
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

		public:
			Sequence( const String & p_name );
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

			void SetTarget( void * p_target );

		public:
			inline void	SetLooped( bool p_looped )
			{
				m_looped = p_looped;
			}

			inline Real					GetTotalLength()const
			{
				return m_totalLength;
			}
			inline bool					IsLooped()const
			{
				return m_looped;
			}
			inline PonctualEventMap		GetPonctualEvents()const
			{
				return m_ponctualEvents;
			}
			inline ContinuousEventMap	GetContinuousEvents()const
			{
				return m_continuousEvents;
			}
		};
	}
}

#endif
