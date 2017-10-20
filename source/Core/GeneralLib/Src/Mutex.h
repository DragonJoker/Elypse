/*
See LICENSE file in root folder
*/
#ifndef ___GENERAL_LIB_MUTEX_H___
#define ___GENERAL_LIB_MUTEX_H___

#include "MultiThreadConfig.h"

#include <thread>
#include <mutex>

namespace General
{
	namespace MultiThreading
	{
		template< typename T_mutex >
		std::unique_lock< T_mutex > make_unique_lock( T_mutex & p_mutex )
		{
			return std::unique_lock< T_mutex > { p_mutex };
		}
	}
}

#endif
