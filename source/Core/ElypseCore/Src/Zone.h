/*****************************************************************************

	Author : Marc BILLON
	Compagny : FDSSoftMedia - Copyright 2007

*****************************************************************************/

#ifndef ___ZONE_H___
#define ___ZONE_H___

#include "Module_Zone.h"

#include "Module_Script.h"
#include "Module_Data.h"


namespace EMuse
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
