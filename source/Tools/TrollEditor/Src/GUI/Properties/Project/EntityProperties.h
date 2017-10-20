/*
See LICENSE file in root folder
*/
#ifndef ___EntityProperties___
#define ___EntityProperties___

#include "GUI/Properties/Common/ObjectProperty.h"

namespace Troll
{
	namespace GUI
	{
		namespace Properties
		{
			class EntityProperties
				: public ObjectProperty
			{
			public:
				EntityProperties( ProjectComponents::Objects3D::TrollEntity & p_entity );
				~EntityProperties();

			private:
				/**
				 *\copydoc		Troll::GUI::Properties::ObjectProperty::DoCreateProperties
				 */
				virtual void DoCreateProperties( wxPGEditor * p_editor );

				void DoUpdateMeshMaterials();
				void DoRemoveMaterialsProperties();

			private:
				ProjectComponents::Objects3D::TrollEntity & m_entity;
			};
		}
	}
}

#endif
