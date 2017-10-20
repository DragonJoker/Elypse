/*
See LICENSE file in root folder
*/
#ifndef ___VIDEOOBJECT_H___
#define ___VIDEOOBJECT_H___

#include <Url.h>

#include "Module_Media.h"
#include "Module_Main.h"
#include "ZoneObject.h"

namespace Elypse
{
	namespace Media
	{
		class d_dll_export VideoObject : public named, public ZoneObject
		{
			friend class VideoTexture;

		private:
			bool m_isFinished;
			bool m_isPlaying;
			bool m_looped;
			bool m_tokenUrl;
			bool m_autoInitialised;

			uint32_t m_width;
			uint32_t m_height;

			VideoOverlayMap m_overlayMap;
			VideoInstanceMap m_instanceMap;

			int m_maxVolume;		// de 0 à 100
			Real m_volumePercent;	// de 0.0 à 1.0
			bool m_muted;

			Url m_mediaUrl;
			Url m_basePath;

			VideoTexture * m_videoTexture;
			ElypsePlugin * m_plugin;

			VideoImplementation * m_impl;

		public:
			VideoObject( String const & p_name, ElypsePlugin * p_plugin );
			virtual ~VideoObject();

			VideoOverlay * CreateOverlay( EMOverlay * p_overlay );
			VideoInstance * CreateInstance( Entity * p_entity );
			VideoOverlay * GetOverlay( EMOverlay * p_overlay );
			VideoInstance * GetInstance( Entity * p_entity );
			void DeleteOverlay( EMOverlay * p_overlay );
			void DeleteInstance( Entity * p_instance );
			void Update( Real p_time );
			void Initialise();
			void Reset();
			void InitialiseVideoTexture();

			void Start();
			void Pause();
			void Stop();

			void SetMaxVolume( int p_maxVolume );	// 0 < p_maxVolume < 100
			void SetVolumePercent( Real p_percent );
			void Mute( bool p_muted );
		public:
			virtual void AddToZone( Zone * p_zone ) {}
			virtual void RemoveFromZone( Zone * p_zone );

		public:
			inline Url const & GetMediaUrl()const
			{
				return m_mediaUrl;
			}

			inline void Finish()
			{
				m_isFinished = true;
			}

			inline bool IsFinished()const
			{
				return m_isFinished;
			}
			inline bool IsPlaying()const
			{
				return m_isPlaying;
			}
			inline bool IsLooped()const
			{
				return m_looped;
			}
			inline bool IsAutoInitialised()const
			{
				return m_autoInitialised;
			}

			inline uint32_t GetWidth()const
			{
				return m_width;
			}
			inline uint32_t GetHeight()const
			{
				return m_height;
			}

			inline VideoTexture * GetVideoTexture()const
			{
				return m_videoTexture;
			}

			inline void SetSize( uint32_t p_width, uint32_t p_height )
			{
				m_width = p_width;
				m_height = p_height;
			}
			inline void SetUrl( Url const & p_mediaUrl )
			{
				m_mediaUrl = p_mediaUrl;
			}
			inline void SetBasePath( const Path & p_basePath )
			{
				m_basePath = p_basePath;
			}
			inline void SetLooped( bool p_looped )
			{
				m_looped = p_looped;
			}
			inline void SetTokenUrl( Url const & p_mediaUrl )
			{
				m_mediaUrl = p_mediaUrl;
				m_tokenUrl = true;
			}
			inline void SetAutoInitialise( bool p_init )
			{
				m_autoInitialised = p_init;
			}
		};
	}
}

#endif
