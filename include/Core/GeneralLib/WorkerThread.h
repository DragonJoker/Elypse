/*
See LICENSE file in root folder
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
