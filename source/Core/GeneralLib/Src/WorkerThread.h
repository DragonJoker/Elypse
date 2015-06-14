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
#ifndef ___WORKER_THREAD_H___
#define ___WORKER_THREAD_H___

#include "Thread.h"
#include "Mutex.h"
#include "Condition.h"

namespace General
{
	namespace MultiThreading
	{
		class WorkerThread
		{
		public:
			WorkerThread()
				: m_stopThread( false )
			{
				m_boostThread = Thread( GENLIB_THREAD_CLASS_FUNCTOR( this, WorkerThread, DoMainLoop ) );
			}
			~WorkerThread()
			{
			}

		private:
			void DoMainLoop()
			{
				bool l_stopThread = false;

				do
				{
					{
						GENLIB_AUTO_SCOPED_LOCK();
						GENLIB_CONDITION_WAIT( m_condition, m_mutex );
					}

					if ( m_functor )
					{
						m_functor();
					}

					{
						GENLIB_AUTO_SCOPED_LOCK();
						l_stopThread = m_stopThread;
					}
				}
				while ( ! l_stopThread );
			}

		public:
			void Execute( const std::function< void() > & p_functor )
			{
				if ( p_functor )
				{
					GENLIB_AUTO_SCOPED_LOCK();
					m_functor = p_functor;
					GENLIB_CONDITION_NOTIFY_ONE( m_condition );
				}
			}

			void StopNoWait()
			{
				GENLIB_AUTO_SCOPED_LOCK();
				m_stopThread = true;
			}

			void Stop()
			{
				{
					GENLIB_AUTO_SCOPED_LOCK();
					m_stopThread = true;
					GENLIB_CONDITION_NOTIFY_ONE( m_condition );
				}
				m_boostThread.join();
			}

		private:
			Condition m_condition;
			Mutex m_mutex;
			bool m_stopThread;
			Thread m_boostThread;
			std::function< void() > m_functor;
		};
	}
}

#endif
