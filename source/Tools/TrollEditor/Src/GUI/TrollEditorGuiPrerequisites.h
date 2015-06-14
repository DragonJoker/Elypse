/*
This source file is part of ElypsePlayer (https://sourceforge.net/projects/elypse/)

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
the program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
*/
#ifndef ___Module_GUI___
#define ___Module_GUI___

#include "TrollEditorPrerequisites.h"

#include <wx/treebase.h>
#include <Memory.h>

#define TROLL_GUI_NAMESPACE TROLL_NAMESPACE::GUI
#define BEGIN_TROLL_GUI_NAMESPACE BEGIN_TROLL_NAMESPACE { namespace GUI
#define END_TROLL_GUI_NAMESPACE END_TROLL_NAMESPACE }

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

BEGIN_TROLL_NAMESPACE
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

		wxPanel * BuildPanel( wxWindow * p_parent, const wxPoint & p_position, const wxSize & p_size, int p_id, const wxColour & p_colour, long p_style );
		wxTextCtrl * BuildTextCtrl( wxWindow * p_parent, const wxPoint & p_position, int p_id, const wxString & p_caption, const wxString & p_value = wxEmptyString );
		wxTextCtrl * BuildTextCtrl( wxWindow * p_parent, const wxPoint & p_position, int p_id, const wxString & p_caption, float p_value );
		wxTextCtrl * BuildTextCtrl( wxWindow * p_parent, const wxPoint & p_position, int p_id, const wxString & p_caption, int p_value );
		wxComboBox * BuildComboBox( wxWindow * p_parent, const wxPoint & p_position, int p_id, const wxString & p_caption, const wxArrayString & p_values, const wxString & p_value );
		wxListCtrl * BuildListBox( wxWindow * p_parent, const wxPoint & p_position, int p_id, const wxArrayString & p_captions, const wxArrayString & p_values );
		wxButton * BuildButton( wxWindow * p_parent, const wxPoint & p_position, int p_id, const wxString & p_caption, const wxString & p_value );

		wxTextCtrl * BuildTextCtrl( wxWindow * p_parent, const wxPoint & p_position, int p_id, const String & p_caption, const String & p_value = "" );
		wxTextCtrl * BuildTextCtrl( wxWindow * p_parent, const wxPoint & p_position, int p_id, const String & p_caption, float p_value );
		wxTextCtrl * BuildTextCtrl( wxWindow * p_parent, const wxPoint & p_position, int p_id, const String & p_caption, int p_value );
		wxButton * BuildButton( wxWindow * p_parent, const wxPoint & p_position, int p_id, const String & p_caption, const String & p_value );

		wxArrayString Split( const wxString & p_toSplit, const wxString & p_delims, unsigned int p_maxSplits = 10 );

		struct SceneItem;

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

			RecursiveTreeItemObject * RecursiveIsIn( const wxString & p_name );
			RecursiveTreeItemObject * RecursiveIsIn( wxTreeItemIdValue p_index );

			~RecursiveTreeItemObject();
		};
		
		typedef std::map< wxTreeItemIdValue, TreeItemObject > TreeItemObjectMap;
		typedef std::map< wxString, SceneItem > TreeItemMap;
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
END_TROLL_NAMESPACE

#endif

