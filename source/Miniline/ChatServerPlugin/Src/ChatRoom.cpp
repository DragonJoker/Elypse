#include "ChatRoom.h"
#include "ChatClient.h"
#include "Buffer.h"

using namespace Chat;
using namespace General::Templates;

bool ChatRoom::AddClient( ChatTcpClient * p_client )
{
	if ( m_clients.find( p_client->GetName() ) == m_clients.end() )
	{
		m_clients.insert( ChatTcpClientStrMap::value_type( p_client->GetName(),
						  p_client ) );
		p_client->SetRoom( this );
		return true;
	}

	return false;
}

bool ChatRoom::RemoveClient( ChatTcpClient * p_client )
{
	ChatTcpClientStrMap::iterator l_it = m_clients.find( p_client->GetName() );

	if ( l_it == m_clients.end() )
	{
		return false;
	}

	p_client->RemoveRoom();
	m_clients.erase( l_it );
	return true;
}

void ChatRoom::ForwardMessage( const String & p_message, const String & p_name )
{
	ChatTcpClientStrMap::iterator l_it;

	for ( l_it = m_clients.begin() ; l_it != m_clients.end() ; ++l_it )
	{
		if ( l_it->second != NULL && l_it->second->GetName() != p_name )
		{
			l_it->second->AsyncSend( p_message );
		}
	}
}
