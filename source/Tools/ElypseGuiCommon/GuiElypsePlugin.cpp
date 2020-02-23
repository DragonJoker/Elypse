/*
See LICENSE file in root folder
*/
#include "GuiCommonPch.h"

#include "GuiElypsePlugin.h"

#include <ElypseLogs.h>
#include <OgreStringConverter.h>

namespace GuiCommon
{
	wxElypsePlugin::wxElypsePlugin()
		: m_handle( 0 )
		, m_protectedMode( false )
	{
	}

	wxElypsePlugin::~wxElypsePlugin()
	{
	}

	String wxElypsePlugin::GetStringFromUrl( String const & p_url , bool p_usePostMethod )
	{
		return String();
	}

	bool wxElypsePlugin::OpenURL( String const & p_url, bool p_newWindow , bool p_usePostMethod )
	{
		return true;
	}

	void wxElypsePlugin::MajorError( String const & p_errorText, String const & p_title )
	{
		wxMessageBox( make_wxString( p_errorText ), make_wxString( p_title ), wxICON_EXCLAMATION, m_handle );
	}

	void wxElypsePlugin::Quit()
	{
	}

	void wxElypsePlugin::PostChangeGraphicalStatus()
	{
	}

	void wxElypsePlugin::LockGui()
	{
		//if ( !wxIsMainThread() )
		//{
		//	wxMutexGuiEnter();
		//}
	}

	void wxElypsePlugin::UnlockGui()
	{
		//if ( !wxIsMainThread() )
		//{
		//	wxMutexGuiLeave();
		//}
	}

	void wxElypsePlugin::ChangeCursorTo( CursorType p_cursorType )
	{
		switch ( p_cursorType )
		{
		case TextCursor:
			m_handle->SetCursor( wxCursor( wxCURSOR_IBEAM ) );
			break;

		case HandCursor:
			m_handle->SetCursor( wxCursor( wxCURSOR_HAND ) );
			break;

		case SizeNSCursor:
			m_handle->SetCursor( wxCursor( wxCURSOR_SIZENS ) );
			break;

		case SizeWECursor:
			m_handle->SetCursor( wxCursor( wxCURSOR_SIZEWE ) );
			break;

		default:
			m_handle->SetCursor( wxCursor( wxCURSOR_ARROW ) );
			break;
		}
	}
}
