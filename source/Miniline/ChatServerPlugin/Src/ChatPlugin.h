/* See Licence.txt in root folder */
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
		: public Elypse::Server::ElypseTcpService
		, public std::enable_shared_from_this< ChatTcpService >
	{
	private:
		ChatPlugin * m_plugin;

	public:
		ChatTcpService( ChatPlugin * p_plugin );
		~ChatTcpService();

	private:
		virtual General::Network::TcpBaseClient * DoCreateClient();
		virtual void DoDestroyClient( General::Network::TcpBaseClient * p_toDelete );
		void DoDeleteClient( General::Network::TcpBaseClient * p_toDelete );
	};

	//**************************************************************************

	class ChatUdpService
		: public Elypse::Server::ElypseUdpService
		, public std::enable_shared_from_this< ChatUdpService >
	{
	public:
		ChatUdpService();
		~ChatUdpService();
	};

	//**************************************************************************

	class ChatPlugin
		: public Elypse::Server::ElypsePlugin
	{
	private:
		Elypse::Server::ElypseServiceArray m_services;
		std::shared_ptr< ChatWorld > m_world;
		ClientIdChatCLientMap m_connectedClients;
		ClientStrIdMap m_clients;
		std::shared_ptr< ChatDatabase > m_database;
		ChatGameStrMap m_games;
		StrUIntIdMap m_gamesNames;

	public:
		ChatPlugin( String const & p_path );
		~ChatPlugin();

		virtual uint16_t GetVersionNo();
		virtual Elypse::Server::ElypseServiceArray const & GetServices()const;

		void AddRoom( std::shared_ptr< ChatRoom > p_room );
		std::shared_ptr< ChatRoom > GetRoom( String const & p_roomName );

		ChatTcpClient * GetClient( uint32_t p_id );
		String const & GetClientName( uint32_t p_id );
		uint32_t GetClientId( String const & p_name );
		void AddClient( ChatTcpClient * p_client );
		void RemoveClient( ChatTcpClient * p_client );

		std::shared_ptr< ChatGame > AddGame( String const & p_gameName, ChatTcpClient * p_initiator );
		std::shared_ptr< ChatGame > GetGame( String const & p_gameName, int p_initiatorID );
		StrUIntIdMap GetGamesList( String const & p_gameName );
		uint32_t GetGameMaxPLayers( String const & p_gameName );
		void RemoveGame( String const & p_gameName, int p_initiatorID );
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
