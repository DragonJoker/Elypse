#ifndef ___GENLIB_THREAD_ID___
#define ___GENLIB_THREAD_ID___

#ifdef WIN32

#include <ThreadSpecific.h>
#include <AutoSingleton.h>
#include <Mutex.h>

namespace General
{
	namespace MultiThreading
	{
		class ThreadId : public AutoSingleton <ThreadId>
		{
		private:
			ThreadSpecificUIntPtr m_id;
			Mutex m_mutex;
			unsigned int m_numThreads;

		public:
			inline ThreadId()
				:	m_numThreads( 0 )
			{
			}

			inline unsigned int GetId()
			{
				if ( m_id.get() == NULL )
				{
					GENLIB_AUTO_SCOPED_LOCK();
					m_id.reset( new unsigned int );
					( * m_id.get() ) = m_numThreads;
					m_numThreads ++;
				}

				return * m_id.get();
			}

		public:
			static inline unsigned int GetSelf()
			{
				return GetSingletonPtr()->GetId();
			}
		};
	}
}

#else

#include <pthread.h>

namespace General
{
	namespace MultiThreading
	{
		class ThreadId
		{
		public:
			static inline pthread_t GetSelf()
			{
				return pthread_self();
			}
		};
	}
}

#endif

#define GENLIB_THREAD_ID() General::MultiThreading::ThreadId::GetSelf()

#endif
