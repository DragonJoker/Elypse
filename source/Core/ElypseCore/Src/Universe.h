/*****************************************************************************

	Author : Marc BILLON
	Compagny : FDSSoftMedia - Copyright 2007

*****************************************************************************/

#ifndef ___UNIVERSE_H___
#define ___UNIVERSE_H___

#include "Module_Zone.h"

#include <Manager.h>

namespace EMuse
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
			void __declspec( nothrow ) ClearScene();

		public:
			inline Camera 	*		GetCamera()const
			{
				return m_camera;
			}
			inline Viewport 	*	GetViewport()const
			{
				return m_viewport;
			}
			inline SceneManager *	GetManager()const
			{
				return m_manager;
			}
			//inline PathMap * GetPathMap()const					{ return m_pathMap; }
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
				:	m_instanceKey( p_instanceKey )
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
