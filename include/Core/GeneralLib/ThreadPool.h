/*
See LICENSE file in root folder
*/
#ifndef ___THREAD_POOL_H___
#define ___THREAD_POOL_H___

#include <vector>

#include "ThreadedQueue.h"
#include <functional>
#include <thread>

/*
Performance : 85-90k empty jobs per second, on 1 to 10 threads (queue 100).
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
		typedef std::function< void() > ThreadPoolFunctor;

		class ThreadPool;

		class PoolWorkerThread
		{
		public:
			PoolWorkerThread( ThreadPool * p_pool )
				: m_pool{ p_pool }
			{
				m_thread = std::thread{ [this]()
				{
					DoMainLoop();
				} };
			}

			~PoolWorkerThread()
			{
				m_thread.join();
			}

		private:
			void DoMainLoop();

		private:
			std::thread m_thread;
			ThreadPool * m_pool;
		};

		class ThreadPool
		{
		public:
			ThreadPool()
				: m_jobQueue{ 10000 }
			{
			}

			ThreadPool( uint32_t p_numThreads, uint32_t p_queueSize = 10 )
				: m_jobQueue{ p_queueSize }
			{
				AddThreads( p_numThreads );
			}

			~ThreadPool()
			{
				m_threads.clear();
			}

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

			void AddThreads( uint32_t p_numThreads )
			{
				for ( uint32_t i = 0 ; i < p_numThreads ; i ++ )
				{
					m_threads.push_back( std::make_unique< PoolWorkerThread >( this ) );
				}
			}

		private:
			ThreadedQueue< ThreadPoolFunctor > m_jobQueue;
			std::vector< std::unique_ptr< PoolWorkerThread > > m_threads;
		};
	}
}

#endif
