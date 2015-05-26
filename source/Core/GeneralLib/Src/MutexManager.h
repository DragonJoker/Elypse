#ifndef ___MUTEX_MANAGER_H___
#define ___MUTEX_MANAGER_H___

#include "ThreadSpecific.h"
#include "AutoSingleton.h"
#include "Mutex.h"

#include <map>

#define GENLIB_THREAD_INIT() General::MultiThreading::MutexManager::GetSingleton().RegisterThread()

namespace General
{
	namespace MultiThreading
	{
		enum LockResult
		{
			LR_SUCCESS,
			LR_FAILURE_DOUBLE_LOCK,
			LR_FAILURE_DEADLOCK,
			LR_FAILURE_NOT_LOCKED,
			LR_FAILURE_WRONG_THREAD
		};

		class MutexState
		{
		private:
			const Mutex * m_mutexPtr;

		public:
			unsigned int m_lockCount;
			unsigned int m_ownerThreadID;
			bool m_recursive;

		public:
			MutexState( const Mutex * p_mutexPtr, unsigned int p_currentThread )
				:	m_mutexPtr( p_mutexPtr ),
					m_lockCount( 1 ),
					m_ownerThreadID( p_currentThread )
			{
			}

			LockResult Lock( unsigned int p_currentThreadID )
			{
				if ( m_lockCount == 0 )
				{
					m_lockCount = 1;
					m_ownerThreadID = p_currentThreadID;
					return LR_SUCCESS;
				}

				if ( p_currentThreadID == m_ownerThreadID )
				{
					m_lockCount ++;
					return LR_FAILURE_DOUBLE_LOCK;
				}

				return LR_FAILURE_DEADLOCK;
			}

			LockResult Unlock( unsigned int p_currentThreadID )
			{
				if ( m_lockCount == 0 )
				{
					return LR_FAILURE_NOT_LOCKED;
				}

				if ( p_currentThreadID != m_ownerThreadID )
				{
					return LR_FAILURE_WRONG_THREAD;
				}

				m_lockCount --;
				return LR_SUCCESS;
			}
		};

		class MutexManager : public AutoSingleton <MutexManager>
		{
		private:
			typedef std::map <const Mutex *, MutexState> MutexStateMap;

		public:
			ThreadSpecificUIntPtr m_threadID;
			unsigned int m_nextThreadID;
			Mutex m_mutex;
			MutexStateMap m_mutexeMap;

		public:
			MutexManager()
				: m_nextThreadID( 0 )
			{}
			~MutexManager() {}

		public:
			void RegisterThread()
			{
				boost::mutex::scoped_lock scoped_lock( m_mutex );
				m_threadID.reset( new unsigned int );
				( * m_threadID.get() ) = m_nextThreadID ++;
			}

			bool SimpleLock( const Mutex * p_mutexPtr )
			{
				boost::mutex::scoped_lock scoped_lock( m_mutex );
				const MutexStateMap::iterator & ifind = m_mutexeMap.find( p_mutexPtr );

				if ( ifind == m_mutexeMap.end() )
				{
					m_mutexeMap.insert( MutexStateMap::value_type( p_mutexPtr, MutexState( p_mutexPtr, ( * m_threadID.get() ) ) ) );
					return true;
				}

				LockResult l_result = ifind->second.Lock( ( * m_threadID.get() ) );

				if ( l_result != LR_SUCCESS )
				{
//				std::cout << "Error : " << l_result << " on thread " << (* m_threadID.get()) << " while locking\n";
					return false;
				}

				return true;
			}

			bool SimpleUnlock( const Mutex * p_mutexPtr )
			{
				boost::mutex::scoped_lock scoped_lock( m_mutex );
				const MutexStateMap::iterator & ifind = m_mutexeMap.find( p_mutexPtr );

				if ( ifind == m_mutexeMap.end() )
				{
//				std::cout << "Error : Mutex does not exist within the database\n";
					return false;
				}

				LockResult l_result = ifind->second.Unlock( ( * m_threadID.get() ) );

				if ( l_result != LR_SUCCESS )
				{
//				std::cout << "Error : " << l_result << " on thread " << (* m_threadID.get()) << " while unlocking\n";
					return false;
				}

				return true;
			}
		};

		struct MutexLock
		{
		private:
			Mutex & m_mutex;
			bool m_dead;

		public:
			MutexLock( Mutex & p_mutex )
				: m_mutex( p_mutex )
			{
				if ( MutexManager::GetSingleton().SimpleLock( & m_mutex ) )
				{
					m_dead = false;
					boost::detail::thread::lock_ops<Mutex>::lock( m_mutex );
				}
				else
				{
					m_dead = true;
				}
			}
			~MutexLock()
			{
				if ( ! m_dead )
				{
					if ( MutexManager::GetSingleton().SimpleUnlock( & m_mutex ) )
					{
						boost::detail::thread::lock_ops <Mutex> ::unlock( m_mutex );
					}
				}
			}

		private:
			MutexLock( const MutexLock & );
		};
	}
}

#ifdef GENLIB_SCOPED_LOCK
#	undef GENLIB_SCOPED_LOCK
#	undef GENLIB_SCOPED_RECURSIVE_LOCK
#	undef GENLIB_LOCK_MUTEX
#	undef GENLIB_UNLOCK_MUTEX
#endif

#define GENLIB_SCOPED_LOCK( p_mutex)			General::MultiThreading::MutexLock l_mutex_lock_ ## p_mutex( p_mutex)
#define GENLIB_SCOPED_RECURSIVE_LOCK( p_mutex)	General::MultiThreading::MutexLock l_mutex_lock_ ## p_mutex( p_mutex)
#define GENLIB_LOCK_MUTEX( p_mutex)				General::MultiThreading::MutexManager::GetSingleton().SimpleLock( & p_mutex)
#define GENLIB_UNLOCK_MUTEX( p_mutex)			General::MultiThreading::MutexManager::GetSingleton().SimpleUnlock( & p_mutex)

#endif
