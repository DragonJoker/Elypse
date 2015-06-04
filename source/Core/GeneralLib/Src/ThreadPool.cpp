#include "ThreadPool.h"

void General::MultiThreading::PoolWorkerThread::DoMainLoop()
{
	while ( m_pool->HasJob() )
	{
		ThreadPoolFunctor const & l_functor = m_pool->GetFunctor();

		if ( l_functor )
		{
			l_functor();
		}
	};
}

