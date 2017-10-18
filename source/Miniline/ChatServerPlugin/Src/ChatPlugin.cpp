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
#include "ChatPlugin.h"
#include "ChatClient.h"
#include "ChatDatabase.h"
#include "ChatGame.h"
#include "ChatWorld.h"
#include "ChatRoom.h"
#include "STLMacros.h"

#include <Database/DatabaseLogger.h>
#include <Database/DatabaseStringUtils.h>

#ifdef __GNUG__
#include <signal.h>
#endif

using namespace Chat;
using namespace Elypse::Server;
using namespace General::Network;

ChatPlugin * g_plugin;

//******************************************************************************

void TreatSignal( int p_signalNo )
{
#ifdef __GNUG__
	std::cout << "TreatSignal" << std::endl;

	if ( p_signalNo == SIGTSTP )
	{
		std::cout << "TreatSignal - Stop" << std::endl;
		g_plugin->_loadRooms();
		std::cout << "TreatSignal - Rooms loaded" << std::endl;
		g_plugin->_loadGames();
		std::cout << "TreatSignal - Games Loaded" << std::endl;
	}

#endif
}

//******************************************************************************

ChatTcpService::ChatTcpService( ChatPlugin * p_plugin )
	: ElypseTcpService( "Chat TCP Service", 48621 )
	, m_plugin( p_plugin )
{
	std::clog << "ChatTCPService(" << m_name << ")" << std::endl;
}

ChatTcpService::~ChatTcpService()
{
	std::clog << "~ChatTCPService(" << m_name << ")" << std::endl;
	DoDeleteClients();
}

TcpBaseClient * ChatTcpService::DoCreateClient()
{
	return new ChatTcpClient( shared_from_this(), m_plugin );
}

void ChatTcpService::DoDeleteClient( TcpBaseClient * p_toDelete )
{
//	delete (reinterpret_cast <ChatTcpClient *> ( p_toDelete));
}

void ChatTcpService::DoDestroyClient( TcpBaseClient * p_toDelete )
{
//	reinterpret_cast <ChatTcpClient *> ( p_toDelete)->Stop();
//	m_service.post( boost::bind( & ChatTcpService::___deleteClient, this, p_toDelete));
//	delete (reinterpret_cast <ChatTcpClient *> ( p_toDelete));
}

//******************************************************************************

ChatUdpService::ChatUdpService()
	:	ElypseUdpService( "Chat UDP Service", 48622 )
{
	std::clog << "ChatUDPService(" << m_name << ")" << std::endl;
}

ChatUdpService::~ChatUdpService()
{
	std::clog << "~ChatUDPService(" << m_name << ")" << std::endl;
}

//******************************************************************************

ChatPlugin::ChatPlugin( String const & p_path )
	: ElypsePlugin( "Chat Plugin" )
	, m_world( std::make_shared< ChatWorld >( "Base", 256 ) )
	, m_database( std::make_shared< ChatDatabase >( p_path ) )
{
	g_plugin = this;
	std::clog << "Chatplugin(" << m_name << ") - Services creation" << std::endl;
	m_services.push_back( std::make_shared< ChatTcpService >( this ) );
	m_services.push_back( std::make_shared< ChatUdpService >() );

	if ( !m_database->Initialize() )
	{
		throw std::runtime_error( "Couldn't initialise database" );
	}

	_loadRooms();
	_loadGames();
	std::clog << "Chatplugin(" << m_name << ") - Services created" << std::endl;
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
	std::clog << "~Chatplugin(" << m_name << ") - Services deletion" << std::endl;

	m_services.clear();

	std::clog << "~Chatplugin(" << m_name << ") - Services deleted" << std::endl;
}

uint16_t ChatPlugin::GetVersionNo()
{
	return sm_versionNo;
}

ElypseServiceArray const & ChatPlugin::GetServices()const
{
	return m_services;
}

void ChatPlugin::AddRoom( std::shared_ptr< ChatRoom > p_room )
{
	m_world->AddRoom( p_room );
}

std::shared_ptr< ChatRoom > ChatPlugin::GetRoom( String const & p_roomName )
{
	return m_world->GetRoom( p_roomName );
}

ChatTcpClient * ChatPlugin::GetClient( uint32_t p_id )
{
	ChatTcpClient * l_return = NULL;
	auto && l_clientIt = m_connectedClients.find( p_id );

	if ( l_clientIt != m_connectedClients.end() )
	{
		l_return = l_clientIt->second;
	}

	return l_return;
}

uint32_t ChatPlugin::GetClientId( String const & p_name )
{
	auto && l_it = std::find_if( m_clients.begin(), m_clients.end(), [&p_name]( ClientStrIdMap::value_type const & p_client )
	{
		return p_client.second == p_name;
	} );

	uint32_t l_return = 0;

	if ( l_it != m_clients.end() )
	{
		l_return = l_it->first;
	}

	return l_return;
}

String const & ChatPlugin::GetClientName( uint32_t p_id )
{
	const String * l_return = &ChatEmptyString;
	auto && l_it = m_clients.find( p_id );

	if ( l_it != m_clients.end() )
	{
		l_return = &l_it->second;
	}

	return *l_return;
}

void ChatPlugin::AddClient( ChatTcpClient * p_client )
{
	m_connectedClients.insert( std::make_pair( p_client->GetId(), p_client ) );
	m_clients.insert( std::make_pair( p_client->GetId(), p_client->GetName() ) );
}

void ChatPlugin::RemoveClient( ChatTcpClient * p_client )
{
	auto && l_it1 = m_connectedClients.find( p_client->GetId() );

	if ( l_it1 != m_connectedClients.end() )
	{
		std::clog << "ChatPlugin::RemoveClient - Removing " << p_client->GetName() << " from m_connectedClients" << std::endl;
		m_connectedClients.erase( l_it1 );
	}

	auto && l_it2 = m_clients.find( p_client->GetId() );

	if ( l_it2 != m_clients.end() )
	{
		std::clog << "ChatPlugin::RemoveClient - Removing " << p_client->GetName() << " from m_clientIDs" << std::endl;
		m_clients.erase( l_it2 );
	}
}

std::shared_ptr< ChatGame > ChatPlugin::AddGame( String const & p_gameName, ChatTcpClient * p_initiator )
{
	if ( !p_initiator )
	{
		return nullptr;
	}

	uint32_t l_gameId = 0;
	uint32_t l_maxPlayers = 0;
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

std::shared_ptr< ChatGame > ChatPlugin::GetGame( String const & p_gameName, int p_initiatorID )
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

StrUIntIdMap ChatPlugin::GetGamesList( String const & p_gameName )
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

uint32_t ChatPlugin::GetGameMaxPLayers( String const & p_gameName )
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

void ChatPlugin::RemoveGame( String const & p_gameName, int p_initiatorID )
{
	auto && l_it = m_games.find( p_gameName );

	if ( l_it == m_games.end() )
	{
		std::cout << "Game named " << p_gameName << " not found" << std::endl;
		return;
	}

	auto && l_it2 = l_it->second.find( p_initiatorID );

	if ( l_it2 == l_it->second.end() )
	{
		std::cout << "Game created by " << p_initiatorID << " not found" << std::endl;
		return;
	}

	l_it2->second.reset();
	l_it->second.erase( l_it2 );
}

void ChatPlugin::_loadRooms()
{
	if ( !m_database->LoadRooms( *m_world ) )
	{
		throw std::runtime_error( "Couldn't load the rooms" );
	}
}

void ChatPlugin::_loadGames()
{
	if ( !m_database->LoadGames( m_gamesNames ) )
	{
		throw std::runtime_error( "Couldn't load the fames" );
	}
}

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef __GNUG__
__declspec( dllexport )
#endif
ElypsePlugin * PluginFactory( char const * const p_path )
{
	ElypsePlugin * l_return = NULL;
	// Configure logger
#if defined( NDEBUG )
	NAMESPACE_DATABASE::CLogger::Initialise( NAMESPACE_DATABASE::ELogType_INFO );
#else
	NAMESPACE_DATABASE::CLogger::Initialise( NAMESPACE_DATABASE::ELogType_DEBUG );
#endif
	NAMESPACE_DATABASE::CLogger::SetFileName( NAMESPACE_DATABASE::StringUtils::ToString( p_path ) + STR( "ChatPlugin.log" ) );


	try
	{
		l_return = new ChatPlugin( p_path );
	}
	catch ( std::exception & p_exc )
	{
		std::cerr << p_exc.what() << std::endl;
	}
	catch ( ... )
	{
		std::cerr << "Unknown error while creating the plugin" << std::endl;
	}

	return l_return;
}

#ifndef __GNUG__
__declspec( dllexport )
#endif
void PluginDestroyer( ElypsePlugin *& p_pluginToDelete )
{
	delete reinterpret_cast< ChatPlugin * >( p_pluginToDelete );
	p_pluginToDelete = NULL;
	NAMESPACE_DATABASE::CLogger::Cleanup();
}

#ifdef __cplusplus
}
#endif
//******************************************************************************
