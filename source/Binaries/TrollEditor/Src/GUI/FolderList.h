
#ifndef ___Troll_FolderList___
#define ___Troll_FolderList___

#include "Module_GUI.h"
#include <wx/frame.h>

class wxListView;

namespace Troll
{
	namespace GUI
	{
		class FolderList : public wxFrame
		{
		private:
			wxListBox * m_filesList;
			wxString m_folder;

		public:
			FolderList( const wxString & p_folder, wxWindow * p_parent, const wxPoint & p_position = wxPoint( 100, 100 ) );
			~FolderList();

			void RemoveFile( const wxString & p_file );
			void AddFile( const wxString & p_file );

		private:
			void _listFolder();
			void _onQuit( wxCommandEvent & p_event );
			void _onAddFile( wxCommandEvent & p_event );
			void _onDelFile( wxCommandEvent & p_event );
			void _onKeyUp( wxKeyEvent & p_event );

		private:
			DECLARE_EVENT_TABLE()
		};
	}
}

#endif

