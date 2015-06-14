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
#ifndef ___SEQUENCE_H___
#define ___SEQUENCE_H___

#include "Module_Sequences.h"

namespace Elypse
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
