/*
See LICENSE file in root folder
*/
#include "TrollEditorPch.h"

#include "MediaObject.h"

using namespace Troll::GUI;

namespace Troll
{
	namespace ProjectComponents
	{
		namespace Media
		{
			MediaObject::MediaObject( wxString const & p_name, wxString const & p_fileName, MediaObjectType p_type )
				: Object( p_name, p_fileName, ttMediaObject )
				, m_mediaObjectType( p_type )
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
				LogMessage( l_msg );
			}

			MediaObject::~MediaObject()
			{
			}
		}
	}
}
