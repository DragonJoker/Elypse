
#ifndef ___Module_GUI___
#define ___Module_GUI___

#include <wx/treebase.h>
#include <Memory.h>

class wxTextCtrl;
class wxTextEvent;
class wxComboBox;
class wxButton;
class wxCommandEvent;
class wxListCtrl;
class wxListEvent;
class wxPoint;
class wxString;
class wxWindow;
class wxPanel;
class wxColour;
class wxCheckBox;

namespace Troll
{
	class TrollObject;

	namespace GUI
	{
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

		class TimeLinePanel;
		class SequencePanel;
		class PonctualEventPanel;
		class ContinuousEventPanel;
		class ArrowPanel;
		class LinePanel;
		class TimePanel;
		class TimeLineContainer;

		class ObjectProperties;
		class ProjectProperties;
		class SceneProperties;
		class OverlayProperties;
		class CameraProperties;
		class EntityProperties;
		class LightProperties;
		class SceneNodeProperties;
		class SoundObjectProperties;
		class SoundProperties;
		class VideoObjectProperties;
		class VideoProperties;
		class ContinuousEventProperties;
		class KeyFrameProperties;
		class PonctualEventProperties;
		class SequenceProperties;

		class SceneDependenciesFrame;

		wxString make_wxString( String const & p_value );
		float GetReal( const wxString & p_value, float & p_res );
		int GetInt( const wxString & p_value, int & p_res );
		float GetReal( const String & p_value, float & p_res );
		int GetInt( const String & p_value, int & p_res );

		wxPanel * BuildPanel( wxWindow * p_parent, const wxPoint & p_position, const wxSize & p_size,
							  int p_id, const wxColour & p_colour, long p_style );
		wxTextCtrl * BuildTextCtrl( wxWindow * p_parent, const wxPoint & p_position, int p_id,
									const wxString & p_caption, const wxString & p_value = wxEmptyString );
		wxTextCtrl * BuildTextCtrl( wxWindow * p_parent, const wxPoint & p_position, int p_id,
									const wxString & p_caption, float p_value );
		wxTextCtrl * BuildTextCtrl( wxWindow * p_parent, const wxPoint & p_position, int p_id,
									const wxString & p_caption, int p_value );
		wxComboBox * BuildComboBox( wxWindow * p_parent, const wxPoint & p_position, int p_id,
									const wxString & p_caption, const wxArrayString & p_values,
									const wxString & p_value );
		wxListCtrl * BuildListBox( wxWindow * p_parent, const wxPoint & p_position, int p_id,
								   const wxArrayString & p_captions,
								   const wxArrayString & p_values );
		wxButton * BuildButton( wxWindow * p_parent, const wxPoint & p_position, int p_id,
								const wxString & p_caption, const wxString & p_value );

		wxTextCtrl * BuildTextCtrl( wxWindow * p_parent, const wxPoint & p_position, int p_id,
									const String & p_caption, const String & p_value = "" );
		wxTextCtrl * BuildTextCtrl( wxWindow * p_parent, const wxPoint & p_position, int p_id,
									const String & p_caption, float p_value );
		wxTextCtrl * BuildTextCtrl( wxWindow * p_parent, const wxPoint & p_position, int p_id,
									const String & p_caption, int p_value );
		wxButton * BuildButton( wxWindow * p_parent, const wxPoint & p_position, int p_id,
								const String & p_caption, const String & p_value );

		wxArrayString Split( const wxString & p_toSplit, const wxString & p_delims, unsigned int p_maxSplits = 10 );

		struct SceneItem;
		struct RecursiveTreeItemObject;
		typedef std::map< wxTreeItemIdValue, RecursiveTreeItemObject * >	RecursiveTreeItemObjectPtrMap;

		struct TreeItemObject
		{
			wxTreeItemId ItemID;
			TrollObject * Object;
		};

		struct RecursiveTreeItemObject
		{
			TreeItemObject ItemObject;
			RecursiveTreeItemObjectPtrMap Childs;

			RecursiveTreeItemObject * RecursiveIsIn( const wxString & p_name );
			RecursiveTreeItemObject * RecursiveIsIn( wxTreeItemIdValue p_index );

			~RecursiveTreeItemObject();
		};

		typedef std::map <wxTreeItemIdValue,	TreeItemObject>			TreeItemObjectMap;
		typedef std::map <wxString,				SceneItem>				TreeItemMap;
		typedef std::map <wxString,				wxTreeItemId>			TreeItemIdMap;
		typedef std::map <wxString,				wxTreeItemId>			MapTypeFile;
	}
}

#endif

