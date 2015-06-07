#ifndef ___LOG_LIST_BOX_H___
#define ___LOG_LIST_BOX_H___

#include <wx/log.h>
#include <wx/listbox.h>

// A log target which just redirects the messages to a listbox
class wxLogListBox
	: public wxLog
{
public:
	wxLogListBox( wxListBox * p_list );

	virtual ~wxLogListBox();

private:
	// implement sink functions
	virtual void DoLog( wxLogLevel p_level, const wxChar * p_line, time_t p_time );

	virtual void DoLogString( const wxChar * p_line, time_t p_time );

	// the control we use
	wxListBox * m_list;
};

#endif
