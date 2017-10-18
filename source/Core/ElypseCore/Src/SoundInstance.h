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
#ifndef ___MediaEntity_H___
#define ___MediaEntity_H___

#include <fmod.hpp>

#include "Module_Media.h"
#include "SoundManager.h"

namespace Elypse
{
	namespace Media
	{
		class d_dll_export SoundInstance
			: public owned_by< SoundObject >
		{
		public:
			SoundInstance( SoundObject & p_parent );
			~SoundInstance();

		private:
			void _setVolume();
			void _set3D();
		public:
			void SetLastPosition();
			void Update3D( Real p_time );
			void UpdateFade( Real p_time, bool p_muted );

			void Pause();
			void Play();
			void Stop();

			void SetNode( SceneNode * p_node );
			void SetLooped( bool p_looped );
			void SetVelocity( const Vector3 & p_velocity );

			void Mute( bool p_mute );

		public:
			inline void SetMaxVolume( Real p_maxVolume )
			{
				m_maxVolume = p_maxVolume;
				_setVolume();
			}
			inline void SetVolumePercent( Real p_volumePercent )
			{
				m_volumePercent = p_volumePercent;
				_setVolume();
			}
			inline void SetFadeIn( Real p_fade )
			{
				m_fadeInTime = p_fade;
			}
			inline void SetFadeOut( Real p_fade )
			{
				m_fadeOutTime = p_fade;
			}
			inline void SetTimerTime( Real p_time )
			{
				m_timerTime = p_time;
			}
			inline void SetInPlaylist( bool p_bool )
			{
				m_isInPlaylist = true;
			}

			inline bool IsPlaying()const
			{
				return m_status == EM_STATUS_PLAY;
			}
			inline bool IsLooped()const
			{
				return m_looped;
			}
			inline bool Is3D()const
			{
				return m_node != NULL;
			}
			inline SceneNode * GetNode()const
			{
				return m_node;
			}
			inline Real GetVolumePercent()const
			{
				return m_volumePercent;
			}
			inline Real GetFadeIn()const
			{
				return m_fadeInTime;
			}
			inline Real GetFadeOut()const
			{
				return m_fadeOutTime;
			}
			inline FMOD::Channel * GetChannel()const
			{
				return m_channel;
			}
			inline bool IsInPlaylist()const
			{
				return m_isInPlaylist;
			}
			inline Real GetTimerTime()const
			{
				return m_timerTime;
			}

		protected:
			SceneNode * m_node;
			FMOD::Channel * m_channel;

			MediaStatus m_status;
			String m_zone;

			bool m_looped;
			bool m_fadeIn;
			bool m_fadeOut;
			bool m_isInPlaylist;
			bool m_mute;

			Real m_fadeInTime;
			Real m_fadeOutTime;
			Real m_mediaTime;
			Real m_currentTime;
			Real m_timerTime;
			Real m_volumePercent;
			Real m_maxVolume;
			Real m_volume;

			FMOD_VECTOR m_lastPosition;
		};
	}
}

#endif
