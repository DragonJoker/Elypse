/*
See LICENSE file in root folder
*/
#ifndef ___SoundProperties___
#define ___SoundProperties___

#include "Project/Media/Module_Media.h"

#include "GUI/Properties/Common/ObjectProperty.h"

namespace Troll
{
	namespace GUI
	{
		namespace Properties
		{
			class SoundProperties
				: public ObjectProperty
			{
			public:
				SoundProperties( ProjectComponents::Media::TrollSound & p_sound );
				~SoundProperties();

			private:
				/**
				 *\copydoc		Troll::GUI::Properties::ObjectProperty::DoCreateProperties
				 */
				virtual void DoCreateProperties( wxPGEditor * p_editor );

			private:
				bool OnEditUrl( wxPGProperty * p_property );

			private:
				ProjectComponents::Media::TrollSound & m_sound;
			};
		}
	}
}

#endif
