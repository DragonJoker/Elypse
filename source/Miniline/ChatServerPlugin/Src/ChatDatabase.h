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
