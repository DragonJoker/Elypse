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
#ifndef ___ZONE_H___
#define ___ZONE_H___

#include "Module_Zone.h"

#include "Module_Script.h"
#include "Module_Data.h"


namespace Elypse
{
	namespace Scene
	{
		class d_dll_export Zone : public named, noncopyable, public owned_by<Universe>
		{
		private:
			PortalMap m_portals;
			ZoneObjectSet m_objects;

			bool m_unloadAtExit;
			bool m_loaded;

		public:
			bool m_firstInit;

			DataFileArray m_dataFiles;

			ConfigFileArray m_loadScriptFiles;
			ConfigFileArray m_unloadScriptFiles;
			ConfigFileArray m_sceneFiles;
			ScriptNodeArray m_loadScriptNodes;
			ScriptNodeArray m_unloadScriptNodes;

		public:
			Zone( const String & p_zoneName , Universe * p_parentUniverse );
			virtual ~Zone();

		public:
			void AddPortal( Portal * p_portal );
			void AddZoneObject( ZoneObject * p_object );
			void RemoveZoneObject( ZoneObject * p_object );

			virtual void Load();
			virtual void Unload();

			void ClearPortals();

			Portal * GetPortal( const String & p_portalName )const
			{
				return General::Utils::map::findOrNull( m_portals, p_portalName );
			}

		public:
			inline void SetUnloadAtExit( bool p_autoUnload )
			{
				m_unloadAtExit = p_autoUnload;
			}
			inline void AddSceneFile( ConfigFile * p_file )
			{
				m_sceneFiles.push_back( p_file );
			}
			inline void AddLoadScript( ConfigFile * p_file )
			{
				m_loadScriptFiles.push_back( p_file );
				m_loadScriptNodes.push_back( NULL );
			}
			inline void AddUnloadScript( ConfigFile * p_file )
			{
				m_unloadScriptFiles.push_back( p_file );
				m_unloadScriptNodes.push_back( NULL );
			}
			inline void AddDataFile( DataFile * p_dataFile )
			{
				m_dataFiles.push_back( p_dataFile );
			}
		};
	}
}

#endif
