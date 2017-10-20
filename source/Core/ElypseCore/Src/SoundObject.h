/*
See LICENSE file in root folder
*/
#ifndef ___SOUND_OBJECT_H___
#define ___SOUND_OBJECT_H___

#include <fmod.hpp>

#include "Module_Media.h"
#include "ZoneObject.h"

namespace Elypse
{
	namespace Media
	{
		class d_dll_export SoundObject : public named, public ZoneObject
		{
		public:
			SoundInstanceMap m_instances;

		private:
			String m_soundURL;
			String m_zone;

			FMOD::Sound * m_sound;
			SoundInstance * m_globalInstance;

			uint32_t m_useCount;
			SoundType m_type;

			bool m_looped;
			bool m_muted;
			bool m_global;

			Real m_mediaTime;
			Real m_maxVolume;
			Real m_volumePercent;

		public:
			SoundObject( String const & p_name );
			virtual ~SoundObject();

		public:
			SoundInstance * CreateInstance( SceneNode * p_node );
			void SetSoundUrl( String const & p_url );
			void RemoveNode( String const & p_entName );
			void PlayAll();
			void PauseAll();
			void StopAll();
			void Update3D( Real p_time );
			void UpdateFade( Real p_time );
			void SetLocal( String const & p_zoneName );

			void SetMaxVolume( Real p_volume );
			void Mute( bool p_mute );

			void Initialise();

			virtual void AddToZone( Zone * p_zone );
			virtual void RemoveFromZone( Zone * p_zone );

		public:
			inline uint32_t		GetUseCount() const
			{
				return m_useCount;
			}
			inline bool				IsGlobal() const
			{
				return m_globalInstance != NULL;
			}
			inline String const &	GetSoundURL() const
			{
				return m_soundURL;
			}
			inline SoundType		GetSoundType() const
			{
				return m_type;
			}
			inline FMOD::Sound *	GetFMODSound() const
			{
				return m_sound;
			}
			inline bool				IsLooped() const
			{
				return m_looped;
			}
			inline bool				IsMuted() const
			{
				return m_muted;
			}
			inline SoundInstance *	GetGlobal() const
			{
				return m_globalInstance;
			}
			inline void				SetSoundType( SoundType p_type )
			{
				m_type = p_type;
			}
			inline void				SetLooped( bool p_looped )
			{
				m_looped = p_looped;
			}

			inline void SetVolumePercent( Real p_volume )
			{
				m_volumePercent = p_volume;
			}

			inline SoundInstance * GetSoundInstance( String const & p_name )
			{
				return General::Utils::map::findOrNull( m_instances, p_name );
			}
		};
	}
}

#endif
