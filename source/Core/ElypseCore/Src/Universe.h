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
		class d_dll_export Universe : public named, noncopyable
		{
		private:
			SceneManager * m_manager;
			Camera * m_camera;
			RenderTarget * m_renderTarget;
			Viewport * m_viewport;

			//PathMap * m_pathMap;

		public:
			ZoneMap m_zones;

		public:
			Universe( const String & p_universeName, const String & p_instanceKey );
			~Universe();

		public:
			void AddZone( Zone * p_zone );
			void SetRenderTarget( RenderTarget * p_renderTarget );
			void ClearScene() d_no_throw;

		public:
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
			inline Zone * GetZone( const String & p_zoneName )const
			{
				return General::Utils::map::findOrNull( m_zones, p_zoneName );
			}
		};

		class UniverseManager : public Manager<Universe>
		{
			String m_instanceKey;
		public:
			UniverseManager( const String & p_instanceKey )
				: m_instanceKey( p_instanceKey )
			{
			}
			~UniverseManager()
			{}

		public:
			Universe * CreateUniverse( const String & p_name )
			{
				return General::Utils::map::insert( m_objectMap, p_name, p_name, m_instanceKey );
			}
		};
	}
}

#endif
