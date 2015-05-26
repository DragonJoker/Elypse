#ifndef ___THREAD_UNIQUE_H___
#define ___THREAD_UNIQUE_H___

#include <boost/thread/tss.hpp>

#include "NonCopyable.h"

namespace General
{
	namespace Theory
	{
		template <typename T>
		class ThreadUnique : d_noncopyable
		{
		public:
			static T * GetInstancePtr()
			{
				static boost::thread_specific_ptr <T> sl_singleton;

				if ( sl_singleton.get() == NULL )
				{
					sl_singleton.reset( new T );
				}

				return sl_singleton.get();
			}
			static T & GetInstance()
			{
				return * GetInstancePtr();
			}
		};
	}
}

#endif
