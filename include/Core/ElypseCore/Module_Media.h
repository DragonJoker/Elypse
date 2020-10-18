/*
See LICENSE file in root folder
*/
#ifndef ___MODULE_MEDIA_H___
#define ___MODULE_MEDIA_H___

#include "Elypse.h"
#include "Module_Gui.h"

namespace Elypse
{
	namespace Media
	{
		const uint32_t MAXCHANNEL = 4095;
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

		using VideoObjectMap = std::map< String, VideoObject *>;
		using VideoOverlayMap = std::map< EMOverlay *, VideoOverlay * >;
		using VideoInstanceMap = std::map< Entity *, VideoInstance * >;
		using SoundObjectMap = std::map< String, SoundObject * >;
		using SoundInstanceMap = std::map< String, SoundInstance * >;
		using SoundPlaylistMap = std::map< String, SoundPlaylist * >;
		using SoundInstanceSet = std::set< SoundInstance *>;
		using SoundNodeMap = std::map< String, SoundInstanceSet >;
		using EMSoundsPosition = std::vector< SoundObject * >;

		using VideoFactoryMap = std::map	<String, VideoFactory *>;

		namespace api
		{
			class SoundSystem;
			class Sound;
			class SoundChannel;

			using SoundSystemPtr = std::unique_ptr< SoundSystem >;
			using SoundPtr = std::unique_ptr< Sound >;
			using SoundChannelPtr = std::unique_ptr< SoundChannel >;
		}

		using SoundSystemPtr = api::SoundSystemPtr;
		using SoundPtr = api::SoundPtr;
		using SoundChannelPtr = api::SoundChannelPtr;
	}
}

using namespace Elypse::Media;

#endif
