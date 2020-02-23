/*
See LICENSE file in root folder
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

			void AddZone( ZonePtr p_zone );
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
