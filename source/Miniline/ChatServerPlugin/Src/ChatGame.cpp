/* See Licence.txt in root folder */
#include "ChatGame.h"
#include "ChatClient.h"
#include "ChatDatabase.h"

using namespace Chat;

ChatGame::ChatGame( uint32_t p_id, ChatTcpClient * p_initiator,
					String const & p_gameName, uint32_t p_maxPlayers,
					std::shared_ptr< ChatDatabase > p_database )
	: m_initiator( p_initiator )
	, m_initiatorId( p_initiator->GetId() )
	, m_gameName( p_gameName )
	, m_maxPlayers( p_maxPlayers )
	, m_started( false )
	, m_id( p_id )
	, m_database( p_database )
{
	m_players.insert( std::make_pair( p_initiator->GetId(), p_initiator ) );
	m_playersPlaces.push_back( p_initiator->GetName() );
}

ChatGame::~ChatGame()
{
	m_initiator = NULL;
	EndGame();
	m_players.clear();
}

bool ChatGame::AddPlayer( ChatTcpClient * p_client )
{
	bool l_return = false;

	if ( m_players.find( p_client->GetId() ) == m_players.end() || IsFull() )
	{
		std::cout << "ChatGame::AddPlayer - adding id :" << p_client->GetId() << std::endl;
		m_players.insert( std::make_pair( p_client->GetId(), p_client ) );
		m_playersPlaces.push_back( p_client->GetName() );
		DoGetDatabase()->AddPlayerToGame( m_id );
		l_return = true;
	}

	return l_return;
}

bool ChatGame::RemovePlayer( ChatTcpClient * p_client )
{
	bool l_return = false;
	auto && l_it = m_players.find( p_client->GetId() );

	if ( l_it != m_players.end() )
	{
		m_started = p_client != m_initiator;
		m_players.erase( l_it );
		DoGetDatabase()->RemovePlayerFromGame( m_id );
		l_return = true;
	}

	return l_return;
}

void ChatGame::StartGame()
{
	m_started = true;

	for ( auto && l_it : m_players )
	{
		ChatTcpClient * l_player = l_it.second;

		if ( l_player )
		{
			l_player->SendStartGameMessage();
		}
	}
}

void ChatGame::EndGame()
{
	m_started = false;

	for ( auto && l_it : m_players )
	{
		ChatTcpClient * l_player = l_it.second;

		if ( l_player )
		{
			l_player->SendEndGameMessage();
		}
	}
}

uint32_t ChatGame::GetPlayerPlace( String const & p_name )
{
	auto && l_it = std::find_if( m_playersPlaces.begin(), m_playersPlaces.end(), [&p_name]( String const & l_name )
	{
		return l_name == p_name;
	} );

	uint32_t l_return = 0xFFFFFFFF;

	if ( l_it != m_playersPlaces.end() )
	{
		l_return = uint32_t( std::distance( m_playersPlaces.begin(), l_it ) );
	}

	return l_return;
}

void ChatGame::ForwardMessage( String const & p_message, uint32_t p_id )
{
	for ( auto && l_it : m_players )
	{
		ChatTcpClient * l_player = l_it.second;

		if ( l_player && !l_player->IsToDelete() && l_player->GetId() != p_id )
		{
			std::clog << "forwarding message to " << l_player->GetName() << std::endl;
			l_player->AsyncSend( p_message );
		}
	}
}
