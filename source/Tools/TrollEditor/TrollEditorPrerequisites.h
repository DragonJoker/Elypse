/*
See LICENSE file in root folder
*/
#ifndef ___TROLL_EDITOR_PREREQUISITES_H___
#define ___TROLL_EDITOR_PREREQUISITES_H___

#include <GuiCommonPrerequisites.h>

#include <vector>
#include <map>

class wxWindow;

namespace Troll
{
	class Application;

	using GuiCommon::make_string;
	using GuiCommon::make_wxString;

	static const wxString SCENE_FILES = _( "Scene files" );
	static const wxString LOAD_SCRIPT_FILES = _( "Load script files" );
	static const wxString UNLOAD_SCRIPT_FILES = _( "Unload script files" );
	static const wxString GRAPHIC_DATA_FOLDERS = _( "Graphic (2D and 3D) data folders" );
	static const wxString OTHER_DATA_FOLDERS = _( "Non graphic data folders" );
	static const wxString TROLL_EDITOR_ERROR = _( "TrollEditor Error" );
	static const wxString TE_OK = _( "OK" );
	static const wxString TE_CANCEL = _( "Cancel" );

	void LogDebug( wxString const & p_message );
	void LogMessage( wxString const & p_message );
	void LogError( wxString const & p_message );

	inline void LogDebug( String const & p_message )
	{
		LogDebug( make_wxString( p_message ) );
	}

	inline void LogMessage( String const & p_message )
	{
		LogMessage( make_wxString( p_message ) );
	}

	inline void LogError( String const & p_message )
	{
		LogError( make_wxString( p_message ) );
	}
}

#endif
