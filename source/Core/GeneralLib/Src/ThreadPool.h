#ifndef ___THREAD_POOL_H___
#define ___THREAD_POOL_H___

#include <vector>

#include "ThreadedQueue.h"
#include "Thread.h"

/*
Performance :	85-90k empty jobs per second, on 1 to 10 threads (queue 100).
				110k / sec if the queue locks less( queue 10k)
				Test done on a single core 2.4 ghz P4
Explanation : low due to functors and multiple mutex/conditions.
To be tested on a dual core soon.

Note : weird performance degradation when using over 16 threads on the test machine.( limited by windows ?)
*/

namespace General
{
	namespace MultiThreading
	{
		typedef std::function<void()> ThreadPoolFunctor;

		class ThreadPool;

		class PoolWorkerThread
		{
		private:
			Thread * m_boostThread;
			ThreadPool * m_pool;

		public:
			PoolWorkerThread( ThreadPool * p_pool )
				:	m_pool( p_pool	)
			{
				m_boostThread	= new Thread( GENLIB_THREAD_CLASS_FUNCTOR( this, PoolWorkerThread, DoMainLoop ) );
			}
			~PoolWorkerThread()
			{
				m_boostThread->join();
				delete m_boostThread;
			}

		private:
			void DoMainLoop();
		};

		class ThreadPool
		{
		private:
			ThreadedQueue	<ThreadPoolFunctor>		m_jobQueue;
			std::vector		<PoolWorkerThread *>	m_threads;

		public:
			ThreadPool()
				:	m_jobQueue( 10000 )
			{}
			ThreadPool( unsigned int p_numThreads, unsigned int p_queueSize = 10 )
				:	m_jobQueue( p_queueSize )
			{
				AddThreads( p_numThreads );
			}
			~ThreadPool()
			{
				unsigned int imax = static_cast <unsigned int>( m_threads.size() );

				for ( unsigned int i = 0 ; i < imax ; i ++ )
				{
					delete m_threads[i];
				}

				m_threads.clear();
			}

		public:
			inline const ThreadPoolFunctor & GetFunctor()
			{
				return m_jobQueue.Pop();
			}
			inline void AddJob( const ThreadPoolFunctor & p_job )
			{
				m_jobQueue.Push( p_job );
			}
			inline bool HasJob()const
			{
				return !m_jobQueue.IsEmpty();
			}

		public:
			void AddThreads( unsigned int p_numThreads )
			{
				for ( unsigned int i = 0 ; i < p_numThreads ; i ++ )
				{
					m_threads.push_back( new PoolWorkerThread( this ) );
				}
			}
		};
	}
}

#endif
