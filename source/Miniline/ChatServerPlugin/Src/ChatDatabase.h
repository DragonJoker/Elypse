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
