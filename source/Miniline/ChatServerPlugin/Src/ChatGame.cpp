#include "ChatGame.h"
#include "ChatClient.h"
#include "ChatDatabase.h"

using namespace Chat;

ChatGame::ChatGame( unsigned int p_id, ChatTcpClient * p_initiator,
					const String & p_gameName, unsigned int p_maxPlayers,
					ChatDatabase * p_database )
	:	m_initiator( p_initiator ),
		m_initiatorId( m_initiator->GetId() ),
		m_gameName( p_gameName ),
		m_maxPlayers( p_maxPlayers ),
		m_started( false ),
		m_id( p_id ),
		m_database( p_database )
{
	m_players.insert( ClientIdChatCLientMap::value_type( m_initiator->GetId(), m_initiator ) );
	m_playersPlaces.push_back( m_initiator->GetName() );
}

ChatGame::~ChatGame()
{
	m_initiator = NULL;
	EndGame();
	m_players.clear();
}

bool ChatGame::AddPlayer( ChatTcpClient * p_client )
{
	if ( m_players.find( p_client->GetId() ) != m_players.end() || IsFull() )
	{
		return false;
	}

	std::cout << "ChatGame::AddPlayer - adding id :" << p_client->GetId() << "\n";
	m_players.insert( ClientIdChatCLientMap::value_type( p_client->GetId(), p_client ) );
	m_playersPlaces.push_back( p_client->GetName() );
	m_database->AddPlayerToGame( m_id );
	return true;
}

bool ChatGame::RemovePlayer( ChatTcpClient * p_client )
{
	ClientIdChatCLientMap::iterator l_it = m_players.find( p_client->GetId() );

	if ( l_it == m_players.end() )
	{
		return false;
	}

	if ( p_client == m_initiator )
	{
		m_started = false;
	}

	m_players.erase( l_it );
	m_database->RemovePlayerFromGame( m_id );
	return true;
}

void ChatGame::StartGame()
{
	m_started = true;

	for ( ClientIdChatCLientMap::iterator l_it = m_players.begin() ; l_it != m_players.end() ; ++l_it )
	{
		l_it->second->SendStartGameMessage();
	}
}

void ChatGame::EndGame()
{
	m_started = false;

	for ( ClientIdChatCLientMap::iterator l_it = m_players.begin() ; l_it != m_players.end() ; ++l_it )
	{
		l_it->second->SendEndGameMessage();
	}
}

int ChatGame::GetPlayerPlace( const String & p_name )
{
	int i = 0;

	while ( i < static_cast <int>( m_playersPlaces.size() ) )
	{
		if ( m_playersPlaces[i] == p_name )
		{
			return i;
		}

		i++;
	}

	return -1;
}
