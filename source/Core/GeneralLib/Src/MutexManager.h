/*
See LICENSE file in root folder
*/
#ifndef ___MUTEX_MANAGER_H___
#define ___MUTEX_MANAGER_H___

#include "AutoSingleton.h"

#include <map>
#include <mutex>

#define GENLIB_THREAD_INIT() General::MultiThreading::MutexManager::GetSingleton().RegisterThread()

namespace Private
{
#	include "Mutex.h"
}

namespace General
{
	namespace MultiThreading
	{
		class MutexManager
			: public Theory::AutoSingleton< MutexManager >
		{
		private:
			enum LockResult
			{
				LR_UNDEFINED,
				LR_SUCCESS,
				LR_FAILURE_DOUBLE_LOCK,
				LR_FAILURE_DEADLOCK,
				LR_FAILURE_NOT_LOCKED,
				LR_FAILURE_WRONG_THREAD
			};

			class MutexState
			{
			public:
				LockResult Lock()
				{
					auto l_return{ LR_UNDEFINED };

					if ( !m_lockCount )
					{
						m_lockCount++;
						m_ownerThreadID = std::this_thread::get_id();
						l_return = LR_SUCCESS;
					}
					else if ( std::this_thread::get_id() == m_ownerThreadID )
					{
						m_lockCount++;
						l_return = LR_FAILURE_DOUBLE_LOCK;
					}
					else
					{
						l_return = LR_FAILURE_DEADLOCK;
					}

					return l_return;
				}

				LockResult Unlock()
				{
					auto l_return{ LR_UNDEFINED };

					if ( m_lockCount == 0 )
					{
						l_return = LR_FAILURE_NOT_LOCKED;
					}
					else if ( std::this_thread::get_id() != m_ownerThreadID )
					{
						l_return = LR_FAILURE_WRONG_THREAD;
					}
					else
					{
						m_lockCount--;
						l_return = LR_SUCCESS;
					}

					return l_return;
				}

			private:
				uint32_t m_lockCount{ 1 };
				std::thread::id m_ownerThreadID{ std::this_thread::get_id() };
			};

			using MutexStateMap = std::map< std::mutex const *, MutexState >;

		public:
			bool SimpleLock( std::mutex const & p_mutex )
			{
				auto l_return{ false };
				std::unique_lock< std::mutex > scoped_lock{ m_mutex };
				auto && ifind = m_mutexesMap.find( &p_mutex );

				if ( ifind == m_mutexesMap.end() )
				{
					m_mutexesMap.insert( { &p_mutex, MutexState{} } );
					l_return = true;
				}
				else
				{
					l_return = ifind->second.Lock() == LR_SUCCESS;
				}

				return l_return;
			}

			bool SimpleUnlock( std::mutex const & p_mutex )
			{
				auto l_return{ false };
				std::unique_lock< std::mutex > scoped_lock{ m_mutex };
				auto && ifind = m_mutexesMap.find( &p_mutex );

				if ( ifind != m_mutexesMap.end() )
				{
					l_return = ifind->second.Unlock() == LR_SUCCESS;
				}

				return l_return;
			}

		private:
			std::mutex m_mutex;
			MutexStateMap m_mutexesMap;
		};

		template< typename T_mutex >
		struct MutexLock
		{
		public:
			MutexLock( T_mutex & p_mutex )
				: m_mutex{ p_mutex }
			{
				if ( MutexManager::GetSingleton().SimpleLock( m_mutex ) )
				{
					m_dead = false;
					m_mutex.lock();
				}
				else
				{
					m_dead = true;
				}
			}
			~MutexLock()
			{
				if ( !m_dead )
				{
					if ( MutexManager::GetSingleton().SimpleUnlock( m_mutex ) )
					{
						m_mutex.unlock();
					}
				}
			}

		private:
			MutexLock( const MutexLock & ) = delete;
			MutexLock & operator=( const MutexLock & ) = delete;

		private:
			T_mutex & m_mutex;
			bool m_dead{ false };
		};

		template< typename T_mutex >
		MutexLock< T_mutex > make_unique_lock( T_mutex & p_mutex )
		{
			return MutexLock< T_mutex > { p_mutex };
		}
	}
}

#endif
