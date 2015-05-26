#ifndef ___VIDEOMANAGER_H___
#define ___VIDEOMANAGER_H___

#include <Singleton.h>

#include "Module_Media.h"
#include "Module_Main.h"

#include <PreciseTimer.h>

namespace EMuse
{
	namespace Media
	{
		class d_dll_export VideoManager : public General::Theory::Singleton <VideoManager>
		{
			friend class VideoImplementation;

		private:
			VideoObjectMap m_objectList;
			VideoFactoryMap m_factories;
			int m_volume;
			bool m_muted;

			PreciseTimer m_timer;

			VideoFactory * m_currentFactory;

		public:
			VideoManager();
			~VideoManager();

		public:
			VideoObject * CreateVideoObject( const String & p_name, EMusePlugin * p_plugin );
			bool IsVideo( const String & p_entityName );
			void Cleanup();
			void StartAll();
			void StopAll();
			void PauseAll();
			void SetVolume( int p_volume );
			void Mute( bool p_muted );
//		void UpdateVolume( const Vector3 & p_position,  Radian p_orientation);
//		void ReleaseAllZone( const String & p_zoneName);
			void DeleteVideoObject( const String & p_name );

			void RegisterFactory( VideoFactory * p_factory );
			void SetCurrentFactory( const String & p_name );

		public:
			void Update();

		public:
			inline bool						IsMuted()const
			{
				return m_muted;
			}
			inline Real						GetVolume()const
			{
				return Real( m_volume );
			}
			inline VideoObject 	*		GetVideoObject( const String & p_name )const
			{
				return General::Utils::map::findOrNull( m_objectList, p_name );
			}
			inline VideoFactory 	*		GetCurrentFactory()const
			{
				return m_currentFactory;
			}
			inline const VideoObjectMap &	GetVideoMap()const
			{
				return m_objectList;
			}
		};
	}
}

#endif
