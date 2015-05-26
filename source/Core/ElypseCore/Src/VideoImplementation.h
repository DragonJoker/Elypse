#ifndef ___VIDEO_IMPLEMENTATION_H___
#define ___VIDEO_IMPLEMENTATION_H___

#include "Module_Media.h"
#include <Url.h>

namespace EMuse
{
	namespace Media
	{
		class d_dll_export VideoImplementation : public owned_by<VideoObject>
		{
		public:
			VideoImplementation( VideoObject * p_owner )
				:	owned_by <VideoObject> ( p_owner )
			{
			}
			virtual ~VideoImplementation()
			{
			}

		public:
			virtual bool Start() = 0;
			virtual bool Pause() = 0;
			virtual bool Stop() = 0;
			virtual void Reset() = 0;
			virtual void Initialise( const Url & p_mediaUrl ) = 0;
			virtual bool Update( Real p_time ) = 0;
			virtual void SetMaxVolume( int p_maxVolume ) = 0;	//  0 < p_maxVolume < 100
			virtual void SetVolumePercent( Real p_percent ) = 0;
			virtual void Mute( bool p_muted ) = 0;
		};
	}
}

#endif
