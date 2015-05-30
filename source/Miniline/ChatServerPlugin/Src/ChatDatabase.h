#ifndef ___Database___
#define ___Database___

#include <stdlib.h>

#include "Common.h"

namespace Chat
{
	class ChatDatabase
	{
	private:
		Database::CDatabase * m_manager;
		ConfigInfo * m_configInfo;
		Database::DatabaseConnectionPtr m_db;
		Database::String m_modulePath;
		ClientIdStrMap m_clients;
		int m_numDressSlots;
		int m_numTattooSlots;
		std::map <int, std::map <int, std::vector <int> > > m_allDresses;
		std::map <int, std::map <int, std::vector <int> > > m_allTattoos;

	public:
		ChatDatabase( String const & p_path );
		~ChatDatabase();

		bool Initialize();
		bool Update();
		unsigned char * Md5( unsigned char * p_toHash )const;
		bool ConnectUser( ChatTcpClient * p_client );
		Clothes * LoadDresses( ChatTcpClient * p_client );
		bool UpdateClothes( ChatTcpClient * p_client );
		bool RemoveFriend( unsigned int p_id, unsigned int p_friend );
		bool RemoveIgnored( unsigned int p_id, unsigned int p_ignored );
		bool AddFriend( unsigned int p_id, unsigned int p_friend );
		bool AddIgnored( unsigned int p_id, unsigned int p_ignored );
		bool LoadFriends( ChatTcpClient * p_client );
		bool LoadIgnored( ChatTcpClient * p_client );
		bool LoadRooms( ChatWorld * p_world );
		bool LoadGames( StrUIntIdMap & p_names );
		bool ReloadRooms( ChatWorld * p_world );
		bool ReloadGames( StrUIntIdMap & p_names );
		unsigned int GetClientId( const String & p_name );
		void AddPlayerToGame( unsigned int p_gameId );
		void RemovePlayerFromGame( unsigned int p_gameId );
		size_t GetDressesSize( size_t p_sex, size_t p_slotId );
		int GetDress( size_t p_sex, size_t p_slotId, size_t p_index );

		inline int GetNumDressSlots()
		{
			return m_numDressSlots;
		}

	private:
		bool _flushDBFromPlayerInfos( ChatTcpClient * p_client );
		bool _fillDBWithPlayerInfos( ChatTcpClient * p_client );
	};
}

#endif
