#ifndef ___BARRIER_H___
#define ___BARRIER_H___

#include "MultiThreadConfig.h"

#if GENLIB_MT_USE_BOOST
#	include <boost/thread/barrier.hpp>

namespace General
{
	namespace MultiThreading
	{
		typedef boost::barrier Barrier;
	}
}

#	define GENLIB_CREATE_BARRIER( p_count)		new General::MultiThreading::Barrier( p_count)
#	define GENLIB_DESTROY_BARRIER( p_barrier)	delete p_barrier
#	define GENLIB_WAIT_AT_BARRIER( p_barrier)	p_barrier->wait()

#else
#	error Only Boost has Barriers. Edit MultiThreadConfig.h to setup the use of boost
#endif

#endif
