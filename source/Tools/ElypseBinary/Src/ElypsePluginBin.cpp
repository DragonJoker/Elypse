#include "ElypsePluginBin.h"
#include <EMuseLogs.h>

#include <OgreStringConverter.h>

#include <ThreadId.h>

EMusePlugin_Binary :: EMusePlugin_Binary()
{
}

EMusePlugin_Binary :: ~EMusePlugin_Binary()
{
}

void EMusePlugin_Binary :: MajorError( const String & p_errorText, const String & p_title )
{
	wxMessageBox( wxString( p_errorText.c_str(), wxConvLibc ), wxString( p_title.c_str(), wxConvLibc ), wxICON_EXCLAMATION, m_handle );
}

void EMusePlugin_Binary :: Quit()
{
	exit( 0 );
}

void EMusePlugin_Binary :: LockGui()
{
	if ( ! wxIsMainThread() )
	{
//		EMUSE_CONSOLE_MESSAGE_RELEASE( "WX LOCK GUI START on thread : " + StringConverter::toString( GENLIB_THREAD_ID()));
		wxMutexGuiEnter();
//		EMUSE_CONSOLE_MESSAGE_RELEASE( "WX LOCK GUI END on thread : " + StringConverter::toString( GENLIB_THREAD_ID()));
	}
}

void EMusePlugin_Binary :: UnlockGui()
{
	// 3 cookies for the first one who can tell me what this changes. except it does change a lot. Prevents a bug.
	if ( ! wxIsMainThread() )
	{
//		EMUSE_CONSOLE_MESSAGE_RELEASE( "WX UNLOCK GUI START on thread : " + StringConverter::toString( GENLIB_THREAD_ID()));
		wxMutexGuiLeave();
//		EMUSE_CONSOLE_MESSAGE_RELEASE( "WX UNLOCK GUI END on thread : " + StringConverter::toString( GENLIB_THREAD_ID()));
	}
}


void EMusePlugin_Binary :: ChangeCursorTo( CursorType p_cursorType )
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
