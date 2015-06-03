#ifndef ___ChatPlugin___
#define ___ChatPlugin___

#include "Common.h"

#include <ElypsePlugin.h>
#include <ElypseClient.h>
#include <ElypseService.h>

namespace Chat
{
	//**************************************************************************

	class ChatTcpService
		: public Elypse::ServerPlugin::ElypseTcpService
		, public std::enable_shared_from_this< ChatTcpService >
	{
	private:
		ChatPlugin * m_plugin;

	public:
		ChatTcpService( ChatPlugin * p_plugin );
		~ChatTcpService();

	private:
		virtual std::shared_ptr< Elypse::Network::TcpBaseClient > DoCreateNewClient();
		virtual void DoDestroyClient( std::shared_ptr< Elypse::Network::TcpBaseClient > & p_toDelete );
		void DoDeleteClient( std::shared_ptr< Elypse::Network::TcpBaseClient > & p_toDelete );
	};

	//**************************************************************************

	class ChatUdpService
		: public Elypse::ServerPlugin::ElypseUdpService
		, public std::enable_shared_from_this< ChatUdpService >
	{
	public:
		ChatUdpService();
		~ChatUdpService();
	};

	//**************************************************************************

	class ChatPlugin
		: public Elypse::ServerPlugin::ElypsePlugin
	{
	private:
		Elypse::ServerPlugin::ElypseServiceArray m_services;
		std::shared_ptr< ChatWorld > m_world;
		ClientIdChatCLientMap m_connectedClients;
		ClientIdStrMap m_clientIDs;
		std::shared_ptr< ChatDatabase > m_database;
		ChatGameStrMap m_games;
		StrUIntIdMap m_gamesNames;

	public:
		ChatPlugin( String const & p_path );
		~ChatPlugin();

		virtual unsigned short GetVersionNo();
		Elypse::ServerPlugin::ElypseServiceArray GetServices();

		void AddRoom( std::shared_ptr< ChatRoom > p_room );
		std::shared_ptr< ChatRoom > GetRoom( const String & p_roomName );

		std::shared_ptr< ChatTcpClient > GetClient( const String & p_name );
		const String & GetClientName( unsigned int p_id );
		void AddClient( std::shared_ptr< ChatTcpClient > p_client );
		void RemoveClient( std::shared_ptr< ChatTcpClient > p_client );

		std::shared_ptr< ChatGame > AddGame( const String & p_gameName, std::shared_ptr< ChatTcpClient > p_initiator );
		std::shared_ptr< ChatGame > GetGame( const String & p_gameName, int p_initiatorID );
		StrUIntIdMap GetGamesList( const String & p_gameName );
		unsigned int GetGameMaxPLayers( const String & p_gameName );
		void RemoveGame( const String & p_gameName, int p_initiatorID );
		void _loadRooms();
		void _loadGames();

	public:
		inline std::shared_ptr< ChatWorld > GetWorld()
		{
			return m_world;
		}
		inline std::shared_ptr< ChatDatabase > GetDatabase()
		{
			return m_database;
		}
	};

	//**************************************************************************
}

#endif
