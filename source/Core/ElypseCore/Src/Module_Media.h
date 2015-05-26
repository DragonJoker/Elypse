#ifndef ___MODULE_MEDIA_H___
#define ___MODULE_MEDIA_H___

#include "EMuse.h"
#include "Module_Gui.h"

namespace FMOD
{
	class System;
	class ChannelGroup;
	class Channel;
	class Sound;
}

namespace EMuse
{
	namespace Media
	{
		const unsigned int MAXCHANNEL = 4095;
		const Real MINDISTSOUND = 4.0;
		const Real MAXDISTSOUND = 10000.0;
		const Real MINVOLUME = 0.0;
		const Real MAXVOLUME = 1.0;

		enum EMTimerType
		{
			TT_Random,
			TT_Fixed
		};

		enum SoundType
		{
			EM_NONE = -1,
			EM_MUSIC = 1,
			EM_SFX = 2
		};

		enum MediaStatus
		{
			EM_STATUS_NONE = 0,
			EM_STATUS_INIT = 1,
			EM_STATUS_READY = 2,
			EM_STATUS_LOOP = 3,
			EM_STATUS_PLAY = 4,
			EM_STATUS_PAUSE = 5,
			EM_STATUS_STOP = 6
		};

		class VideoManager;
		class VideoObject;
		class VideoOverlay;
		class VideoInstance;
		class VideoTexture;
		class VideoFactory;

		class SoundManager;
		class SoundObject;
		class SoundPlaylist;
		class SoundInstance;

		class VideoImplementation;

		typedef std::map	<String, VideoObject *>			VideoObjectMap;
		typedef std::map	<EMOverlay *, VideoOverlay *>	VideoOverlayMap;
		typedef std::map	<Entity *, VideoInstance *>		VideoInstanceMap;
		typedef std::map	<String, SoundObject *>			SoundObjectMap;
		typedef std::map	<String, SoundInstance *>		SoundInstanceMap;
		typedef std::map	<String, SoundPlaylist *>		SoundPlaylistMap;
		typedef std::set	<SoundInstance *>				SoundInstanceSet;
		typedef std::map	<String, SoundInstanceSet>		SoundNodeMap;
		typedef std::vector	<SoundObject *>					EMSoundsPosition;

		typedef std::map	<String, VideoFactory *>			VideoFactoryMap;
	}
}

using namespace EMuse::Media;

#endif
