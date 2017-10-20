/*
See LICENSE file in root folder
*/
#ifndef ___ProjectProperties___
#define ___ProjectProperties___

#include "GUI/Properties/Common/ObjectProperty.h"

namespace Troll
{
	namespace GUI
	{
		namespace Properties
		{
			class ProjectProperties
				: public ObjectProperty
			{
			public:
				ProjectProperties( ProjectComponents::Project & p_project );
				~ProjectProperties();

			private:
				/**
				 *\copydoc		Troll::GUI::Properties::ObjectProperty::DoCreateProperties
				 */
				virtual void DoCreateProperties( wxPGEditor * p_editor );

			private:
				bool DoSelectBackground( wxPGProperty * p_property );
				bool DoChangeStartupScript( wxPGProperty * p_property );

			protected:
				ProjectComponents::Project & m_project;
			};
		}
	}
}

#endif
