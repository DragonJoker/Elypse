/*
See LICENSE file in root folder
*/
#ifndef ___LightProperties___
#define ___LightProperties___

#include "GUI/Properties/Common/ObjectProperty.h"

#include "Project/3D/Module_3D.h"

namespace Troll
{
	namespace GUI
	{
		namespace Properties
		{
			class LightProperties
				: public ObjectProperty
			{
			public:
				LightProperties( ProjectComponents::Objects3D::TrollLight & p_light );
				~LightProperties();

			private:
				/**
				 *\copydoc		Troll::GUI::Properties::ObjectProperty::DoCreateProperties
				 */
				virtual void DoCreateProperties( wxPGEditor * p_editor );

			private:
				ProjectComponents::Objects3D::TrollLight & m_light;
			};
		}
	}
}

#endif
