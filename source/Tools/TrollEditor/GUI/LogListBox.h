/*
See LICENSE file in root folder
*/
#ifndef ___LOG_LIST_BOX_H___
#define ___LOG_LIST_BOX_H___

#include "GUI/TrollEditorGuiPrerequisites.h"

#include <wx/log.h>
#include <wx/listbox.h>

namespace Troll
{
	namespace GUI
	{
		// A log target which just redirects the messages to a listbox
		class wxLogListBox
			: public wxLog
		{
		public:
			wxLogListBox( wxListBox * p_list );

			virtual ~wxLogListBox();

		private:
			// implement sink functions
			virtual void DoLogTextAtLevel( wxLogLevel p_level, wxString const & p_msg );

			virtual void DoLogText( wxString const & p_msg );

		private:
			// the control we use
			wxListBox * m_list;
		};
	}
}

#endif
