/*
See LICENSE file in root folder
*/
#ifndef ___Troll_KeyFrameProperties___
#define ___Troll_KeyFrameProperties___

#include "GUI/Properties/Common/ObjectProperty.h"

namespace Troll
{
	namespace GUI
	{
		namespace Properties
		{
			class KeyFrameProperties
				: public ObjectProperty
			{
			public:
				KeyFrameProperties( ProjectComponents::Temporal::TrollKeyFrame & p_keyFrame );
				~KeyFrameProperties();

			private:
				/**
				*\copydoc		Troll::GUI::Properties::ObjectProperty::DoCreateProperties
				*/
				virtual void DoCreateProperties( wxPGEditor * p_editor );

				void DoBuildArguments();

			private:
				ProjectComponents::Temporal::TrollKeyFrame & m_key;
			};
		}
	}
}

#endif
