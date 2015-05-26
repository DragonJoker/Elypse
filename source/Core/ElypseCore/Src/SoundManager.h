#ifndef ___SOUND_MANAGER_H___
#define ___SOUND_MANAGER_H___

#include "Module_Media.h"

#include <Singleton.h>

#include <fmod.hpp>
#include <fmod_errors.h>

#define CHECKFMODERROR( X) SoundManager::CheckFMODError( X, __LINE__, __FILE__)

namespace EMuse
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
			SoundManager( const String & p_basePath );
			~SoundManager();

		public:
			void AddPlaylist( SoundPlaylist * p_playlist );
			void RemoveSoundObject( const String & p_SOName );
			void RemoveNode( const String & p_nodeName );
			bool IsSoundObject( const String & p_name );
			void Cleanup();
			void StartAll();
			void StopAll();
			void PauseAll();
			void SetVolumePercent( const String & p_nodeName, Real p_volumePercent );
			void Update( const Vector3 & p_position, const Real * p_up, const Real * p_forward, Real p_updateTime );
			void SetVolume( Real p_volume, SoundType p_type );
			void Mute( bool p_mute, SoundType p_type );

			void RegisterInstance( SoundInstance * p_instance );

			SoundObject * CreateSound( const String & p_name );

		private:
			void _logMessage( const String & p_msg );
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
			inline const String &	GetBasePath()const
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
			inline SoundObject *	GetSoundObject( const String & p_name )
			{
				if ( ! m_initialised )
				{
					return NULL;
				}

				return General::Utils::map::findOrNull( m_sounds, p_name );
			}
			inline SoundPlaylist * GetSoundPlaylist( const String & p_name )
			{
				if ( ! m_initialised )
				{
					return NULL;
				}

				return General::Utils::map::findOrNull( m_playlists, p_name );
			}

		public:
			static bool CheckFMODError( FMOD_RESULT p_result, unsigned int p_line, const char * p_file );
			static const String OutputTypeToStr( FMOD_OUTPUTTYPE p_outputType );

			static inline void LogMessage( const String & p_msg )
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
