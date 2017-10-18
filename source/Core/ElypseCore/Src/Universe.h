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
#ifndef ___UNIVERSE_H___
#define ___UNIVERSE_H___

#include "Module_Zone.h"

#include <Manager.h>

namespace Elypse
{
	namespace Scene
	{
		class d_dll_export Universe
			: public named
			, private noncopyable
			, public std::enable_shared_from_this< Universe >
		{
		public:
			Universe( String const & p_universeName, String const & p_instanceKey );
			~Universe();

			void AddZone( Zone * p_zone );
			void SetRenderTarget( RenderTarget * p_renderTarget );
			void ClearScene() noexcept;

			inline Camera * GetCamera()const
			{
				return m_camera;
			}
			inline Viewport * GetViewport()const
			{
				return m_viewport;
			}
			inline SceneManager * GetManager()const
			{
				return m_manager;
			}
			inline Zone * GetZone( String const & p_zoneName )const
			{
				return General::Utils::map::findOrNull( m_zones, p_zoneName );
			}

		private:
			SceneManager * m_manager{ nullptr };
			Camera * m_camera{ nullptr };
			RenderTarget * m_renderTarget{ nullptr };
			Viewport * m_viewport{ nullptr };
			//PathMap * m_pathMap{ nullptr };

		public:
			ZoneMap m_zones;
		};

		class UniverseManager
			: public Manager< Universe >
		{
		public:
			UniverseManager( String const & p_instanceKey )
				: m_instanceKey{ p_instanceKey }
			{
			}

			std::shared_ptr< Universe > CreateUniverse( String const & p_name )
			{
				return Manager< Universe >::CreateElement( p_name, m_instanceKey );
			}

		private:
			using Manager< Universe >::CreateElement;

		private:
			String m_instanceKey;
		};
	}
}

#endif
