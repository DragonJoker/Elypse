#include "ChatPlugin.h"
#include "ChatClient.h"
#include "ChatDatabase.h"
#include "ChatGame.h"
#include "ChatWorld.h"
#include "ChatRoom.h"
#include "STLMacros.h"

#ifdef __GNUG__
#include <signal.h>
#endif

using namespace Chat;
using namespace Elypse::ServerPlugin;
using namespace Elypse::Network;

ChatPlugin * g_plugin;

//******************************************************************************

void TreatSignal( int p_signalNo )
{
#ifdef __GNUG__
	std::cout << "TreatSignal\n";

	if ( p_signalNo == SIGTSTP )
	{
		std::cout << "TreatSignal - Stop\n";
		g_plugin->_loadRooms();
		std::cout << "TreatSignal - Rooms loaded\n";
		g_plugin->_loadGames();
		std::cout << "TreatSignal - Games Loaded\n";
	}

#endif
}

//******************************************************************************

ChatTcpService::ChatTcpService( ChatPlugin * p_plugin )
	:	ElypseTcpService( "Chat TCP Service", 48621 ),
		m_plugin( p_plugin )
{
	std::cout << "ChatTCPService(" << m_name << ")" << std::endl;
}

ChatTcpService::~ChatTcpService()
{
	std::cout << "~ChatTCPService(" << m_name << ")" << std::endl;
}

std::shared_ptr< TcpBaseClient > ChatTcpService::DoCreateNewClient()
{
	return std::make_shared< ChatTcpClient >( shared_from_this(), m_plugin );
}

void ChatTcpService::DoDeleteClient( std::shared_ptr< TcpBaseClient > & p_toDelete )
{
//	delete (reinterpret_cast <ChatTcpClient *> ( p_toDelete));
}

void ChatTcpService::DoDestroyClient( std::shared_ptr< TcpBaseClient > & p_toDelete )
{
//	reinterpret_cast <ChatTcpClient *> ( p_toDelete)->Stop();
//	m_service.post( boost::bind( & ChatTcpService::___deleteClient, this, p_toDelete));
//	delete (reinterpret_cast <ChatTcpClient *> ( p_toDelete));
}

//******************************************************************************

ChatUdpService::ChatUdpService()
	:	ElypseUdpService( "Chat UDP Service", 48622 )
{
	std::cout << "ChatUDPService(" << m_name << ")" << std::endl;
}

ChatUdpService::~ChatUdpService()
{
	std::cout << "~ChatUDPService(" << m_name << ")" << std::endl;
}

//******************************************************************************

ChatPlugin::ChatPlugin( String const & p_path )
	:	ElypsePlugin( "Chat Plugin" ),
		m_world( std::make_shared< ChatWorld >( "Base", 256 ) ),
		m_database( std::make_shared< ChatDatabase >( p_path ) )
{
	g_plugin = this;
	std::cout << "Chatplugin(" << m_name << ") - Services creation" << std::endl;
	m_services.push_back( std::make_shared< ChatTcpService >( this ) );
	m_services.push_back( std::make_shared< ChatUdpService >() );

	srand( time( NULL ) );

	if ( ! m_database->Initialize() )
	{
		exit( 3 );
	}

	_loadRooms();
	_loadGames();
	std::cout << "Chatplugin(" << m_name << ") - Services created" << std::endl;
#ifdef __GNUG__
	struct sigaction l_action;
	memset( & l_action, 0, sizeof( l_action ) );
	l_action.sa_handler = TreatSignal;
	sigemptyset( & l_action.sa_mask );
	l_action.sa_flags = 0;
	sigaction( SIGTSTP, & l_action, NULL );
#endif
}

ChatPlugin::~ChatPlugin()
{
	std::cout << "~Chatplugin(" << m_name << ") - Services deletion" << std::endl;

	m_services.clear();

	std::cout << "~Chatplugin(" << m_name << ") - Services deleted" << std::endl;
}

unsigned short ChatPlugin::GetVersionNo()
{
	return sm_versionNo;
}

ElypseServiceArray ChatPlugin::GetServices()
{
	return m_services;
}

void ChatPlugin::AddRoom( std::shared_ptr< ChatRoom > p_room )
{
	m_world->AddRoom( p_room );
}

std::shared_ptr< ChatRoom > ChatPlugin::GetRoom( const String & p_roomName )
{
	return m_world->GetRoom( p_roomName );
}

std::shared_ptr< ChatTcpClient > ChatPlugin::GetClient( const String & p_name )
{
	auto && l_clientIdIt = m_clientIDs.find( p_name );

	if ( l_clientIdIt != m_clientIDs.end() )
	{
		auto && l_clientIt = m_connectedClients.find( l_clientIdIt->second );

		if ( l_clientIt != m_connectedClients.end() )
		{
			return l_clientIt->second.lock();
		}
	}

	return nullptr;
}

const String & ChatPlugin::GetClientName( unsigned int p_id )
{
	ClientIdStrMap::iterator l_it = m_clientIDs.begin();

	while ( l_it != m_clientIDs.end() )
	{
		if ( l_it->second == p_id )
		{
			return l_it->first;
		}

		++l_it;
	}

	return ChatEmptyString;
}

void ChatPlugin::AddClient( std::shared_ptr< ChatTcpClient > p_client )
{
	m_connectedClients.insert( std::make_pair( p_client->GetId(), p_client ) );
	m_clientIDs.insert( std::make_pair( p_client->GetName(), p_client->GetId() ) );
}

void ChatPlugin::RemoveClient( std::shared_ptr< ChatTcpClient > p_client )
{
	auto && l_it1 = m_connectedClients.find( p_client->GetId() );

	if ( l_it1 != m_connectedClients.end() )
	{
		std::cout << "ChatPlugin::RemoveClient - Removing " << p_client->GetName() << " from m_connectedClients\n";
		m_connectedClients.erase( l_it1 );
	}

	auto && l_it2 = m_clientIDs.find( p_client->GetName() );

	if ( l_it2 != m_clientIDs.end() )
	{
		std::cout << "ChatPlugin::RemoveClient - Removing " << p_client->GetName() << " from m_clientIDs\n";
		m_clientIDs.erase( l_it2 );
	}
}

std::shared_ptr< ChatGame > ChatPlugin::AddGame( const String & p_gameName, std::shared_ptr< ChatTcpClient > p_initiator )
{
	if ( !p_initiator )
	{
		return nullptr;
	}

	unsigned int l_gameId = 0;
	unsigned int l_maxPlayers = 0;
	bool l_found = false;
	auto && l_gnit = m_gamesNames.begin();

	while ( l_gnit != m_gamesNames.end() && !l_found )
	{
		if ( l_gnit->second.first == p_gameName )
		{
			l_found = true;
			l_gameId = l_gnit->first;
			l_maxPlayers = l_gnit->second.second;
		}

		++l_gnit;
	}

	if ( !l_found )
	{
		p_initiator->SendGameDontExistMessage();
		return nullptr;
	}

	auto && l_it = m_games.find( p_gameName );

	if ( l_it != m_games.end() )
	{
		auto && l_it2 = l_it->second.find( p_initiator->GetId() );

		if ( l_it2 != l_it->second.end() )
		{
			if ( l_it2->second->AddPlayer( p_initiator ) )
			{
				return l_it2->second;
			}
			else
			{
				p_initiator->SendGameAlreadyCreatedMessage();
				return nullptr;
			}
		}
	}
	else
	{
		ChatGameIDMap l_map;
		m_games.insert( std::make_pair( p_gameName, l_map ) );
		l_it = m_games.find( p_gameName );
	}

	std::shared_ptr< ChatGame > l_game = std::make_shared< ChatGame >( l_gameId, p_initiator, p_gameName, l_maxPlayers, m_database );
	l_it->second.insert( std::make_pair( p_initiator->GetId(), l_game ) );
	p_initiator->SendCreateGameOK();
	return l_game;
}

std::shared_ptr< ChatGame > ChatPlugin::GetGame( const String & p_gameName, int p_initiatorID )
{
	auto && l_it = m_games.find( p_gameName );

	if ( l_it == m_games.end() )
	{
		return nullptr;
	}

	auto && l_it2 = l_it->second.find( p_initiatorID );

	if ( l_it2 == l_it->second.end() )
	{
		return nullptr;
	}

	return l_it2->second;
}

StrUIntIdMap ChatPlugin::GetGamesList( const String & p_gameName )
{
	StrUIntIdMap l_map;
	auto && l_it = m_games.find( p_gameName );

	if ( l_it == m_games.end() )
	{
		return l_map;
	}

	for ( auto && l_it2 : l_it->second )
	{
		if ( !l_it2.second->IsFull() )
		{
			StrUIntPair l_pair( GetClientName( l_it2.first ), l_it2.second->GetPlayersCount() );
			l_map.insert( std::make_pair( l_it2.first, l_pair ) );
		}
	}

	return l_map;
}

unsigned int ChatPlugin::GetGameMaxPLayers( const String & p_gameName )
{
	StrUIntIdMap l_map;
	auto && l_it = m_gamesNames.begin();
	bool l_found = false;

	while ( l_it != m_gamesNames.end() )
	{
		if ( l_it->second.first == p_gameName )
		{
			l_found = true;
			return l_it->second.second;
		}

		++l_it;
	}

	return 1;
}

void ChatPlugin::RemoveGame( const String & p_gameName, int p_initiatorID )
{
	auto && l_it = m_games.find( p_gameName );

	if ( l_it == m_games.end() )
	{
		std::cout << "Game named " << p_gameName << " not found\n";
		return;
	}

	auto && l_it2 = l_it->second.find( p_initiatorID );

	if ( l_it2 == l_it->second.end() )
	{
		std::cout << "Game created by " << p_initiatorID << " not found\n";
		return;
	}

	l_it2->second.reset();
	l_it->second.erase( l_it2 );
}

void ChatPlugin::_loadRooms()
{
	if ( !m_database->LoadRooms( *m_world ) )
	{
		exit( 1 );
	}
}

void ChatPlugin::_loadGames()
{
	if ( !m_database->LoadGames( m_gamesNames ) )
	{
		exit( 2 );
	}
}

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef __GNUG__
__declspec( dllexport )
#endif
ElypsePlugin * PluginFactory( const char * p_path )
{
	return new ChatPlugin( p_path );
}

#ifndef __GNUG__
__declspec( dllexport )
#endif
void PluginDestroyer( ElypsePlugin *& p_pluginToDelete )
{
	delete reinterpret_cast <ChatPlugin *>( p_pluginToDelete );
	p_pluginToDelete = NULL;
}

#ifdef __cplusplus
}
#endif
//******************************************************************************
