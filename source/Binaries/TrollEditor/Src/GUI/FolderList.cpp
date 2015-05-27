
#include "PrecompiledHeader.h"

#include "GUI/FolderList.h"
/*
#include <boost/filesystem.hpp>
#include <General/File.h>
*/

using namespace Troll;
using namespace Troll::GUI;
using namespace boost::filesystem;

enum FolderListIDs
{
	Liste,
	Quitter,
	Supprimer,
	Ajouter,
};

BEGIN_EVENT_TABLE( FolderList, wxFrame )
	EVT_BUTTON(	Quitter,	FolderList::_onQuit )
	EVT_BUTTON(	Ajouter,	FolderList::_onAddFile )
	EVT_BUTTON(	Supprimer,	FolderList::_onDelFile )
	EVT_KEY_UP(	FolderList::_onKeyUp )
END_EVENT_TABLE()


FolderList :: FolderList( const wxString & p_folder, wxWindow * p_parent, const wxPoint & p_position )
	:	wxFrame( p_parent, wxID_ANY, wxT( "Liste" ), p_position, wxSize( 200, 350 ), wxFRAME_TOOL_WINDOW | wxCAPTION | wxCLOSE_BOX | wxFRAME_FLOAT_ON_PARENT ),
		m_folder( p_folder ),
		m_filesList( NULL )
{
//	std::cout << "FolderList :: FolderList - " << m_folder << "\n";
	wxSize l_size = GetClientSize();
	int l_iBtnCx = 80;
	int l_iBtnCy = 25;
	int l_iOffset = 5;
	int l_iBtnX = l_iOffset;
	int l_iBtnY = l_size.y - ( l_iOffset + l_iBtnCy ) * 2;
	wxButton * l_buttonDel 	= new wxButton( this, Supprimer,	wxT( "Supprimer"	), wxPoint( l_iBtnX, l_iBtnY	), wxSize( l_iBtnCx, l_iBtnCy ), wxBORDER_SIMPLE );
	l_iBtnX = l_size.x - l_iBtnCx - l_iOffset;
	wxButton * l_buttonAdd 	= new wxButton( this, Ajouter, 		wxT( "Ajouter"	), wxPoint( l_iBtnX, l_iBtnY	), wxSize( l_iBtnCx, l_iBtnCy ), wxBORDER_SIMPLE );
	l_iBtnX = ( l_size.x - l_iBtnCx ) / 2;
	l_iBtnY += l_iOffset + l_iBtnCy;
	wxButton * l_buttonQuit	= new wxButton( this, Quitter, 		wxT( "Retour"	), wxPoint( l_iBtnX, l_iBtnY	), wxSize( l_iBtnCx, l_iBtnCy ), wxBORDER_SIMPLE );
	_listFolder();
}



FolderList :: ~FolderList()
{
}



void FolderList :: RemoveFile( const wxString & p_file )
{
	FileDelete( ( m_folder + wxT( "/" ) + p_file ).char_str().data() );
}



void FolderList :: AddFile( const wxString & p_file )
{
	unsigned int l_index = min( p_file.find_last_of( wxT( "/" ) ), p_file.find_last_of( wxT( "\\" ) ) );
	wxString l_fileName = p_file.substr( l_index + 1 );

//	std::cout << "FolderList :: AddFile - " << m_folder + "/" + l_fileName << "\n";
	if ( FileCopy( p_file.char_str().data(), ( m_folder + wxT( "/" ) + l_fileName ).char_str().data() ) )
	{
		m_filesList->InsertItems( 1, & l_fileName, m_filesList->GetCount() );
	}
	else
	{
		std::cout << "FolderList :: AddFile - Can't copy from : " << p_file.char_str().data() << " to : " << ( m_folder + wxT( "/" ) + l_fileName ).char_str().data() << "\n";
	}
}



void FolderList :: _listFolder()
{
	if ( m_filesList != NULL )
	{
		m_filesList->Clear();
		RemoveChild( m_filesList );
		delete m_filesList;
		m_filesList = NULL;
	}

	if ( ! exists( m_folder.char_str().data() ) )
	{
		std::cout << "FolderList :: _listFolder - Folder " << m_folder << " doesn't exist\n";
	}
	else
	{
		directory_iterator end_itr;
		long l_index = 0;
		wxString l_file;
		wxArrayString l_files;
		unsigned int l_lastSepIndex;

		for ( directory_iterator itr( m_folder.char_str().data() ) ; itr != end_itr ; ++itr, l_index++ )
		{
			if ( ! is_directory( itr->status() ) )
			{
				l_file = wxString( itr->path().string().c_str(), wxMBConvLibc() );
				l_lastSepIndex = min( l_file.find_last_of( wxT( "/" ) ), l_file.find_last_of( wxT( "\\" ) ) );
				l_file = l_file.substr( l_lastSepIndex + 1 );
//				std::cout << "FolderList :: _listFolder - Index : " << l_index << " - File : " << l_file << "\n";
				l_files.push_back( l_file );
			}
		}

		wxPoint l_position( 0, 0 );
		wxSize l_size = GetClientSize();
		m_filesList = new wxListBox( this, Liste, l_position, wxSize( l_size.x, l_size.y  - 70 ), l_files, wxLB_EXTENDED | wxLB_NEEDED_SB );
	}
}



void FolderList :: _onQuit( wxCommandEvent & p_event )
{
	Close();
}



void FolderList :: _onAddFile( wxCommandEvent & p_event )
{
	wxFileDialog l_dialog( this, wxT( "Choisissez un ou plusieurs fichiers à ouvrir" ), wxT( "" ), wxT( "" ), wxT( "Tous|*.*" ),
						   wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_MULTIPLE );

	if ( l_dialog.ShowModal() == wxID_OK )
	{
		wxArrayString l_fileNames;
		l_dialog.GetPaths( l_fileNames );

		for ( unsigned int i = 0 ; i < l_fileNames.size() ; i++ )
		{
			AddFile( l_fileNames[i] );
		}
	}
}



void FolderList :: _onDelFile( wxCommandEvent & p_event )
{
	if ( m_filesList == NULL )
	{
		return;
	}

	wxArrayInt l_selection;
	m_filesList->GetSelections( l_selection );

	for ( unsigned int i = 0 ; i < l_selection.size() ; i++ )
	{
		RemoveFile( m_filesList->GetString( l_selection[i] ) );
	}

	for ( int i = l_selection.size() - 1 ; i >= 0 ; i-- )
	{
		m_filesList->Delete( l_selection[i] );
	}
}



void FolderList :: _onKeyUp( wxKeyEvent & p_event )
{
	if ( p_event.GetKeyCode() == WXK_ESCAPE )
	{
		Close();
	}
}


