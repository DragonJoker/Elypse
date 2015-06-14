/*
This source file is part of ElypsePlayer (https://sourceforge.net/projects/elypse/)

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
the program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
*/
#ifndef ___Module_Media___
#define ___Module_Media___

#include "Project/Module_Project.h"

#include <wx/string.h>
#include <map>

#define TROLL_PROJECT_MEDIA_NAMESPACE TROLL_PROJECT_NAMESPACE::Media
#define BEGIN_TROLL_PROJECT_MEDIA_NAMESPACE BEGIN_TROLL_PROJECT_NAMESPACE { namespace Media
#define END_TROLL_PROJECT_MEDIA_NAMESPACE END_TROLL_PROJECT_NAMESPACE }

BEGIN_TROLL_PROJECT_NAMESPACE
{
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
				: m_attenuation( 0.0 )
				, m_looped( false )
				, m_timer( 0.0 )
				, m_fadeInTime( 0.0 )
				, m_fadeOutTime( 0.0 )
			{
			}
		};

		typedef std::map< wxString, TrollSound * > TrollSoundStrMap;
		typedef std::map< wxString, TrollVideo * > TrollVideoStrMap;
		typedef std::map< wxString, TrollSoundObject * > TrollSoundObjectMap;
		typedef std::map< wxString, TrollVideoObject * > TrollVideoObjectMap;
	}
}
END_TROLL_PROJECT_NAMESPACE

#endif
