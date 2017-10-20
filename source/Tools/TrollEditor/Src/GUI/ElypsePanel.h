/*
See LICENSE file in root folder
*/
#ifndef ___ElypsePanel___
#define ___ElypsePanel___

#include "GUI/TrollEditorGuiPrerequisites.h"

#include <wx/panel.h>

namespace Troll
{
	namespace GUI
	{
		class ElypsePanel
			: public wxPanel
		{
		private:
			ElypseCtrl * m_elypseCtrl;

		public:
			ElypsePanel( wxWindow * p_parent, wxPoint const & p_position, wxSize const & p_size )
				: wxPanel( p_parent, wxID_ANY, p_position, p_size, wxWANTS_CHARS )
			{
				SetBackgroundColour( GuiCommon::PanelBackgroundColour );
				SetForegroundColour( GuiCommon::PanelForegroundColour );
			}

			~ElypsePanel()
			{
			}
		};
	}
}

#endif

