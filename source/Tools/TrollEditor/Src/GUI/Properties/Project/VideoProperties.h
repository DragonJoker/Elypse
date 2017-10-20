/*
See LICENSE file in root folder
*/
#ifndef ___VideoProperties___
#define ___VideoProperties___

#include "GUI/Properties/Common/ObjectProperty.h"

namespace Troll
{
	namespace GUI
	{
		namespace Properties
		{
			class VideoProperties
				: public ObjectProperty
			{
			public:
				VideoProperties( ProjectComponents::Media::TrollVideo & p_video );
				~VideoProperties();

			private:
				/**
				 *\copydoc		Troll::GUI::Properties::ObjectProperty::DoCreateProperties
				 */
				virtual void DoCreateProperties( wxPGEditor * p_editor );

			private:
				bool OnEditUrl( wxPGProperty * p_property );

			private:
				ProjectComponents::Media::TrollVideo & m_video;
			};
		}
	}
}

#endif
