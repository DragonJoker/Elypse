#include "PrecompiledHeader.h"

#include "Project/Media/MediaObject.h"
#include "GUI/MainFrame.h"

using namespace Troll::Media;
using namespace Troll::GUI;



MediaObject :: MediaObject( const wxString & p_name, const wxString & p_fileName, MediaObjectType p_type )
	:	TrollObject( p_name, p_fileName, ttMediaObject ),
		m_mediaObjectType( p_type )
{
	wxString l_msg = wxT( "Creating media object : " );

	if ( p_type == ttSound )
	{
		l_msg += wxT( "Sound - " );
	}
	else if ( p_type == ttSoundObject )
	{
		l_msg += wxT( "Sound object - " );
	}
	else if ( p_type == ttSoundPlaylist )
	{
		l_msg += wxT( "Sound playlist - " );
	}
	else if ( p_type == ttVideo )
	{
		l_msg += wxT( "Video - " );
	}
	else if ( p_type == ttVideoObject )
	{
		l_msg += wxT( "Video object - " );
	}

	l_msg += wxT( "Name : " ) + p_name;
	g_mainFrame->LogDebugMessage( l_msg );
//	std::cout << l_msg << "\n";
}



MediaObject :: ~MediaObject()
{
}


