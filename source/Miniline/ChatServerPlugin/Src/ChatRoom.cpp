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
#include "ChatRoom.h"
#include "ChatClient.h"
#include "Buffer.h"

using namespace Chat;
using namespace General::Templates;

bool ChatRoom::AddClient( ChatTcpClient * p_client )
{
	bool l_return = false;

	if ( m_clients.find( p_client->GetId() ) == m_clients.end() )
	{
		m_clients.insert( std::make_pair( p_client->GetId(), p_client ) );
		p_client->SetRoom( shared_from_this() );
		l_return = true;
	}

	return l_return;
}

bool ChatRoom::RemoveClient( ChatTcpClient * p_client )
{
	bool l_return = false;
	auto && l_it = m_clients.find( p_client->GetId() );

	if ( l_it != m_clients.end() )
	{
		p_client->RemoveRoom();
		m_clients.erase( l_it );
		l_return = true;
	}

	return l_return;
}

ChatTcpClient * ChatRoom::GetClient( String const & p_name )const
{
	ChatTcpClient * l_return = NULL;

	auto && l_it = std::find_if( m_clients.begin(), m_clients.end(), [&p_name]( ChatTcpClientIdMap::value_type const & p_client )
	{
		return p_client.second->GetName() == p_name;
	} );

	if ( l_it != m_clients.end() )
	{
		l_return = l_it->second;
	}

	return l_return;
}

void ChatRoom::ForwardMessage( String const & p_message, uint32_t p_id )
{
	for ( auto && l_client : m_clients )
	{
		if ( l_client.second && l_client.second->IsToDelete() && l_client.second->GetId() != p_id )
		{
			std::clog << "forwarding message to " << l_client.second->GetName() << std::endl;
			l_client.second->AsyncSend( p_message );
		}
	}
}

void ChatRoom::ForwardMessage( String const & p_message, uint32_t p_id, ClientStrIdMap const & p_ignored )
{
	for ( auto && l_client : m_clients )
	{
		if ( l_client.second && l_client.second->IsToDelete() && l_client.second->GetId() != p_id && p_ignored.find( l_client.second->GetId() ) == p_ignored.end() )
		{
			l_client.second->AsyncSend( p_message );
		}
	}
}

void ChatRoom::Save( General::Templates::WriteBuffer & p_buffer )const
{
	std::clog << m_name << " - " << m_sceneName << " - " << m_clients.size() << std::endl;
	p_buffer << int( m_name.size() );
	p_buffer.writeArray< char >( m_name.c_str(), m_name.size() );
	p_buffer << int( m_sceneName.size() );
	p_buffer.writeArray< char >( m_sceneName.c_str(), m_sceneName.size() );
	p_buffer << int( m_clients.size() );
}
