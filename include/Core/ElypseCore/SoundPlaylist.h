/*
See LICENSE file in root folder
*/
#ifndef ___SOUND_PLAYLIST_H___
#define ___SOUND_PLAYLIST_H___

#include "Module_Media.h"

namespace Elypse
{
	namespace Media
	{
		class d_dll_export SoundPlaylist
		{
		private:
			SoundObjectMap m_objectList;
			EMSoundsPosition m_positions;
			String m_name;
			String m_refName;
			String m_zoneName;
			EMTimerType m_timerType;

			bool m_random;
			bool m_looped;
			bool m_waiting;

			uint32_t m_nbUse;
			int m_currentPlaying;
			SoundType m_type;

			Real m_currentTime;
			Real m_timerTime;
			Real m_timerMin;
			Real m_timerMax;
			Real m_randomTimerTime;
			Real m_volume;
			Real m_musicVolume;
			Real m_SFXVolume;
			Real m_volumePercent;

		private:
			void _randomise();
			void _logMessage( String const & p_msg );
			void _updateTime( Real p_tslf );
			void _update3D( Real p_tslf );

		public:
			SoundPlaylist( String const & p_name, Real p_SFXVolume, Real p_musicVolume, String const & p_zoneName );
			~SoundPlaylist();

			void AddSoundObject( SoundObject * p_soundObject );
			void SetNode( SceneNode * p_node, String const & p_zone );

			void Play();
			void Pause();
			void Stop();
			void Next();

			void ReleaseAllZone( String const & p_zone );
			void Update( Real p_tslf );

			inline void SetRandom( bool p_rand )
			{
				m_random = p_rand;
			}
			inline void SetLooped( bool p_loop )
			{
				m_looped = p_loop;
			}
			inline void SetTimerType( EMTimerType p_type )
			{
				m_timerType = p_type;
			}
			inline void SetTimerTime( Real p_time )
			{
				m_timerTime = p_time;
			}
			inline void SetTimerMin( Real p_time )
			{
				m_timerMin = p_time;
			}
			inline void SetTimerMax( Real p_time )
			{
				m_timerMax = p_time;
			}
			inline void SetVolumePercent( Real p_percent )
			{
				m_volumePercent = minmax <Real> ( 0, p_percent, 1 );
			}

			inline bool				IsRandom()const
			{
				return m_random;
			}
			inline bool				IsLooped()const
			{
				return m_looped;
			}
			inline EMTimerType		GetTimerType()const
			{
				return m_timerType;
			}
			inline Real				GetTimeTime()const
			{
				return m_timerTime;
			}
			inline Real				GetTimerMin()const
			{
				return m_timerMin;
			}
			inline Real				GetTimerMax()const
			{
				return m_timerMax;
			}
			inline Real				GetVolumePercent()const
			{
				return m_volumePercent;
			}
			inline String const &	GetName()const
			{
				return m_name;
			}
			inline uint32_t		GetUse()const
			{
				return m_nbUse;
			}
		};
	}
}

#endif
