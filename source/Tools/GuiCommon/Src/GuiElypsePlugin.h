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
#ifndef ___EMUSE_PLUGIN_WX_H___
#define ___EMUSE_PLUGIN_WX_H___

#include "GuiCommonPrerequisites.h"

#include <ElypsePlugin.h>

#include <wx/defs.h>
#include <wx/dynlib.h>

namespace GuiCommon
{
	class wxElypsePlugin
		: public Elypse::Main::ElypsePlugin
	{
	public:
		wxElypsePlugin();
		virtual ~wxElypsePlugin();

		virtual String GetStringFromUrl( String const & p_url , bool p_usePostMethod );
		virtual bool OpenURL( String const & p_url, bool p_newWindow, bool p_usePostMethod );
		virtual void MajorError( String const & p_errorText, String const & p_title );
		virtual void Quit();
		virtual void PostChangeGraphicalStatus();
		virtual void LockGui();
		virtual void UnlockGui();
		virtual void ChangeCursorTo( CursorType p_cursorType );

		virtual bool ExecuteJavascript( String const & p_functionName )
		{
			return true;
		};

		inline void SetHandle( wxWindow * p_handle )
		{
			m_handle = p_handle;
		}
		inline void SetProgramFilesDir( String const & p_fullPath )
		{
			m_programFiles = p_fullPath;
		}
		inline void UseProtectedMode()
		{
			m_protectedMode = true;
		}
		inline void SetBaseUrl( String const & p_baseUrl )
		{
			m_baseUrl = p_baseUrl;
		}
		inline void SetDllInstance( std::unique_ptr< wxDynamicLibrary > && p_pLibrary )
		{
			m_pLibrary = std::move( p_pLibrary );
		}

	private:
		wxWindow * m_handle;
		String m_programFiles;
		String m_baseUrl;
		bool m_protectedMode;
		std::unique_ptr< wxDynamicLibrary > m_pLibrary;
	};
}

#endif
