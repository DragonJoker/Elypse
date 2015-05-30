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

TcpBaseClient * ChatTcpService::_createNewClient()
{
	return reinterpret_cast <TcpBaseClient *>( new ChatTcpClient( this, m_plugin ) );
}

void ChatTcpService::___deleteClient( TcpBaseClient * p_toDelete )
{
//	delete (reinterpret_cast <ChatTcpClient *> ( p_toDelete));
}

void ChatTcpService::_deleteClient( TcpBaseClient * p_toDelete )
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
		m_world( new ChatWorld( "Base", 256 ) ),
		m_database( new ChatDatabase( p_path ) )
{
	g_plugin = this;
	std::cout << "Chatplugin(" << m_name << ") - Services creation" << std::endl;
	m_services.push_back( new ChatTcpService( this ) );
	m_services.push_back( new ChatUdpService() );

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

	General::Utils::vector::deleteAll <ElypseService> ( m_services );

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

void ChatPlugin::AddRoom( ChatRoom * p_room )
{
	m_world->AddRoom( p_room );
}

ChatRoom * ChatPlugin::GetRoom( const String & p_roomName )
{
	return m_world->GetRoom( p_roomName );
}

ChatTcpClient * ChatPlugin::GetClient( const String & p_name )
{
	ClientIdStrMap::iterator l_clientIdIt = m_clientIDs.find( p_name );

	if ( l_clientIdIt != m_clientIDs.end() )
	{
		ClientIdChatCLientMap::iterator l_clientIt = m_connectedClients.find( l_clientIdIt->second );

		if ( l_clientIt != m_connectedClients.end() )
		{
			return l_clientIt->second;
		}
	}

	return NULL;
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

void ChatPlugin::AddClient( ChatTcpClient * p_client )
{
	m_connectedClients.insert( ClientIdChatCLientMap::value_type( p_client->GetId(), p_client ) );
	m_clientIDs.insert( ClientIdStrMap::value_type( p_client->GetName(), p_client->GetId() ) );
}

void ChatPlugin::RemoveClient( ChatTcpClient * p_client )
{
	ClientIdChatCLientMap::iterator l_it1 = m_connectedClients.find( p_client->GetId() );

	if ( l_it1 != m_connectedClients.end() )
	{
		std::cout << "ChatPlugin::RemoveClient - Removing " << p_client->GetName() << " from m_connectedClients\n";
		m_connectedClients.erase( l_it1 );
	}

	ClientIdStrMap::iterator l_it2 = m_clientIDs.find( p_client->GetName() );

	if ( l_it2 != m_clientIDs.end() )
	{
		std::cout << "ChatPlugin::RemoveClient - Removing " << p_client->GetName() << " from m_clientIDs\n";
		m_clientIDs.erase( l_it2 );
	}
}

ChatGame * ChatPlugin::AddGame( const String & p_gameName,
								ChatTcpClient * p_initiator )
{
	if ( p_initiator == NULL )
	{
		return NULL;
	}

	unsigned int l_gameId = 0;
	unsigned int l_maxPlayers = 0;
	bool l_found = false;
	StrUIntIdMap::iterator l_gnit = m_gamesNames.begin();

	while ( l_gnit != m_gamesNames.end() && ! l_found )
	{
		if ( l_gnit->second.first == p_gameName )
		{
			l_found = true;
			l_gameId = l_gnit->first;
			l_maxPlayers = l_gnit->second.second;
		}

		++l_gnit;
	}

	if ( ! l_found )
	{
		p_initiator->SendGameDontExistMessage();
		return NULL;
	}

	ChatGameStrMap::iterator l_it = m_games.find( p_gameName );

	if ( l_it != m_games.end() )
	{
		ChatGameIDMap::iterator l_it2 = l_it->second.find( p_initiator->GetId() );

		if ( l_it2 != l_it->second.end() )
		{
			if ( l_it2->second->AddPlayer( p_initiator ) )
			{
				return l_it2->second;
			}
			else
			{
				p_initiator->SendGameAlreadyCreatedMessage();
				return NULL;
			}
		}
	}
	else
	{
		ChatGameIDMap l_map;
		m_games.insert( ChatGameStrMap::value_type( p_gameName, l_map ) );
		l_it = m_games.find( p_gameName );
	}

	ChatGame * l_game = new ChatGame( l_gameId, p_initiator, p_gameName, l_maxPlayers, m_database );
	l_it->second.insert( ChatGameIDMap::value_type( p_initiator->GetId(), l_game ) );
	p_initiator->SendCreateGameOK();
	return l_game;
}

ChatGame * ChatPlugin::GetGame( const String & p_gameName, int p_initiatorID )
{
	ChatGameStrMap::iterator l_it = m_games.find( p_gameName );

	if ( l_it == m_games.end() )
	{
		return NULL;
	}

	ChatGameIDMap::iterator l_it2 = l_it->second.find( p_initiatorID );

	if ( l_it2 == l_it->second.end() )
	{
		return NULL;
	}

	return l_it2->second;
}

StrUIntIdMap ChatPlugin::GetGamesList( const String & p_gameName )
{
	StrUIntIdMap l_map;
	ChatGameStrMap::iterator l_it = m_games.find( p_gameName );

	if ( l_it == m_games.end() )
	{
		return l_map;
	}

	for ( ChatGameIDMap::iterator l_it2 = l_it->second.begin() ; l_it2 != l_it->second.end() ; ++l_it2 )
	{
		if ( ! l_it2->second->IsFull() )
		{
			StrUIntPair l_pair( GetClientName( l_it2->first ), l_it2->second->GetNbPlayers() );
			l_map.insert( StrUIntIdMap::value_type( l_it2->first, l_pair ) );
		}
	}

	return l_map;
}

unsigned int ChatPlugin::GetGameMaxPLayers( const String & p_gameName )
{
	StrUIntIdMap l_map;
	StrUIntIdMap::iterator l_it = m_gamesNames.begin();
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
	ChatGameStrMap::iterator l_it = m_games.find( p_gameName );

	if ( l_it == m_games.end() )
	{
		std::cout << "Game named " << p_gameName << " not found\n";
		return;
	}

	ChatGameIDMap::iterator l_it2 = l_it->second.find( p_initiatorID );

	if ( l_it2 == l_it->second.end() )
	{
		std::cout << "Game created by " << p_initiatorID << " not found\n";
		return;
	}

	delete l_it2->second;
	l_it->second.erase( l_it2 );
}

void ChatPlugin::_loadRooms()
{
	if ( ! m_database->LoadRooms( m_world ) )
	{
		exit( 1 );
	}
}

void ChatPlugin::_loadGames()
{
	if ( ! m_database->LoadGames( m_gamesNames ) )
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
