#ifndef ___VIDEO_FACTORY_H___
#define ___VIDEO_FACTORY_H___

#include "Module_Media.h"

namespace EMuse
{
	namespace Media
	{
		class d_dll_export VideoFactory : public named
		{
		public:
			VideoFactory( const String & p_name )
				:	named( p_name )
			{
			}
			virtual void Initialise() = 0;
			virtual VideoImplementation * Create( VideoObject * p_owner ) = 0;
			virtual void Update() = 0;
		};
	}
}

#endif
