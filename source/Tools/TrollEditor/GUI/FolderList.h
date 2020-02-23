/*
See LICENSE file in root folder
*/
#ifndef ___Troll_FolderList___
#define ___Troll_FolderList___

#include "GUI/TrollEditorGuiPrerequisites.h"

#include <wx/dialog.h>

class wxListBox;

namespace Troll
{
	namespace GUI
	{
		class FolderList
			: public wxDialog
		{
		private:
			wxListBox * m_filesList;
			wxString m_folder;

		public:
			FolderList( wxString const & p_folder, wxWindow * p_parent, wxPoint const & p_position = wxPoint( 100, 100 ) );
			~FolderList();

			void RemoveFile( wxString const & p_file );
			void AddFile( wxString const & p_file );

		private:
			void _listFolder();
			void OnQuit( wxCommandEvent & p_event );
			void OnAddFile( wxCommandEvent & p_event );
			void OnDelFile( wxCommandEvent & p_event );
			void OnKeyUp( wxKeyEvent & p_event );

		private:
			DECLARE_EVENT_TABLE()
		};
	}
}

#endif
