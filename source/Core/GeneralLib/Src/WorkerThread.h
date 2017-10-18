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

#include <thread>
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
				: m_stopThread{ false }
				, m_thread{ [this](){ DoMainLoop(); } }
			{
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
						auto l_lock = make_unique_lock( m_mutex );
						m_condition.wait( l_lock );
					}

					if ( m_functor )
					{
						m_functor();
					}

					{
						auto l_lock = make_unique_lock( m_mutex );
						l_stopThread = m_stopThread;
					}
				}
				while ( !l_stopThread );
			}

		public:
			void Execute( const std::function< void() > & p_functor )
			{
				if ( p_functor )
				{
					auto l_lock = make_unique_lock( m_mutex );
					m_functor = p_functor;
					m_condition.notify_one();
				}
			}

			void StopNoWait()
			{
				auto l_lock = make_unique_lock( m_mutex );
				m_stopThread = true;
			}

			void Stop()
			{
				{
					auto l_lock = make_unique_lock( m_mutex );
					m_stopThread = true;
					m_condition.notify_one();
				}
				m_thread.join();
			}

		private:
			std::condition_variable m_condition;
			std::mutex m_mutex;
			bool m_stopThread;
			std::thread m_thread;
			std::function< void() > m_functor;
		};
	}
}

#endif
