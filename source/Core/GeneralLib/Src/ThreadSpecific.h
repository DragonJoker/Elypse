#ifndef ___THREAD_SPECIFIC_H___
#define ___THREAD_SPECIFIC_H___

#include <boost/thread/tss.hpp>

namespace General
{
	namespace MultiThreading
	{
		typedef boost::thread_specific_ptr <unsigned int> ThreadSpecificUIntPtr;
	}
}

#endif
