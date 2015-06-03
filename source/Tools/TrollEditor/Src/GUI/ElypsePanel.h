
#ifndef ___ElypsePanel___
#define ___ElypsePanel___

#include <wx/panel.h>

#include "Module_GUI.h"

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
			ElypsePanel( wxWindow * p_parent, const wxPoint & p_position, const wxSize & p_size )
				:	wxPanel( p_parent, wxID_ANY, p_position, p_size, wxWANTS_CHARS )
			{
			}
			~ElypsePanel()
			{
			}
		};
	}
}

#endif

