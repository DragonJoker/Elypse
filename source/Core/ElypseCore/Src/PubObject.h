#ifndef ___PUB_OBJECT_H___
#define ___PUB_OBJECT_H___

#include "Module_Pub.h"

#include "Module_Main.h"
#include "Module_Download.h"
#include "Module_Media.h"
#include "Module_SpecialEffects.h"

#include <PreciseTimer.h>

namespace EMuse
{
	namespace Pub
	{
		class d_dll_export PubObject : public named
		{
		private:
			EMusePlugin * m_plugin;

			PubInstanceMap m_instances;

			String m_imageURL;
			String m_linkURL;
			String m_timestamp;

			bool m_clicked;

			PreciseTimer m_timer;

			WebImage * m_image;

		private:
			void _retrieveUrl();

		public:
			PubObject( const String & p_name, EMusePlugin * p_plugin );
			virtual ~PubObject();

		public:
			PubInstance * CreateInstance( Entity * p_ent );
			void RemoveInstance( PubInstance * p_instance );

			void SetImageUrl( const String & p_url );
			void SetLinkUrl( const String & p_url );
			void SetUrl( const String & p_url );

			void Finalise();

			void Reload();

		public:
			void OnClick();
			void ReleaseAllZone( const String & p_zoneName );

		private:
			static void _reloadCallback( void * p_data );

		public:
			inline bool HasEntity( const String & p_entName )
			{
				return General::Utils::map::has( m_instances, p_entName );
			}
			inline void SetUnclicked()
			{
				m_clicked = false;
			}
			inline void SetImageURL( const String & p_url )
			{
				m_imageURL = p_url;
			}
			inline void SetLinkURL( const String & p_url )
			{
				m_linkURL = p_url;
			}
		};
	}
}
#endif
