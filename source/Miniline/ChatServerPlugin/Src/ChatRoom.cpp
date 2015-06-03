#include "ChatRoom.h"
#include "ChatClient.h"
#include "Buffer.h"

using namespace Chat;
using namespace General::Templates;

bool ChatRoom::AddClient( std::shared_ptr< ChatTcpClient > p_client )
{
	bool l_return = false;

	if ( m_clients.find( p_client->GetName() ) == m_clients.end() )
	{
		m_clients.insert( std::make_pair( p_client->GetName(), p_client ) );
		p_client->SetRoom( shared_from_this() );
		l_return = true;
	}

	return l_return;
}

bool ChatRoom::RemoveClient( std::shared_ptr< ChatTcpClient > p_client )
{
	bool l_return = false;
	auto && l_it = m_clients.find( p_client->GetName() );

	if ( l_it != m_clients.end() )
	{
		p_client->RemoveRoom();
		m_clients.erase( l_it );
		l_return = true;
	}

	return l_return;
}

void ChatRoom::ForwardMessage( const String & p_message, const String & p_name )
{
	for ( auto && l_client : m_clients )
	{
		if ( l_client.second && l_client.second->IsToDelete() && l_client.second->GetName() != p_name )
		{
			//std::cout << "forwarding message to ";
			//std::cout << l_it->second->GetName() << "\n";
			l_client.second->AsyncSend( p_message );
		}
	}
}

void ChatRoom::ForwardMessage( const String & p_message, const String & p_name, ClientIdStrMap const & p_ignored )
{
	for ( auto && l_client : m_clients )
	{
		if ( l_client.second && l_client.second->IsToDelete() && l_client.second->GetName() != p_name && p_ignored.find( l_client.second->GetName() ) == p_ignored.end() )
		{
			l_client.second->AsyncSend( p_message );
		}
	}
}

void ChatRoom::Save( General::Templates::WriteBuffer & p_buffer )const
{
	//std::cout << m_name << " - " << m_sceneName << " - " << m_clients.size() << "\n";
	p_buffer << int( m_name.size() );
	p_buffer.writeArray< char >( m_name.c_str(), m_name.size() );
	p_buffer << int( m_sceneName.size() );
	p_buffer.writeArray< char >( m_sceneName.c_str(), m_sceneName.size() );
	p_buffer << int( m_clients.size() );
}
