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
		private:
			Condition m_condition;
			Mutex m_mutex;
			bool m_stopThread;
			Thread * m_boostThread;
			boost::function0 <void> m_functor;

		public:
			WorkerThread()
				:	m_stopThread( false )
			{
				m_boostThread = new Thread( GENLIB_THREAD_CLASS_FUNCTOR( this, WorkerThread, DoMainLoop ) );
			}
			~WorkerThread()
			{
				delete m_boostThread;
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

					if ( ! m_functor.empty() )
					{
						m_functor();
					}

					{
						GENLIB_AUTO_SCOPED_LOCK();
						m_functor.clear();
						l_stopThread = m_stopThread;
					}
				}
				while ( ! l_stopThread );
			}

		public:
			void Execute( const boost::function0 <void> & p_functor )
			{
				if ( ! p_functor.empty() )
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
				m_boostThread->join();
			}
		};
	}
}

#endif
