#ifndef ___CONDITION_H___
#define ___CONDITION_H___

#include "MultiThreadConfig.h"

#include <condition_variable>

namespace General
{
	namespace MultiThreading
	{
		typedef std::condition_variable Condition;
	}
}

#define GENLIB_CONDITION_WAIT( p_condition, p_mutexName ) p_condition.wait( scoped_lock_ ## p_mutexName )
#define GENLIB_CONDITION_NOTIFY_ONE( p_condition ) p_condition.notify_one()
#define GENLIB_CONDITION_NOTIFY_ALL( p_condition ) p_condition.notify_all()

#endif
