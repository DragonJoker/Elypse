/*
See LICENSE file in root folder
*/
#ifndef ___VideoObjectProperties___
#define ___VideoObjectProperties___

#include "GUI/Properties/Common/ObjectProperty.h"

namespace Troll
{
	namespace GUI
	{
		namespace Properties
		{
			class VideoObjectProperties
				: public ObjectProperty
			{
			public:
				VideoObjectProperties( ProjectComponents::Media::TrollVideoObject & p_videoObject );
				~VideoObjectProperties();

			private:
				/**
				 *\copydoc		Troll::GUI::Properties::ObjectProperty::DoCreateProperties
				 */
				virtual void DoCreateProperties( wxPGEditor * p_editor );

			private:
				ProjectComponents::Media::TrollVideoObject & m_videoObject;
			};
		}
	}
}

#endif
