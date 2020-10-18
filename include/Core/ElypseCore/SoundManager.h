/*
See LICENSE file in root folder
*/
#ifndef ___SOUND_MANAGER_H___
#define ___SOUND_MANAGER_H___

#include "Module_Media.h"
#include "SoundApi.h"

#include <Singleton.h>

namespace Elypse::Media
{
	class SoundManager : public General::Theory::Singleton <SoundManager>
	{
	private:
		api::SoundSystemPtr m_system;

		SoundObjectMap m_sounds;
		SoundPlaylistMap m_playlists;
		SoundNodeMap m_nodeInstances;

		String m_basePath; //C:/Program Files/FordevStudio/rsc/

		Real m_musicVolume;
		Real m_SFXVolume;

		bool m_musicMuted;
		bool m_SFXMuted;

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

		SoundChannelPtr CreateChannel( SoundObject const & object );

	private:
		void _logMessage( String const & p_msg );
		void _update( Real p_time );

	public:
		inline Real GetSFXVolume()const
		{
			return m_SFXVolume;
		}
		inline Real GetMusicVolume()const
		{
			return m_musicVolume;
		}
		inline bool GetSFXMuted()const
		{
			return m_SFXMuted;
		}
		inline bool GetMusicMuted()const
		{
			return m_musicMuted;
		}
		inline String const & GetBasePath()const
		{
			return m_basePath;
		}
		inline api::SoundSystem & GetSystem()const
		{
			return *m_system;
		}
		inline const SoundObjectMap & GetAllSoundsObjects()const
		{
			return m_sounds;
		}
		inline SoundObject * GetSoundObject( String const & p_name )
		{
			if ( !m_system->IsInitialised() )
			{
				return nullptr;
			}

			return General::Utils::map::findOrNull( m_sounds, p_name );
		}
		inline SoundPlaylist * GetSoundPlaylist( String const & p_name )
		{
			if ( !m_system->IsInitialised() )
			{
				return nullptr;
			}

			return General::Utils::map::findOrNull( m_playlists, p_name );
		}

	public:
		static inline void LogMessage( String const & p_msg )
		{
			GetSingletonPtr()->_logMessage( p_msg );
		}

		static inline void AddToMusic( api::SoundChannel const & toAdd )
		{
			GetSingletonPtr()->m_system->AddToMusic( toAdd );
		}

		static inline void AddToSFX( api::SoundChannel const & toAdd )
		{
			GetSingletonPtr()->m_system->AddToSFX( toAdd );
		}
	};
}

#endif
