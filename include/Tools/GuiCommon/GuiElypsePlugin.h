/*
See LICENSE file in root folder
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
