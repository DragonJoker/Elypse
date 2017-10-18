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
