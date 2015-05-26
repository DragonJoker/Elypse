#ifndef ___GENERAL_LIB_MUTEX_H___
#define ___GENERAL_LIB_MUTEX_H___

#include "MultiThreadConfig.h"

#if GENLIB_MT_USE_STL
#	include <thread>
#	include <mutex>

namespace General
{
	namespace MultiThreading
	{
		typedef std::recursive_mutex RecursiveMutex;
		typedef std::mutex Mutex;
	}
}

#	define GENLIB_AUTO_SCOPED_LOCK()				std::unique_lock< std::mutex > scoped_lock_m_mutex( m_mutex)
#	define GENLIB_SCOPED_LOCK( p_mutex)				std::unique_lock< std::mutex > scoped_lock_ ## p_mutex( p_mutex)
#	define GENLIB_SCOPED_RECURSIVE_LOCK( p_mutex)	std::unique_lock< std::recursive_mutex > scoped_lock_ ## p_mutex( p_mutex)

#	define GENLIB_LOCK_MUTEX( p_mutex)				(p_mutex).lock()
#	define GENLIB_UNLOCK_MUTEX( p_mutex)			(p_mutex).unlock()
#	define GENLIB_RECURSIVE_LOCK_MUTEX( p_mutex)	(p_mutex).lock()
#	define GENLIB_RECURSIVE_UNLOCK_MUTEX( p_mutex)	(p_mutex).unlock()

#elif GENLIB_MT_USE_BOOST
#	include <boost/thread/recursive_mutex.hpp>
#	include <boost/thread/mutex.hpp>

namespace General
{
	namespace MultiThreading
	{
		typedef boost::recursive_mutex RecursiveMutex;
		typedef boost::mutex Mutex;
	}
}

#	define GENLIB_AUTO_SCOPED_LOCK()				boost::mutex::scoped_lock scoped_lock_m_mutex( m_mutex)
#	define GENLIB_SCOPED_LOCK( p_mutex)				boost::mutex::scoped_lock scoped_lock_ ## p_mutex( p_mutex)
#	define GENLIB_SCOPED_RECURSIVE_LOCK( p_mutex)	boost::recursive_mutex::scoped_lock scoped_lock_ ## p_mutex( p_mutex)

#	define GENLIB_LOCK_MUTEX( p_mutex)				(p_mutex).lock()
#	define GENLIB_UNLOCK_MUTEX( p_mutex)			(p_mutex).unlock()
#	define GENLIB_RECURSIVE_LOCK_MUTEX( p_mutex)	(p_mutex).lock()
#	define GENLIB_RECURSIVE_UNLOCK_MUTEX( p_mutex)	(p_mutex).unlock()

#elif GENLIB_MT_USE_MFC

#	ifndef WINVER
#		define WINVER 0x0501
#	endif

#	include <afxmt.h>

namespace General
{
	namespace MultiThreading
	{
		typedef CMutex Mutex;

		class MutexScopedLock
		{
		private:
			Mutex & m_mutex;
			bool m_locked;

		public:
			MutexScopedLock( Mutex & p_mutex, bool p_startLocked = true )
				:	m_mutex( p_mutex ),
					m_locked( false )
			{
				if ( p_startLocked )
				{
					Lock();
				}
			}
			~MutexScopedLock()
			{
				if ( m_locked )
				{
					Unlock();
				}
			}

			void Lock()
			{
				if ( ! m_locked )
				{
					m_locked = true;
					m_mutex.Lock();
				}
			}

			void Unlock()
			{
				if ( m_locked )
				{
					m_locked = false;
					m_mutex.Unlock();
				}
			}

			bool IsLocked()const
			{
				return m_locked;
			}
			operator const void * ()const
			{
				return m_locked ? this : 0;
			}
		};
	}
}

#	define GENLIB_LOCK_MUTEX( p_mutex)	p_mutex.Lock()
#	define GENLIB_UNLOCK_MUTEX( p_mutex)	p_mutex.Unlock()
#	define GENLIB_SCOPED_LOCK( p_mutex)	MutexScopedLock scoped_lock_ ## p_mutex( p_mutex)

#endif  //GENLIB_MT_USE_BOOST || GENLIB_MT_USE_MFC

#endif
