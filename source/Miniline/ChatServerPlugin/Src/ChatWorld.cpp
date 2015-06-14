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
#include "ChatWorld.h"
#include "ChatRoom.h"

#include <Buffer.h>

using namespace Chat;;

ChatWorld::~ChatWorld()
{
	m_rooms.clear();
}

void ChatWorld::AddRoom( const String & p_name, const String & p_sceneName )
{
	if ( m_rooms.find( p_name ) == m_rooms.end() )
	{
		m_rooms.insert( std::make_pair( p_name, std::make_shared< ChatRoom >( p_name, p_sceneName ) ) );
	}
}

void ChatWorld::AddRoom( std::shared_ptr< ChatRoom > p_room )
{
	if ( m_rooms.find( p_room->GetName() ) == m_rooms.end() )
	{
		m_rooms.insert( std::make_pair( p_room->GetName(), p_room ) );
	}
}

std::shared_ptr< ChatRoom > ChatWorld::GetRoom( const String & p_name )
{
	std::shared_ptr< ChatRoom > l_return;
	ChatRoomStrMap::iterator l_it = m_rooms.find( p_name );

	if ( l_it != m_rooms.end() )
	{
		l_return = l_it->second;
	}

	return l_return;
}

void ChatWorld::Save( General::Templates::WriteBuffer & p_buffer )const
{
	std::clog << "DoSendRoomsMessage - " << m_rooms.size() << "\n";
	p_buffer << int( m_rooms.size() );

	for ( auto && l_room : m_rooms )
	{
		if ( l_room.second )
		{
			l_room.second->Save( p_buffer );
		}
	}
}
