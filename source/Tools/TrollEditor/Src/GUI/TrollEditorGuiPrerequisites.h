/*
See LICENSE file in root folder
*/
#ifndef ___Module_GUI___
#define ___Module_GUI___

#include "TrollEditorPrerequisites.h"

#include <wx/treebase.h>
#include <Memory.h>

class wxButton;
class wxCheckBox;
class wxComboBox;
class wxColour;
class wxCommandEvent;
class wxListCtrl;
class wxListEvent;
class wxPanel;
class wxPoint;
class wxStaticText;
class wxString;
class wxTextCtrl;
class wxTextEvent;
class wxWindow;

namespace Troll
{
	namespace ProjectComponents
	{
		class Object;
	}

	namespace GUI
	{
		using GuiCommon::GetInt;
		using GuiCommon::GetReal;

		static const wxString ITEM_DESC_ROOT = wxT( "root" );
		static const wxString ITEM_DESC_SCENE = wxT( "scene" );
		static const wxString ITEM_DESC_FOLDER = wxT( "folder" );
		static const wxString ITEM_DESC_FILE = wxT( "file" );

		static const wxString TE_YES = _( "Yes" );
		static const wxString TE_NO = _( "No" );
		static const wxString TE_REMOVE = _( "Remove" );
		static const wxString TE_ADD_PONCTUAL_EVENT = _( "Add a ponctual event" );
		static const wxString TE_ADD_CONTINUOUS_EVENT = _( "Add a continuous event" );
		static const wxString TE_NEW = _( "New" );
		static const wxString TE_OPEN = _( "Open" );
		static const wxString TE_SAVE = _( "Save" );

		class MainFrame;
		class ProjectFrame;
		class NewOverlayFrame;
		class NewSceneFrame;
		class ElypseContainer;
		class ElypseFrame;
		class ElypsePanel;
		class ElypseCtrl;
		class FolderList;
		class ReplaceDialog;
		class FunctionsTree;
		class FilesTree;
		class ObjectsTree;
		class LogCtrl;
		class TextCtrl;
		class TextLinesCtrl;
		class TextPanel;
		class wxStcTextEditor;
		class SceneDependenciesFrame;
		class FilesTree;

		wxPanel * BuildPanel( wxWindow * p_parent, wxPoint const & p_position, wxSize const & p_size, int p_id, const wxColour & p_colour, long p_style );
		wxTextCtrl * BuildTextCtrl( wxWindow * p_parent, wxPoint const & p_position, int p_id, wxString const & p_caption, wxString const & p_value = wxEmptyString );
		wxTextCtrl * BuildTextCtrl( wxWindow * p_parent, wxPoint const & p_position, int p_id, wxString const & p_caption, float p_value );
		wxTextCtrl * BuildTextCtrl( wxWindow * p_parent, wxPoint const & p_position, int p_id, wxString const & p_caption, int p_value );
		wxComboBox * BuildComboBox( wxWindow * p_parent, wxPoint const & p_position, int p_id, wxString const & p_caption, const wxArrayString & p_values, wxString const & p_value );
		wxListCtrl * BuildListBox( wxWindow * p_parent, wxPoint const & p_position, int p_id, const wxArrayString & p_captions, const wxArrayString & p_values );
		wxButton * BuildButton( wxWindow * p_parent, wxPoint const & p_position, int p_id, wxString const & p_caption, wxString const & p_value );

		wxTextCtrl * BuildTextCtrl( wxWindow * p_parent, wxPoint const & p_position, int p_id, String const & p_caption, String const & p_value = "" );
		wxTextCtrl * BuildTextCtrl( wxWindow * p_parent, wxPoint const & p_position, int p_id, String const & p_caption, float p_value );
		wxTextCtrl * BuildTextCtrl( wxWindow * p_parent, wxPoint const & p_position, int p_id, String const & p_caption, int p_value );
		wxButton * BuildButton( wxWindow * p_parent, wxPoint const & p_position, int p_id, String const & p_caption, String const & p_value );

		wxArrayString Split( wxString const & p_toSplit, wxString const & p_delims, uint32_t p_maxSplits = 10 );

		struct TreeItemObject
		{
			wxTreeItemId ItemID;
			ProjectComponents::Object * Object;
		};

		struct RecursiveTreeItemObject;
		typedef std::map< wxTreeItemIdValue, RecursiveTreeItemObject * > RecursiveTreeItemObjectPtrMap;

		struct RecursiveTreeItemObject
		{
			TreeItemObject ItemObject;
			RecursiveTreeItemObjectPtrMap Childs;

			RecursiveTreeItemObject * RecursiveIsIn( wxString const & p_name );
			RecursiveTreeItemObject * RecursiveIsIn( wxTreeItemIdValue p_index );

			~RecursiveTreeItemObject();
		};

		typedef std::map< wxTreeItemIdValue, TreeItemObject > TreeItemObjectMap;
		typedef std::map< wxString, wxTreeItemId > TreeItemIdMap;
		typedef std::map< wxString, wxTreeItemId > MapTypeFile;
		typedef std::vector< wxString > StrArray;
		typedef std::pair< wxString, wxString > StrPair;
		typedef std::vector< StrPair > StrPairArray;
		typedef std::map< wxString, StrPairArray> StrPairArrayStrMap;
		typedef std::multimap< wxString, wxString> StrStrMultiMap;
		typedef std::map< wxString, wxString> StrStrMap;
		typedef std::map< wxString, wxTreeItemId > ItemStrMap;
	}
}

#endif
