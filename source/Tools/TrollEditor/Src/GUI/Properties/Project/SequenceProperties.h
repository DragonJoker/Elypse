/*
See LICENSE file in root folder
*/
#ifndef ___Troll_SequenceProperties___
#define ___Troll_SequenceProperties___

#include "GUI/Properties/Common/ObjectProperty.h"

namespace Troll
{
	namespace GUI
	{
		namespace Properties
		{
			class SequenceProperties
				: public ObjectProperty
			{
			public:
				SequenceProperties( ProjectComponents::Temporal::TrollSequence & p_sequence );
				~SequenceProperties();

			private:
				/**
				*\copydoc		Troll::GUI::Properties::ObjectProperty::DoCreateProperties
				*/
				virtual void DoCreateProperties( wxPGEditor * p_editor );

			private:
				ProjectComponents::Temporal::TrollSequence & m_sequence;
			};
		}
	}
}

#endif

