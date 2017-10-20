/*
See LICENSE file in root folder
*/
#ifndef ___SoundObjectProperties___
#define ___SoundObjectProperties___

#include "GUI/Properties/Common/ObjectProperty.h"

namespace Troll
{
	namespace GUI
	{
		namespace Properties
		{
			class SoundObjectProperties
				: public ObjectProperty
			{
			public:
				SoundObjectProperties( ProjectComponents::Media::TrollSoundObject & p_soundObject );
				~SoundObjectProperties();

			private:
				/**
				 *\copydoc		Troll::GUI::Properties::ObjectProperty::DoCreateProperties
				 */
				virtual void DoCreateProperties( wxPGEditor * p_editor );

			private:
				ProjectComponents::Media::TrollSoundObject & m_soundObject;
			};
		}
	}
}

#endif
