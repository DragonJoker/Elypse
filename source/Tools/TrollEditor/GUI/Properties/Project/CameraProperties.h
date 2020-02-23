/*
See LICENSE file in root folder
*/
#ifndef ___CameraProperties___
#define ___CameraProperties___

#include "GUI/Properties/Common/ObjectProperty.h"

namespace Troll
{
	namespace GUI
	{
		namespace Properties
		{
			class CameraProperties
				: public ObjectProperty
			{
			public:
				CameraProperties( ProjectComponents::Objects3D::TrollCamera & p_camera );
				~CameraProperties();

			private:
				/**
				 *\copydoc		Troll::GUI::Properties::ObjectProperty::DoCreateProperties
				 */
				virtual void DoCreateProperties( wxPGEditor * p_editor );

			private:
				wxArrayString m_nodesArray;
				ProjectComponents::Objects3D::TrollCamera & m_camera;
			};
		}
	}
}

#endif
