#ifndef ___ChatPlugin___
#define ___ChatPlugin___

#include "Common.h"
#include <ElypsePlugin.h>
#include <ElypseClient.h>
#include <ElypseService.h>

using namespace Elypse::ServerPlugin;

namespace Chat
{
	//**************************************************************************

	class ChatTcpService : public ElypseTcpService
	{
	private:
		ChatPlugin * m_plugin;

	public:
		ChatTcpService( ChatPlugin * p_plugin );
		~ChatTcpService();

	private:
		virtual TcpBaseClient * _createNewClient();
		virtual void _deleteClient( TcpBaseClient * p_toDelete );
		void ___deleteClient( TcpBaseClient * p_toDelete );
	};

	//**************************************************************************

	class ChatUdpService : public ElypseUdpService
	{
	public:
		ChatUdpService();
		~ChatUdpService();
	};

	//**************************************************************************

	class ChatPlugin : public ElypsePlugin
	{
	private:
		ElypseServiceArray m_services;
		ChatWorld * m_world;
		ClientIdChatCLientMap m_connectedClients;
		ClientIdStrMap m_clientIDs;
		ChatDatabase * m_database;
		ChatGameStrMap m_games;
		StrUIntIdMap m_gamesNames;

	public:
		ChatPlugin( String const & p_path );
		~ChatPlugin();

		virtual unsigned short GetVersionNo();
		ElypseServiceArray GetServices();

		void AddRoom( ChatRoom * p_room );
		ChatRoom * GetRoom( const String & p_roomName );

		ChatTcpClient * GetClient( const String & p_name );
		const String & GetClientName( unsigned int p_id );
		void AddClient( ChatTcpClient * p_client );
		void RemoveClient( ChatTcpClient * p_client );

		ChatGame * AddGame( const String & p_gameName, ChatTcpClient * p_initiator );
		ChatGame * GetGame( const String & p_gameName, int p_initiatorID );
		StrUIntIdMap GetGamesList( const String & p_gameName );
		unsigned int GetGameMaxPLayers( const String & p_gameName );
		void RemoveGame( const String & p_gameName, int p_initiatorID );
		void _loadRooms();
		void _loadGames();

	public:
		inline ChatWorld  *	GetWorld()
		{
			return m_world;
		}
		inline  ChatDatabase * GetDatabase()
		{
			return m_database;
		}
	};

	//**************************************************************************
}

#endif
