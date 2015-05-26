/*
#ifndef WINVER				// Allow use of features specific to Windows XP or later.
#define WINVER 0x0501		// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 6.0 or later.
#define _WIN32_IE 0x0600	// Change this to the appropriate value to target other versions of IE.
#endif
*/
#include "PrecompiledHeader.h"
#include "EMusePlugin_wxWidget.h"

EMusePlugin_wxWidget::EMusePlugin_wxWidget()
	:	m_handle( 0 ),
		m_protectedMode( false )
{
}

EMusePlugin_wxWidget::~EMusePlugin_wxWidget()
{
}

String EMusePlugin_wxWidget::GetStringFromUrl( const String & p_url , bool p_usePostMethod )
{
	return String();
}

bool EMusePlugin_wxWidget::OpenURL( const String & p_url, bool p_newWindow , bool p_usePostMethod )
{
	return true;
}

void EMusePlugin_wxWidget::MajorError( const String & p_errorText , const String & p_title )
{
	wxMessageBox( wxString( p_errorText.c_str(), wxMBConvLibc() ), wxString( p_title.c_str(), wxMBConvLibc() ), wxICON_EXCLAMATION, m_handle );
}

void EMusePlugin_wxWidget::Quit( )
{
//	exit( 0);
}

void EMusePlugin_wxWidget::InitGDI()
{
}

void EMusePlugin_wxWidget::CloseGDI()
{
}

void EMusePlugin_wxWidget::PostChangeGraphicalStatus()
{
}

void EMusePlugin_wxWidget::Paint()
{
}

void EMusePlugin_wxWidget :: LockGui()
{
	/*
		if ( ! wxIsMainThread())
		{
			wxMutexGuiEnter();
		}
	*/
}

void EMusePlugin_wxWidget :: UnlockGui()
{
	/*
		// 3 cookies for the first one who can tell me what this changes. except it does change a lot. Prevents a bug.
		if ( ! wxIsMainThread())
		{
			wxMutexGuiLeave();
		}
	*/
}

void EMusePlugin_wxWidget :: ChangeCursorTo( CursorType p_cursorType )
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
