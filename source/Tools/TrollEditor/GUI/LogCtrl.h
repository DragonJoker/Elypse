/*
See LICENSE file in root folder
*/
#ifndef ___Troll_LogCtrl___
#define ___Troll_LogCtrl___

#include "GUI/TrollEditorGuiPrerequisites.h"

#include <wx/listbox.h>

namespace Troll::GUI
{
	class LogCtrl
		: public wxListBox
	{
	public:
		LogCtrl( wxWindow * parent, wxWindowID id, wxPoint const & pos, wxSize const & size, long style );

	private:
		DECLARE_EVENT_TABLE()
		void OnDoubleClick( wxMouseEvent & p_event );
	};

	class LogFile
		: public wxLog
	{
	public:
		LogFile( wxString const & p_fileName );

	protected:
		// this function is not pure virtual as it might not be needed if you do
		// the logging in overridden DoLogRecord() or DoLogTextAtLevel() directly
		// but if you do not override them in your derived class you must override
		// this one as the default implementation of it simply asserts
		void DoLogText( const wxString & msg )override;

	private:
		wxString m_fileName;
		std::mutex m_mutex;
	};
}

#endif
