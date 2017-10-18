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
