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
#ifndef ___MUTEX_MANAGER_H___
#define ___MUTEX_MANAGER_H___

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
			MutexState( const Mutex * p_mutexPtr )
				: m_mutexPtr( p_mutexPtr )
				, m_lockCount( 1 )
				, m_ownerThreadID( std::this_thread::get_id() )
			{
			}

			LockResult Lock()
			{
				LockResult l_return = LR_UNDEFINED;

				if ( !m_lockCount )
				{
					m_lockCount++;
					m_ownerThreadID = std::this_thread::get_id();
					l_return = LR_SUCCESS;
				}
				else if ( std::this_thread::get_id() == m_ownerThreadID )
				{
					m_lockCount ++;
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
				LockResult l_return = LR_UNDEFINED;

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
					m_lockCount --;
					l_return = LR_SUCCESS;
				}

				return l_return;
			}

		private:
			const Mutex * m_mutexPtr;

		public:
			unsigned int m_lockCount;
			std::thread::id m_ownerThreadID;
			bool m_recursive;
		};

		class MutexManager
			: public Theory::AutoSingleton< MutexManager >
		{
		public:
			MutexManager()
				: m_nextThreadID( 0 )
			{
			}

			~MutexManager()
			{
			}

		public:
			void RegisterThread()
			{
				std::unique_lock< std::mutex > scoped_lock( m_mutex );
			}

			bool SimpleLock( const Mutex * p_mutexPtr )
			{
				bool l_return = false;
				std::unique_lock< std::mutex > scoped_lock( m_mutex );
				auto && ifind = m_mutexeMap.find( p_mutexPtr );

				if ( ifind == m_mutexeMap.end() )
				{
					m_mutexeMap.insert( std::make_pair( p_mutexPtr, MutexState( p_mutexPtr ) ) );
					l_return = true;
				}
				else
				{
					l_return = ifind->second.Lock() == LR_SUCCESS;
				}

				return l_return;
			}

			bool SimpleUnlock( const Mutex * p_mutexPtr )
			{
				bool l_return = false;
				std::unique_lock< std::mutex > scoped_lock( m_mutex );
				auto && ifind = m_mutexeMap.find( p_mutexPtr );

				if ( ifind != m_mutexeMap.end() )
				{
					l_return = ifind->second.Unlock() == LR_SUCCESS;
				}

				return l_return;
			}

		private:
			typedef std::map< const Mutex *, MutexState > MutexStateMap;

		public:
			Mutex m_mutex;
			MutexStateMap m_mutexeMap;
		};

		struct MutexLock
		{
		public:
			MutexLock( Mutex & p_mutex )
				: m_mutex( p_mutex )
			{
				if ( MutexManager::GetSingleton().SimpleLock( &m_mutex ) )
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
					if ( MutexManager::GetSingleton().SimpleUnlock( &m_mutex ) )
					{
						m_mutex.unlock();
					}
				}
			}

		private:
			MutexLock( const MutexLock & );

		private:
			Mutex & m_mutex;
			bool m_dead;
		};
	}
}

#ifdef GENLIB_SCOPED_LOCK
#	undef GENLIB_SCOPED_LOCK
#	undef GENLIB_SCOPED_RECURSIVE_LOCK
#	undef GENLIB_LOCK_MUTEX
#	undef GENLIB_UNLOCK_MUTEX
#endif

#define GENLIB_SCOPED_LOCK( p_mutex ) General::MultiThreading::MutexLock l_mutex_lock_ ## p_mutex( p_mutex )
#define GENLIB_SCOPED_RECURSIVE_LOCK( p_mutex ) General::MultiThreading::MutexLock l_mutex_lock_ ## p_mutex( p_mutex )
#define GENLIB_LOCK_MUTEX( p_mutex ) General::MultiThreading::MutexManager::GetSingleton().SimpleLock( &p_mutex )
#define GENLIB_UNLOCK_MUTEX( p_mutex ) General::MultiThreading::MutexManager::GetSingleton().SimpleUnlock( &p_mutex )

#endif
