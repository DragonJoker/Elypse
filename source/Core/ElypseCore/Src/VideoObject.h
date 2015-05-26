#ifndef ___VIDEOOBJECT_H___
#define ___VIDEOOBJECT_H___

#include <Url.h>

#include "Module_Media.h"
#include "Module_Main.h"
#include "ZoneObject.h"

namespace EMuse
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

			unsigned int m_width;
			unsigned int m_height;

			VideoOverlayMap m_overlayMap;
			VideoInstanceMap m_instanceMap;

			int m_maxVolume;		// de 0 à 100
			Real m_volumePercent;	// de 0.0 à 1.0
			bool m_muted;

			Url m_mediaUrl;
			Url m_basePath;

			VideoTexture * m_videoTexture;
			EMusePlugin * m_plugin;

			VideoImplementation * m_impl;

		public:
			VideoObject( const String & p_name, EMusePlugin * p_plugin );
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
			inline const Url & GetMediaUrl()const
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

			inline unsigned int GetWidth()const
			{
				return m_width;
			}
			inline unsigned int GetHeight()const
			{
				return m_height;
			}

			inline VideoTexture * GetVideoTexture()const
			{
				return m_videoTexture;
			}

			inline void SetSize( unsigned int p_width, unsigned int p_height )
			{
				m_width = p_width;
				m_height = p_height;
			}
			inline void SetUrl( const Url & p_mediaUrl )
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
			inline void SetTokenUrl( const Url & p_mediaUrl )
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
