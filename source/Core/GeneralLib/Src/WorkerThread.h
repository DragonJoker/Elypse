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
