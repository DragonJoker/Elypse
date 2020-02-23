/*
See LICENSE file in root folder
*/
#ifndef ___SceneProperties___
#define ___SceneProperties___

#include "GUI/Properties/Common/ObjectProperty.h"

namespace Troll
{
	namespace GUI
	{
		namespace Properties
		{
			class SceneNodeProperties
				: public ObjectProperty
			{
			public:
				SceneNodeProperties( ProjectComponents::Objects3D::TrollSceneNode & p_node );
				~SceneNodeProperties();

			private:
				/**
				 *\copydoc		Troll::GUI::Properties::ObjectProperty::DoCreateProperties
				 */
				virtual void DoCreateProperties( wxPGEditor * p_editor );

			private:
				ProjectComponents::Objects3D::TrollSceneNode & m_sceneNode;
			};
		}
	}
}

#endif

