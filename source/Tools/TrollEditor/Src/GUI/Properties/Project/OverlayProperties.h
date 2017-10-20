/*
See LICENSE file in root folder
*/
#ifndef ___OverlayProperties___
#define ___OverlayProperties___

#include "GUI/Properties/Common/ObjectProperty.h"

namespace Troll
{
	namespace GUI
	{
		namespace Properties
		{
			class OverlayProperties
				: public ObjectProperty
			{
			public:
				OverlayProperties( ProjectComponents::Objects2D::TrollOverlay & p_overlay );
				~OverlayProperties();

			private:
				/**
				 *\copydoc		Troll::GUI::Properties::ObjectProperty::DoCreateProperties
				 */
				virtual void DoCreateProperties( wxPGEditor * p_editor );

			private:
				void DoCreateTextProperties( wxPGEditor * p_editor );
				void DoCreateBorderProperties( wxPGEditor * p_editor );
				wxArrayString DoGetVideosNames();

			private:
				ProjectComponents::Objects2D::TrollOverlay & m_overlay;
			};
		}
	}
}

#endif
