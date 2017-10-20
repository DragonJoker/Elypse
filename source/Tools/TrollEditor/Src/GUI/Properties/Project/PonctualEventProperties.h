/*
See LICENSE file in root folder
*/
#ifndef ___Troll_PonctualEventProperties___
#define ___Troll_PonctualEventProperties___

#include "GUI/Properties/Common/ObjectProperty.h"

namespace Troll
{
	namespace GUI
	{
		namespace Properties
		{
			class PonctualEventProperties
				: public ObjectProperty
			{
			public:
				PonctualEventProperties( ProjectComponents::Temporal::TrollPonctualEvent & p_event );
				~PonctualEventProperties();

			private:
				/**
				 *\copydoc		Troll::GUI::Properties::ObjectProperty::DoCreateProperties
				 */
				virtual void DoCreateProperties( wxPGEditor * p_editor );

			protected:
				wxArrayString DoGetTargetTypes( wxString const & p_action );
				void DoCreateValueProperties();

			private:
				void DoUpdateTargetType();
				void DoUpdateTarget();
				wxArrayString DoGetTargetsList();

			private:
				ProjectComponents::Temporal::TrollPonctualEvent & m_event;
				wxPGProperty * m_targetType{ nullptr };
				wxPGProperty * m_target{ nullptr };
			};
		}
	}
}

#endif
