/* See Licence.txt in root folder */
#ifndef ___Database___
#define ___Database___

#include <cstdlib>
#include <cstdint>

#include "Common.h"

namespace Chat
{
	static const uint32_t DEFAULT_DRESS_ID = 137;

	class ChatDatabase
	{
	public:
		ChatDatabase( String const & p_path );
		~ChatDatabase();

		bool Initialize();
		bool Update();
		unsigned char * Md5( unsigned char * p_toHash )const;
		bool ConnectUser( LoginInformations & p_client );
		bool LoadClothes( Clothes & p_clothes, uint32_t p_id );
		bool SaveClothes( Clothes const & p_clothes, uint32_t p_id );
		bool RemoveFriend( uint32_t p_id, uint32_t p_friend );
		bool RemoveIgnored( uint32_t p_id, uint32_t p_ignored );
		bool AddFriend( uint32_t p_id, uint32_t p_friend );
		bool AddIgnored( uint32_t p_id, uint32_t p_ignored );
		bool LoadFriends( ClientStrIdMap & p_friends, uint32_t p_id );
		bool LoadIgnored( ClientStrIdMap & p_ignored, uint32_t p_id );
		bool LoadRooms( ChatWorld & p_world );
		bool LoadGames( StrUIntIdMap & p_names );
		uint32_t GetClientId( String const & p_name );
		void AddPlayerToGame( uint32_t p_gameId );
		void RemovePlayerFromGame( uint32_t p_gameId );
		uint32_t GetDressesCount( uint32_t p_sex, uint32_t p_slotId );
		uint32_t GetDress( uint32_t p_sex, uint32_t p_slotId, uint32_t p_index );

		inline uint64_t GetDressSlotsCount()
		{
			return m_numDressSlots;
		}

	private:
		bool DoFlushDBFromPlayerInfos( uint32_t p_id );
		bool DoFillDBWithPlayerInfos( Clothes const & p_clothes, uint32_t p_id );

	private:
		Database::CDatabase * m_manager;
		ConfigInfo m_configInfo;
		Database::DatabaseConnectionSPtr m_db;
		Database::String m_modulePath;
		ClientStrIdMap m_clients;
		uint64_t m_numDressSlots;
		uint64_t m_numTattooSlots;
		UintArrayUIntMapUIntMap m_allDresses;
		UintArrayUIntMapUIntMap m_allTattoos;
	};
}

#endif
