#ifndef ___PUB_MANAGER_H___
#define ___PUB_MANAGER_H___

#include "Module_Pub.h"
#include "Module_Physics.h"
#include "Module_Main.h"

#include <Singleton.h>

namespace EMuse
{
	namespace Pub
	{
		class d_dll_export PubManager : public General::Theory::Singleton<PubManager>
		{
		private:
			PubObjectMap m_objectList;

		public:
			PubManager();
			~PubManager();

		public:
			PubObject * CreatePubObject( const String & p_name, EMusePlugin * p_plugin );

			void RemovePubObject( const String & p_name );

			void OnClick( Real p_x, Real p_y, SceneManager * p_sceneManager, Camera * p_camera, PhysicsSimulation * p_physics );

		public:
			inline PubObject * GetPubObject( const String & p_name )
			{
				return General::Utils::map::findOrNull( m_objectList, p_name );
			}
		};
	}
}

#endif
