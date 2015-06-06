#ifndef ___GENERAL_LIB_MUTEX_H___
#define ___GENERAL_LIB_MUTEX_H___

#include "MultiThreadConfig.h"

#include <thread>
#include <mutex>

namespace General
{
	namespace MultiThreading
	{
		typedef std::recursive_mutex RecursiveMutex;
		typedef std::mutex Mutex;
	}
}

#define GENLIB_AUTO_SCOPED_LOCK() std::unique_lock< std::mutex > scoped_lock_m_mutex( m_mutex )
#define GENLIB_SCOPED_LOCK( p_mutex ) std::unique_lock< std::mutex > scoped_lock_ ## p_mutex( p_mutex )
#define GENLIB_SCOPED_RECURSIVE_LOCK( p_mutex ) std::unique_lock< std::recursive_mutex > scoped_lock_ ## p_mutex( p_mutex )

#define GENLIB_LOCK_MUTEX( p_mutex ) ( p_mutex ).lock()
#define GENLIB_UNLOCK_MUTEX( p_mutex ) ( p_mutex ).unlock()
#define GENLIB_RECURSIVE_LOCK_MUTEX( p_mutex ) ( p_mutex ).lock()
#define GENLIB_RECURSIVE_UNLOCK_MUTEX( p_mutex ) ( p_mutex ).unlock()

#endif
