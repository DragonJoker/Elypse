#ifndef ___Module_Media___
#define ___Module_Media___

#include <wx/string.h>
#include <map>

namespace Troll
{
	class TrollObject;

	namespace Media
	{
		enum MediaObjectType
		{
			ttSound,
			ttSoundObject,
			ttSoundPlaylist,
			ttVideo,
			ttVideoObject
		};

		enum TrollSoundType
		{
			Sfx = 1,
			Music = 2
		};

		class MediaObject;
		class TrollSoundObject;
		class TrollSound;
		class TrollVideoObject;
		class TrollVideo;

		struct SoundInstance
		{
			float m_attenuation;
			bool m_looped;
			float m_timer;
			float m_fadeInTime;
			float m_fadeOutTime;

			SoundInstance()
				:	m_attenuation( 0.0 ),
					m_looped( false ),
					m_timer( 0.0 ),
					m_fadeInTime( 0.0 ),
					m_fadeOutTime( 0.0 )
			{
			}
		};
	}
}
typedef std::map <wxString, Troll::Media::TrollSound *>			TrollSoundStrMap;
typedef std::map <wxString, Troll::Media::TrollVideo *>			TrollVideoStrMap;
typedef std::map <wxString, Troll::Media::TrollSoundObject *>	TrollSoundObjectMap;
typedef std::map <wxString, Troll::Media::TrollVideoObject *>	TrollVideoObjectMap;

#endif
