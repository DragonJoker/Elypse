#ifndef ___Database___
#define ___Database___

#include <cstdlib>
#include <cstdint>

#include "Common.h"

namespace Chat
{
	class ChatDatabase
	{
	private:
		Database::CDatabase * m_manager;
		ConfigInfo m_configInfo;
		Database::DatabaseConnectionPtr m_db;
		Database::String m_modulePath;
		ClientIdStrMap m_clients;
		uint64_t m_numDressSlots;
		uint64_t m_numTattooSlots;
		std::map <int, std::map <int, std::vector <int> > > m_allDresses;
		std::map <int, std::map <int, std::vector <int> > > m_allTattoos;

	public:
		ChatDatabase( String const & p_path );
		~ChatDatabase();

		bool Initialize();
		bool Update();
		unsigned char * Md5( unsigned char * p_toHash )const;
		bool ConnectUser( LoginInformations & p_client );
		bool LoadClothes( Clothes & p_clothes, int p_id );
		bool SaveClothes( Clothes const & p_clothes, int p_id );
		bool RemoveFriend( unsigned int p_id, unsigned int p_friend );
		bool RemoveIgnored( unsigned int p_id, unsigned int p_ignored );
		bool AddFriend( unsigned int p_id, unsigned int p_friend );
		bool AddIgnored( unsigned int p_id, unsigned int p_ignored );
		bool LoadFriends( ClientIdStrMap & p_friends, int p_id );
		bool LoadIgnored( ClientIdStrMap & p_ignored, int p_id );
		bool LoadRooms( ChatWorld & p_world );
		bool LoadGames( StrUIntIdMap & p_names );
		unsigned int GetClientId( const String & p_name );
		void AddPlayerToGame( unsigned int p_gameId );
		void RemovePlayerFromGame( unsigned int p_gameId );
		size_t GetDressesSize( size_t p_sex, size_t p_slotId );
		int GetDress( size_t p_sex, size_t p_slotId, size_t p_index );

		inline uint64_t GetNumDressSlots()
		{
			return m_numDressSlots;
		}

	private:
		bool DoFlushDBFromPlayerInfos( int p_id );
		bool DoFillDBWithPlayerInfos( Clothes const & p_clothes, int p_id );
	};
}

#endif
