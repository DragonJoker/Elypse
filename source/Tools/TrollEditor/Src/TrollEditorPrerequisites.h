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
