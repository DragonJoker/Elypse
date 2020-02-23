/*
See LICENSE file in root folder
*/
#include "ThreadPool.h"

void General::MultiThreading::PoolWorkerThread::DoMainLoop()
{
	while ( m_pool->HasJob() )
	{
		m_pool->GetFunctor()();
	};
}

