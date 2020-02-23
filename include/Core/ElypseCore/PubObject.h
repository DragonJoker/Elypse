/*
See LICENSE file in root folder
*/
#ifndef ___PUB_OBJECT_H___
#define ___PUB_OBJECT_H___

#include "Module_Pub.h"

#include "Module_Main.h"
#include "Module_Download.h"
#include "Module_Media.h"
#include "Module_SpecialEffects.h"

#include <PreciseTimer.h>

namespace Elypse
{
	namespace Pub
	{
		class d_dll_export PubObject : public named
		{
		private:
			ElypsePlugin * m_plugin;

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
			PubObject( String const & p_name, ElypsePlugin * p_plugin );
			virtual ~PubObject();

		public:
			PubInstance * CreateInstance( Entity * p_ent );
			void RemoveInstance( PubInstance * p_instance );

			void SetImageUrl( String const & p_url );
			void SetLinkUrl( String const & p_url );
			void SetUrl( String const & p_url );

			void Finalise();

			void Reload();

		public:
			void OnClick();
			void ReleaseAllZone( String const & p_zoneName );

		private:
			static void _reloadCallback( void * p_data );

		public:
			inline bool HasEntity( String const & p_entName )
			{
				return General::Utils::map::has( m_instances, p_entName );
			}
			inline void SetUnclicked()
			{
				m_clicked = false;
			}
			inline void SetImageURL( String const & p_url )
			{
				m_imageURL = p_url;
			}
			inline void SetLinkURL( String const & p_url )
			{
				m_linkURL = p_url;
			}
		};
	}
}
#endif
