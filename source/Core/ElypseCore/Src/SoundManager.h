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
#ifndef ___SOUND_MANAGER_H___
#define ___SOUND_MANAGER_H___

#include "Module_Media.h"

#include <Singleton.h>

#include <fmod.hpp>
#include <fmod_errors.h>

#define CHECKFMODERROR( X) SoundManager::CheckFMODError( X, __LINE__, __FILE__)

namespace Elypse
{
	namespace Media
	{
		class SoundManager : public General::Theory::Singleton <SoundManager>
		{
		private:
			FMOD::System * m_system;
			FMOD::ChannelGroup * m_musicGroup;
			FMOD::ChannelGroup * m_SFXGroup;
			FMOD_VECTOR m_listenerPos;
			FMOD_VECTOR m_lastListenerPos;

			SoundObjectMap m_sounds;
			SoundPlaylistMap m_playlists;
			SoundNodeMap m_nodeInstances;

			String m_basePath; //C:/Program Files/FordevStudio/rsc/

			Real m_musicVolume;
			Real m_SFXVolume;

			bool m_musicMuted;
			bool m_SFXMuted;
			bool m_initialised;

		public:
			SoundManager( String const & p_basePath );
			~SoundManager();

		public:
			void AddPlaylist( SoundPlaylist * p_playlist );
			void RemoveSoundObject( String const & p_SOName );
			void RemoveNode( String const & p_nodeName );
			bool IsSoundObject( String const & p_name );
			void Cleanup();
			void StartAll();
			void StopAll();
			void PauseAll();
			void SetVolumePercent( String const & p_nodeName, Real p_volumePercent );
			void Update( const Vector3 & p_position, const Real * p_up, const Real * p_forward, Real p_updateTime );
			void SetVolume( Real p_volume, SoundType p_type );
			void Mute( bool p_mute, SoundType p_type );

			void RegisterInstance( SoundInstance * p_instance );

			SoundObject * CreateSound( String const & p_name );

		private:
			void _logMessage( String const & p_msg );
			void _update( Real p_time );

		public:
			inline Real				GetSFXVolume()const
			{
				return m_SFXVolume;
			}
			inline Real				GetMusicVolume()const
			{
				return m_musicVolume;
			}
			inline bool				GetSFXMuted()const
			{
				return m_SFXMuted;
			}
			inline bool				GetMusicMuted()const
			{
				return m_musicMuted;
			}
			inline String const &	GetBasePath()const
			{
				return m_basePath;
			}
			inline FMOD::System *	GetFMODSystem()const
			{
				return m_system;
			}
			inline const SoundObjectMap &	GetAllSoundsObjects()const
			{
				return m_sounds;
			}
			inline SoundObject *	GetSoundObject( String const & p_name )
			{
				if ( ! m_initialised )
				{
					return NULL;
				}

				return General::Utils::map::findOrNull( m_sounds, p_name );
			}
			inline SoundPlaylist * GetSoundPlaylist( String const & p_name )
			{
				if ( ! m_initialised )
				{
					return NULL;
				}

				return General::Utils::map::findOrNull( m_playlists, p_name );
			}

		public:
			static bool CheckFMODError( FMOD_RESULT p_result, uint32_t p_line, char const * const p_file );
			static const String OutputTypeToStr( FMOD_OUTPUTTYPE p_outputType );

			static inline void LogMessage( String const & p_msg )
			{
				GetSingletonPtr()->_logMessage( p_msg );
			}

			static inline void AddMusicChannel( FMOD::Channel * p_toAdd )
			{
				if ( p_toAdd != NULL )
				{
					CHECKFMODERROR( p_toAdd->setChannelGroup( SoundManager::GetSingletonPtr()->m_musicGroup ) );
				}
			}

			static inline void AddSFXChannel( FMOD::Channel * p_toAdd )
			{
				if ( p_toAdd != NULL )
				{
					CHECKFMODERROR( p_toAdd->setChannelGroup( SoundManager::GetSingletonPtr()->m_SFXGroup ) );
				}
			}
		};
	}
}

#endif
