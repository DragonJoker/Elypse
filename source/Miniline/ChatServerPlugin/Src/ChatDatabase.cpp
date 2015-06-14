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
#include "ChatDatabase.h"
#include "ChatWorld.h"
#include "ChatClient.h"
#include "Buffer.h"
#include "StringConverter.h"

#include <Database/Database.h>
#include <Database/DatabaseConnection.h>
#include <Database/DatabaseLogger.h>
#include <Database/DatabasePluginManager.h>
#include <Database/DatabaseFactoryManager.h>
#include <Database/DatabaseResult.h>
#include <Database/DatabaseRow.h>

#if defined( _WIN32 )
#if defined( NDEBUG )
static const Database::String MYSQL_PLUGIN = STR( "DatabasePluginMySql.dll" );
#else
static const Database::String MYSQL_PLUGIN = STR( "DatabasePluginMySqld.dll" );
#endif
#else
#if defined( NDEBUG )
static const Database::String MYSQL_PLUGIN = STR( "libDatabasePluginMySql.so" );
#else
static const Database::String MYSQL_PLUGIN = STR( "libDatabasePluginMySqld.so" );
#endif
#endif
static const Database::String DATABASE_MYSQL_TYPE = STR( "Database.MySql" );

using namespace General::Templates;
using namespace General::Utils;

using namespace Chat;

namespace
{
	static const Database::String c_loadUsersQuery = STR( "select id_user, login from tbl_user" );
	static const Database::String c_connectQuery = STR( "select id_user from tbl_user where login='%s' and pwd='%s'" );
	static const Database::String c_loadSexQuery = STR( "select sexe from tbl_user where id_user='%i'" );
	static const Database::String c_updateSexQuery = STR( "update tbl_user set sexe='%s' where id_user='%i'" );
	static const Database::String c_loadDressesQuery = STR( "select id_player_dress, ref_slot, ref_dress from mini_player_dress where ref_player='%i'" );
	static const Database::String c_loadDressMatQuery = STR( "select ref_dress_mat, submat_index from mini_player_dress_mat, mini_player_dress, mini_dress_mats where id_player_dress='%i' and ref_player_dress=id_player_dress and mini_player_dress.ref_dress=mini_dress_mats.ref_dress" );
	static const Database::String c_loadTattoosQuery = STR( "select id_player_tattoo, ref_slot, ref_tattoo from mini_player_tattoo where ref_player='%i'" );
	static const Database::String c_removeDressesQuery = STR( "delete from mini_player_dress where ref_player='%i'" );
	static const Database::String c_removeTattoosQuery = STR( "delete from mini_player_tattoo where ref_player='%i'" );
	static const Database::String c_retrievePlayerDressQuery = STR( "select id_player_dress, ref_dress from mini_player_dress where ref_player='%i'" );
	static const Database::String c_retrieveOnePlayerDressQuery = STR( "select id_player_dress from mini_player_dress where ref_player='%i' and ref_dress='%i'" );
	static const Database::String c_removePlayerDressMatsQuery = STR( "delete from mini_player_dress_mat where ref_player_dress='%s'" );
	static const Database::String c_insertDressQuery = STR( "insert into mini_player_dress (ref_player, ref_dress, ref_slot) values( '%i', '%i', '%i')" );
	static const Database::String c_insertDressMatQuery = STR( "insert into mini_player_dress_mat (ref_player_dress, ref_dress_mat, submat) values( '%i', '%i', '%i')" );
	static const Database::String c_insertTattooQuery = STR( "insert into mini_player_tattoo (ref_player, ref_tattoo, ref_slot) values( '%i', '%i', '%i')" );
	static const Database::String c_getNumDressSlotsQuery = STR( "select count(*) from mini_slots" );
	static const Database::String c_getNumTattooSlotsQuery = STR( "select count(*) from mini_paint_slots" );
	static const Database::String c_loadRoomsQuery = STR( "select * from chat_room" );
	static const Database::String c_loadGamesQuery = STR( "select name, id_game, max_players from games where max_players > 1" );
	static const Database::String c_addPlayerToGameQuery = STR( "update games set current_players=current_players+1 where id_game='%i'" );
	static const Database::String c_removePlayerFromGameQuery = STR( "update games set current_players=current_players-1 where id_game='%i'" );
	static const Database::String c_loadFriendsQuery = STR( "select tbl_user.id_user, tbl_user.login from chat_friends,tbl_user where chat_friends.ref_user='%i' and chat_friends.ref_friend=tbl_user.id_user" );
	static const Database::String c_loadIgnoredQuery = STR( "select tbl_user.id_user, tbl_user.login from chat_ignored,tbl_user where chat_ignored.ref_user='%i' and chat_ignored.ref_ignored=tbl_user.id_user" );
	static const Database::String c_removeFriendQuery = STR( "delete from chat_friends where ref_user='%i' and ref_friend='%i'" );
	static const Database::String c_removeIgnoredQuery = STR( "delete from chat_ignored where ref_user='%i' and ref_ignored='%i'" );
	static const Database::String c_addFriendQuery = STR( "insert into chat_friends values( '%i', '%i')" );
	static const Database::String c_addIgnoredQuery = STR( "insert into chat_ignored values( '%i', '%i')" );
	static const Database::String c_loadAllDressesQuery = STR( "SELECT id_dress, id_slot, mini_dresses.sex AS dress_sex, mini_slots.sex AS slot_sex FROM mini_dresses, mini_slots, mini_dress_slots WHERE ref_dress = id_dress AND ref_slot = id_slot ORDER BY id_slot, id_dress" );
	static const Database::String c_loadAllTattoosQuery = STR( "SELECT id_tatoo, id_slot, mini_tattoos.sex AS tattoo_sex, mini_paint_slots.sex AS slot_sex FROM mini_tattoos, mini_paint_slots, mini_tattoo_slots WHERE ref_tattoo = id_tatoo AND ref_slot = id_slot ORDER BY id_slot, id_tatoo" );

	static const size_t MAX_QUERY_LEN = 4096;

	void Format( Database::String & p_result, size_t p_count, Database::String const & p_format, ... )
	{
		std::vector< Database::TChar > l_result( p_count + 1 );
		va_list l_list;
		va_start( l_list, p_format );
		vsnprintf( l_result.data(), p_count, p_format.c_str(), l_list );
		va_end( l_list );
	}
}

ChatDatabase::ChatDatabase( String const & p_path )
	: m_manager( NULL )
	, m_numDressSlots( 0 )
	, m_numTattooSlots( 0 )
	, m_modulePath( p_path )
{

	Database::String l_modulePath( m_modulePath );
	Database::CPluginManager::Instance().SetApplicationPath( l_modulePath );
	Database::CPluginManager::Instance().SetPluginsPath( l_modulePath );
	Database::CPluginManager::Instance().SetTranslationsPath( l_modulePath );
	Database::CPluginManager::Instance().LoadPlugin( l_modulePath + MYSQL_PLUGIN );
	m_manager = Database::CFactoryManager::Instance().CreateInstance( DATABASE_MYSQL_TYPE );

	ConfigInfo l_infos;
	std::swap( m_configInfo, l_infos );

	//m_configInfo.DBName = "kazugame_prod";
	//m_configInfo.DBUser = "miniline";
	//m_configInfo.DBPass = "b1cr0sS3r";
	//m_configInfo.DBHost = "r22764.ovh.net";

	m_configInfo.DBName = "kazugame_prod";
	m_configInfo.DBUser = "root";
	m_configInfo.DBPass = "";
	m_configInfo.DBHost = "127.0.0.1";
	m_configInfo.DBPort = 3306;
}

ChatDatabase::~ChatDatabase()
{
	if ( m_manager )
	{
		if ( m_db )
		{
			m_db.reset();
			m_manager->RemoveConnection();
		}

		delete m_manager;
	}

	Database::String l_modulePath( m_modulePath );
	Database::CPluginManager::Instance().UnloadPlugin( l_modulePath + MYSQL_PLUGIN );
}

bool ChatDatabase::Initialize()
{
	bool l_return = false;

	if ( !m_configInfo.DBName.empty() && !m_configInfo.DBUser.empty() && m_configInfo.DBPort > 1024 )
	{
		Database::String l_result;
		m_manager->Initialize( m_configInfo.DBHost, m_configInfo.DBName, m_configInfo.DBUser, m_configInfo.DBPass );
		m_manager->CreateConnection( l_result );
		m_db = m_manager->RetrieveConnection();
		l_return = Update();
	}

	return l_return;
}

bool ChatDatabase::Update()
{
	Database::DatabaseResultPtr l_result = m_db->ExecuteSelect( c_loadUsersQuery );

	if ( !l_result || l_result->GetRowCount() == 0 )
	{
		return false;
	}

	Database::DatabaseRowPtr l_row;
	unsigned long long l_rowCount = l_result->GetRowCount();

	m_clients.clear();
	unsigned int l_id;
	String l_name;

	for ( unsigned long long i = 0 ; i < l_rowCount ; i++ )
	{
		l_row = l_result->GetNextRow();
		l_row->Get( 0, l_id );
		l_row->Get( 1, l_name );
		m_clients.insert( std::make_pair( l_name, l_id ) );
	}

	l_result = m_db->ExecuteSelect( c_getNumDressSlotsQuery );

	if ( !l_result || l_result->GetRowCount() == 0 )
	{
		return false;
	}

	l_row = l_result->GetNextRow();
	l_row->Get( 0, m_numDressSlots );

	l_result = m_db->ExecuteSelect( c_getNumTattooSlotsQuery );

	if ( !l_result || l_result->GetRowCount() == 0 )
	{
		return false;
	}

	l_row = l_result->GetNextRow();
	l_row->Get( 0, m_numTattooSlots );

	l_result = m_db->ExecuteSelect( c_loadAllDressesQuery );

	if ( !l_result || l_result->GetRowCount() == 0 )
	{
		std::cerr << "There are no clothes in the database, is it empty?" << std::endl;
		return false;
	}

	m_allDresses.clear();
	l_rowCount = l_result->GetRowCount();
	int l_dressId;
	int l_slotId;
	int l_slotSex;
	int l_dressSex;
	std::map <int, std::map <int, std::vector <int> > >::iterator l_slotsIt;
	std::map <int, std::vector <int> >::iterator l_dressesIt;

	for ( unsigned long long i = 0 ; i < l_rowCount ; i++ )
	{
		l_row = l_result->GetNextRow();
		l_dressId = l_row->Get< int >( 0 );
		l_slotId = l_row->Get< int >( 1 );
		l_dressSex = l_row->Get< int >( 2 );
		l_slotSex = l_row->Get< int >( 3 );
		l_slotsIt = m_allDresses.find( l_dressSex );

		//std::clog << "ChatDatabase::Update - Dress : " << l_dressId << ", Sex : " << l_dressSex << " - Slot : " << l_slotId << ", Sex : " << l_slotSex << "\n";
		if ( l_slotsIt == m_allDresses.end() )
		{
			std::map <int, std::vector <int> > l_map;
			m_allDresses.insert( std::make_pair( l_dressSex, l_map ) );
			l_slotsIt = m_allDresses.find( l_dressSex );
		}

		l_dressesIt = l_slotsIt->second.find( l_slotId );

		if ( l_slotsIt->second.find( l_slotId ) == l_slotsIt->second.end() )
		{
			std::vector <int> l_array;
			l_slotsIt->second.insert( std::make_pair( l_slotId, l_array ) );
			l_dressesIt = l_slotsIt->second.find( l_slotId );
		}

		l_dressesIt->second.push_back( l_dressId );
	}


	l_result = m_db->ExecuteSelect( c_loadAllTattoosQuery );

	if ( !l_result || l_result->GetRowCount() == 0 )
	{
		return false;
	}

	m_allTattoos.clear();
	l_rowCount = l_result->GetRowCount();
	int l_tattooId;
	int l_tattooSex;
	std::map <int, std::vector <int> >::iterator l_tattoosIt;

	for ( unsigned long long i = 0 ; i < l_rowCount ; i++ )
	{
		l_row = l_result->GetNextRow();
		l_tattooId = l_row->Get< int >( 0 );
		l_slotId = l_row->Get< int >( 1 );
		l_tattooSex = l_row->Get< int >( 2 );
		l_slotSex = l_row->Get< int >( 3 );
		l_slotsIt = m_allTattoos.find( l_tattooSex );

		//std::clog << "ChatDatabase::Update - Tattoo : " << l_tattooId << ", Sex : " << l_tattooSex << " - Slot : " << l_slotId << ", Sex : " << l_slotSex << "\n";
		if ( l_slotsIt == m_allTattoos.end() )
		{
			std::map <int, std::vector <int> > l_map;
			m_allTattoos.insert( std::make_pair( l_tattooSex, l_map ) );
			l_slotsIt = m_allTattoos.find( l_tattooSex );
		}

		l_tattoosIt = l_slotsIt->second.find( l_slotId );

		if ( l_slotsIt->second.find( l_slotId ) == l_slotsIt->second.end() )
		{
			std::vector <int> l_array;
			l_slotsIt->second.insert( std::make_pair( l_slotId, l_array ) );
			l_tattoosIt = l_slotsIt->second.find( l_slotId );
		}

		l_tattoosIt->second.push_back( l_tattooId );
	}

	return true;
}

bool ChatDatabase::ConnectUser( LoginInformations & p_client )
{
	bool l_return = false;
	Database::String l_query;
	Format( l_query, MAX_QUERY_LEN,
			c_connectQuery,
			p_client.GetName().c_str(),
			p_client.GetPass().c_str() );

	Database::DatabaseResultPtr l_result = m_db->ExecuteSelect( l_query );

	if ( l_result && l_result->GetRowCount() > 0 )
	{
		int l_id = l_result->GetNextRow()->Get< int >( 0 );
		//std::clog << "Connect User - ID : " << l_id << "\n";
		p_client.SetId( static_cast <unsigned int>( l_id ) );
		l_return = true;
	}

	return l_return;
}

bool ChatDatabase::LoadClothes( Clothes & p_clothes, int p_id )
{
	Database::String l_query;
	Format( l_query, MAX_QUERY_LEN,
			c_loadSexQuery,
			p_id );

	Database::DatabaseResultPtr l_result1 = m_db->ExecuteSelect( l_query );

	if ( !l_result1 || l_result1->GetRowCount() == 0 )
	{
		std::cout << "ChatDatabase::LoadDresses - can't retrieve sex\n";
		return NULL;
	}

	String l_sex = l_result1->GetNextRow()->Get< String >( 0 );
	//std::clog << "ChatDatabase::LoadDresses - Sex : " << l_sex << "\n";

	p_clothes.m_sex = ( l_sex == "homme" ? 0 : 1 );

	{
		// DRESSES
		Format( l_query, MAX_QUERY_LEN,
				c_loadDressesQuery,
				p_id );
		Database::DatabaseResultPtr l_result = m_db->ExecuteSelect( l_query );

		if ( !l_result || l_result->GetRowCount() <= 0 )
		{
			std::cout << "ChatDatabase::LoadDresses - can't retrieve dresses\n";
			return false;
		}

		uint32_t l_rowCount = l_result->GetRowCount();

		for ( uint32_t i = 0 ; i < l_rowCount ; i++ )
		{
			Database::DatabaseRowPtr l_row = l_result->GetNextRow();
			int l_playerDressId = l_row->Get< int >( 0 );
			Dress l_dress;
			l_dress.m_slot = ChatClientDressSlot( l_row->Get< int >( 1 ) );
			l_dress.m_id = l_row->Get< int >( 2 );
			Format( l_query, MAX_QUERY_LEN,
					c_loadDressMatQuery,
					l_playerDressId );
			Database::DatabaseResultPtr l_result2 = m_db->ExecuteSelect( l_query );

			if ( !l_result2 )
			{
				std::cout << "ChatDatabase::LoadDresses - can't retrieve dresses\n";
				return false;
			}

			uint32_t l_rowCount2 = l_result2->GetRowCount();

			for ( uint32_t j = 0 ; j < l_rowCount2 ; j++ )
			{
				Database::DatabaseRowPtr l_row2 = l_result2->GetNextRow();
				int l_submatId = l_row2->Get< int >( 1 );
				int l_matIndex = l_row2->Get< int >( 0 );
				l_dress.m_materials.insert( std::make_pair( l_submatId, l_matIndex ) );
			}

			p_clothes.m_dresses.insert( std::make_pair( l_dress.m_slot, l_dress ) );
		}
	} // END DRESSES

	{
		// TATTOOS
		Format( l_query, MAX_QUERY_LEN,
				c_loadTattoosQuery,
				p_id );

		Database::DatabaseResultPtr l_result = m_db->ExecuteSelect( l_query );

		if ( !l_result || l_result->GetRowCount() <= 0 )
		{
			std::cout << "ChatDatabase::LoadDresses - can't retrieve tattoos\n";
			return true;
		}

		uint32_t l_rowCount = l_result->GetRowCount();

		for ( uint32_t i = 0 ; i < l_rowCount ; i++ )
		{
			Database::DatabaseRowPtr l_row = l_result->GetNextRow();
			int l_playerTattooId = l_row->Get< int >( 0 );
			Tattoo l_tattoo;
			l_tattoo.m_slot = ChatClientDressSlot( l_row->Get< int >( 1 ) );
			l_tattoo.m_id = l_row->Get< int >( 2 );
			p_clothes.m_tattoos.insert( std::make_pair( l_tattoo.m_slot, l_tattoo ) );
		}
	} // END TATTOOS

	return true;
}

bool ChatDatabase::SaveClothes( Clothes const & p_clothes, int p_id )
{
	if ( DoFlushDBFromPlayerInfos( p_id ) )
	{
		return DoFillDBWithPlayerInfos( p_clothes, p_id );
	}

	return false;
}

bool ChatDatabase::RemoveFriend( unsigned int p_id, unsigned int p_friend )
{
	Database::String l_query;
	Format( l_query, MAX_QUERY_LEN, c_removeFriendQuery,
			p_id, p_friend );
	m_db->ExecuteUpdate( l_query );

	return true;
}

bool ChatDatabase::RemoveIgnored( unsigned int p_id, unsigned int p_ignored )
{
	Database::String l_query;
	Format( l_query, MAX_QUERY_LEN, c_removeIgnoredQuery,
			p_id, p_ignored );
	m_db->ExecuteUpdate( l_query );

	return true;
}

bool ChatDatabase::AddFriend( unsigned int p_id, unsigned int p_friend )
{
	Database::String l_query;
	Format( l_query, MAX_QUERY_LEN, c_addFriendQuery,
			p_id, p_friend );
	m_db->ExecuteUpdate( l_query );

	return true;
}

bool ChatDatabase::AddIgnored( unsigned int p_id, unsigned int p_ignored )
{
	Database::String l_query;
	Format( l_query, MAX_QUERY_LEN, c_addIgnoredQuery,
			p_id, p_ignored );
	m_db->ExecuteUpdate( l_query );

	return true;
}

bool ChatDatabase::LoadFriends( ClientIdStrMap & p_friends, int p_id )
{
	Database::String l_query;
	Format( l_query, MAX_QUERY_LEN,
			c_loadFriendsQuery,
			p_id );

	Database::DatabaseResultPtr l_result = m_db->ExecuteSelect( l_query );

	if ( l_result != NULL && l_result->GetRowCount() > 0 )
	{
		unsigned long long l_rowCount = l_result->GetRowCount();
		Database::DatabaseRowPtr l_row;
		int l_id;
		String l_name;

		for ( size_t i = 0 ; i < l_rowCount ; i++ )
		{
			l_row = l_result->GetNextRow();
			l_id = l_row->Get< int >( 0 );
			l_name = l_row->Get< String >( 1 );
			p_friends.insert( std::make_pair( l_name, l_id ) );
		}
	}

	return true;
}

bool ChatDatabase::LoadIgnored( ClientIdStrMap & p_ignored, int p_id )
{
	Database::String l_query;
	Format( l_query, MAX_QUERY_LEN,
			c_loadIgnoredQuery,
			p_id );

	Database::DatabaseResultPtr l_result = m_db->ExecuteSelect( l_query );

	if ( l_result != NULL && l_result->GetRowCount() > 0 )
	{
		unsigned long long l_rowCount = l_result->GetRowCount();
		Database::DatabaseRowPtr l_row;
		int l_id;
		String l_name;

		for ( size_t i = 0 ; i < l_rowCount ; i++ )
		{
			l_row = l_result->GetNextRow();
			l_id = l_row->Get< int >( 0 );
			l_name = l_row->Get< String >( 1 );
			p_ignored.insert( std::make_pair( l_name, l_id ) );
		}
	}

	return true;
}

bool ChatDatabase::LoadRooms( ChatWorld & p_world )
{
	bool l_return = false;
	std::cout << "******************************" << std::endl;
	std::cout << "LoadingRooms" << std::endl;

	//std::clog << "ChatDatabase::LoadRooms - " << c_loadRoomsQuery << "\n";
	Database::DatabaseResultPtr l_result = m_db->ExecuteSelect( c_loadRoomsQuery );

	if ( l_result )
	{
		String l_roomName;
		String l_sceneName;
		unsigned long long l_rowCount = l_result->GetRowCount();
		Database::DatabaseRowPtr l_row;
		std::cout << "NbRooms : " << l_rowCount << "\n";

		for ( size_t i = 0 ; i < l_rowCount ; i++ )
		{
			l_row = l_result->GetNextRow();
			l_roomName = l_row->Get< String >( 1 );
			l_sceneName = l_row->Get< String >( 2 );
			std::cout << l_roomName << " - " << l_sceneName << "\n";
			p_world.AddRoom( l_roomName, l_sceneName );
		}

		l_return = true;
	}
	else
	{
		std::cerr << "ChatDatabase Error" << std::endl;
	}

	std::cout << "******************************" << std::endl;
	return l_return;
}

bool ChatDatabase::LoadGames( StrUIntIdMap & p_names )
{
	bool l_return = false;
	std::cout << "LoadingGames\n";
	Database::DatabaseResultPtr l_result = m_db->ExecuteSelect( c_loadGamesQuery );

	if ( l_result != NULL )
	{
		String l_gameName;
		unsigned int l_gameId;
		unsigned int l_maxPlayers;
		unsigned long long l_rowCount = l_result->GetRowCount();
		Database::DatabaseRowPtr l_row;
		std::cout << "NbGames : " << l_rowCount << "\n";

		for ( size_t i = 0 ; i < l_rowCount ; i++ )
		{
			l_row = l_result->GetNextRow();
			l_gameName = l_row->Get< String >( 0 );
			l_gameId = l_row->Get< int >( 1 );
			l_maxPlayers = l_row->Get< int >( 2 );

			if ( p_names.find( l_gameId ) == p_names.end() )
			{
				std::cout << l_gameId << " - " << l_gameName << " - " << l_maxPlayers << "\n";
				p_names.insert( std::make_pair( l_gameId, std::make_pair( l_gameName, l_maxPlayers ) ) );
			}
		}

		l_return = true;
	}
	else
	{
		std::cerr << "ChatDatabase Error" << std::endl;
	}

	std::cout << "******************************" << std::endl;
	return false;
}

unsigned int ChatDatabase::GetClientId( const String & p_name )
{
	ClientIdStrMap::iterator l_it = m_clients.find( p_name );

	if ( l_it != m_clients.end() )
	{
		return l_it->second;
	}

	return 0;
}

void ChatDatabase::AddPlayerToGame( unsigned int p_gameId )
{
	Database::String l_query;
	Format( l_query, MAX_QUERY_LEN,
			c_addPlayerToGameQuery,
			p_gameId );
	m_db->ExecuteUpdate( l_query );
}

void ChatDatabase::RemovePlayerFromGame( unsigned int p_gameId )
{
	Database::String l_query;
	Format( l_query, MAX_QUERY_LEN,
			c_removePlayerFromGameQuery,
			p_gameId );
	m_db->ExecuteUpdate( l_query );
}

size_t ChatDatabase::GetDressesSize( size_t p_sex, size_t p_slotId )
{
//	std::cout << "ChatDatabase::GetDressesSize - Sex : " << p_sex << " - Slot " << p_slotId << "\n";
	std::map <int, std::map <int, std::vector <int> > >::iterator l_it = m_allDresses.find( p_sex );

	if ( l_it == m_allDresses.end() )
	{
		return 0;
	}

	std::map <int, std::vector <int> >::iterator l_it2 = l_it->second.find( p_slotId );

	if ( l_it2 == l_it->second.end() )
	{
		return 0;
	}

	return l_it2->second.size();
}

int ChatDatabase::GetDress( size_t p_sex, size_t p_slotId, size_t p_index )
{
	std::clog << "ChatDatabase::GetDress - Sex : " << p_sex << " - Slot " << p_slotId << " - Index " << p_index << "\n";
	std::map <int, std::map <int, std::vector <int> > >::iterator l_it = m_allDresses.find( p_sex );

	if ( l_it == m_allDresses.end() )
	{
		return 137;
	}

	std::map <int, std::vector <int> >::iterator l_it2 = l_it->second.find( p_slotId );

	if ( l_it2 == l_it->second.end() || p_index >= l_it2->second.size() )
	{
		return 137;
	}

	return l_it2->second[p_index];
}

bool ChatDatabase::DoFlushDBFromPlayerInfos( int p_id )
{
	bool l_return = false;
	Database::String l_query;
	// first, we flush the db infos about the player's dresses
	Format( l_query, MAX_QUERY_LEN,
			c_retrievePlayerDressQuery,
			p_id );
	Database::DatabaseResultPtr l_result = m_db->ExecuteSelect( l_query );

	if ( !l_result )
	{
		std::cerr << "ChatDatabase::DoFlushDBFromPlayerInfos - Can't execute query #1\n";
		l_return = false;
	}
	else
	{
		unsigned long long l_rowCount = l_result->GetRowCount();
		std::clog << "ChatDatabase::DoFlushDBFromPlayerInfos - Query #1 - Nb Rows : " << l_rowCount << "\n";

		if ( l_rowCount > 0 )
		{
			Format( l_query, MAX_QUERY_LEN,
					c_removePlayerDressMatsQuery,
					l_result->GetNextRow()->Get< String >( 0 ).c_str() );

			for ( unsigned long long i = 1 ; i < l_rowCount ; i++ )
			{
				l_query += " or ref_player_dress='";
				l_query += l_result->GetNextRow()->Get< String >( 0 );
				l_query += "'";
			}

			m_db->ExecuteUpdate( l_query );
			std::clog << "ChatDatabase::DoFlushDBFromPlayerInfos - Query #2 -> Success\n";

			Format( l_query, MAX_QUERY_LEN,
					c_removeDressesQuery,
					p_id );
			m_db->ExecuteUpdate( l_query );
			std::clog << "ChatDatabase::DoFlushDBFromPlayerInfos - Query #3 -> Success\n";
		}

		Format( l_query, MAX_QUERY_LEN,
				c_removeTattoosQuery,
				p_id );
		m_db->ExecuteUpdate( l_query );
		std::clog << "ChatDatabase::DoFlushDBFromPlayerInfos - Query #4 -> Success\n";
		l_return = true;
	}

	return l_return;
}

bool ChatDatabase::DoFillDBWithPlayerInfos( Clothes const & p_clothes, int p_id )
{
	bool l_return = false;
	Database::String l_query;
	Format( l_query, MAX_QUERY_LEN,
			c_updateSexQuery,
			( p_clothes.m_sex == 1 ? "femme" : "homme" ),
			p_id );
	m_db->ExecuteUpdate( l_query );
	std::clog << "ChatDatabase::DoFillDBWithPlayerInfos - Query #1 -> Success\n";

	std::clog << "ChatDatabase::DoFillDBWithPlayerInfos - Nb Dresses : " << p_clothes.m_dresses.size() << "\n";
	auto && l_dressIt = p_clothes.m_dresses.begin();
	Format( l_query, MAX_QUERY_LEN,
			c_insertDressQuery,
			p_id,
			l_dressIt->second.m_id,
			l_dressIt->second.m_slot );
	l_dressIt++;

	size_t l_length = MAX_QUERY_LEN;

	while ( l_dressIt != p_clothes.m_dresses.end() )
	{
		l_query += ", ('";
		l_query += Database::CStrUtils::ToString( p_id );
		l_query += "', '";
		l_query += Database::CStrUtils::ToString( l_dressIt->second.m_id );
		l_query += "', '";
		l_query += Database::CStrUtils::ToString( l_dressIt->second.m_slot );
		l_query += "')";
		l_dressIt++;
	}

	m_db->ExecuteUpdate( l_query );
	std::clog << "ChatDatabase::DoFillDBWithPlayerInfos - Query #2 -> Success\n";

	Format( l_query, MAX_QUERY_LEN,
			c_retrievePlayerDressQuery,
			p_id );
	Database::DatabaseResultPtr l_result = m_db->ExecuteSelect( l_query );

	if ( !l_result || !l_result->GetRowCount() )
	{
		std::cerr << "ChatDatabase::DoFillDBWithPlayerInfos - can't execute query #6\n";
	}
	else
	{
		uint64_t l_rowCount = l_result->GetRowCount();
		std::clog << "ChatDatabase::DoFillDBWithPlayerInfos - Query #3 - Nb Rows : " << l_rowCount << "\n";
		std::map <int, int> l_map;

		for ( uint64_t i = 0 ; i < l_rowCount ; i++ )
		{
			Database::DatabaseRowPtr l_row = l_result->GetNextRow();
			int l_dress = l_row->Get< int >( 1 );
			int l_pdress = l_row->Get< int >( 0 );
			l_map.insert( std::make_pair( l_dress, l_pdress ) );
			std::clog << "ChatDatabase::DoFillDBWithPlayerInfos - DressId : " << l_dress << " - PlayerDressId : " << l_pdress << "\n";
		}

		std::map <int, int>::iterator l_mapIt;
		int l_playerDressId;

		for ( auto && l_it : p_clothes.m_dresses )
		{
			std::clog << "ChatDatabase::DoFillDBWithPlayerInfos - Dress Id : " << l_it.second.m_id << "\n";
			l_mapIt = l_map.find( l_it.second.m_id );

			if ( l_mapIt != l_map.end() )
			{
				std::clog << "ChatDatabase::DoFillDBWithPlayerInfos - Found - Nb materials : " << l_it.second.m_materials.size() << "\n";
				l_playerDressId = l_mapIt->second;

				for ( auto && l_matit : l_it.second.m_materials )
				{
					std::clog << "ChatDatabase::DoFillDBWithPlayerInfos - " << l_playerDressId << " - " << l_matit.second << " - " << l_matit.first << "\n";
					Format( l_query, MAX_QUERY_LEN,
							c_insertDressMatQuery,
							l_playerDressId,
							l_matit.second,
							l_matit.first );
					m_db->ExecuteUpdate( l_query );
					std::clog << "ChatDatabase::DoFillDBWithPlayerInfos - Query #4 : " << l_query << "-> Success\n";
				}
			}
		}

		auto && l_tattooIt = p_clothes.m_tattoos.begin();

		if ( l_tattooIt != p_clothes.m_tattoos.end() )
		{
			Format( l_query, MAX_QUERY_LEN,
					c_insertTattooQuery,
					p_id,
					l_tattooIt->second.m_id,
					l_tattooIt->second.m_slot );
			l_tattooIt++;

			while ( l_tattooIt != p_clothes.m_tattoos.end() )
			{
				std::clog << "ChatDatabase::DoFillDBWithPlayerInfos - Tattoo : ID : " << l_tattooIt->second.m_id << ", Slot : " << l_tattooIt->second.m_slot << "\n";
				l_query += ", ('";
				l_query += Database::CStrUtils::ToString( p_id );
				l_query += "', '";
				l_query += Database::CStrUtils::ToString( l_tattooIt->second.m_id );
				l_query += "', '";
				l_query += Database::CStrUtils::ToString( l_tattooIt->second.m_slot );
				l_query += "')";
				l_tattooIt++;
			}

			m_db->ExecuteUpdate( l_query );
			std::clog << "ChatDatabase::DoFillDBWithPlayerInfos - Query #5 -> Success\n";
		}

		l_return = true;
	}

	return l_return;
}
