#ifndef ___CONDITION_H___
#define ___CONDITION_H___

#include "MultiThreadConfig.h"

#if GENLIB_MT_USE_STL
#	include <condition_variable>

namespace General
{
	namespace MultiThreading
	{
		typedef std::condition_variable Condition;
	}
}

#	define GENLIB_CONDITION_WAIT( p_condition, p_mutexName)	p_condition.wait( scoped_lock_ ## p_mutexName)
#	define GENLIB_CONDITION_NOTIFY_ONE( p_condition)			p_condition.notify_one()
#	define GENLIB_CONDITION_NOTIFY_ALL( p_condition)			p_condition.notify_all()

#elif GENLIB_MT_USE_BOOST
#	include <boost/thread/condition.hpp>

namespace General
{
	namespace MultiThreading
	{
		typedef boost::condition Condition;
	}
}

#	define GENLIB_CONDITION_WAIT( p_condition, p_mutexName)	p_condition.wait( scoped_lock_ ## p_mutexName)
#	define GENLIB_CONDITION_NOTIFY_ONE( p_condition)			p_condition.notify_one()
#	define GENLIB_CONDITION_NOTIFY_ALL( p_condition)			p_condition.notify_all()

#else
#	error Only Boost has Conditions.Edit MultiThreadConfig.h to setup the use of boost
#endif

#endif
