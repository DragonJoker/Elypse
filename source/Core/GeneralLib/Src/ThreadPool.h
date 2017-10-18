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
#ifndef ___THREAD_POOL_H___
#define ___THREAD_POOL_H___

#include <vector>

#include "ThreadedQueue.h"
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
