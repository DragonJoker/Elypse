/* See Licence.txt in root folder */
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

	Database::String Format( Database::String const & p_format, size_t p_count, ... )
	{
		std::vector< Database::TChar > l_result( p_count + 1 );
		va_list l_list;
		va_start( l_list, p_count );
		int l_written = vsnprintf( l_result.data(), p_count, p_format.c_str(), l_list );
		va_end( l_list );
		return String( l_result.data(), l_written );
	}

	void AddElement( UintArrayUIntMapUIntMap & p_allElements, uint32_t p_elemId, uint32_t p_slotId, uint32_t p_elemSex, uint32_t p_slotSex )
	{
		p_allElements.insert( std::make_pair( p_elemSex, UintArrayUIntMap() ) ).first->second.insert( std::make_pair( p_slotId, UIntArray() ) ).first->second.push_back( p_elemId );
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
		m_manager->Initialise( m_configInfo.DBHost, m_configInfo.DBUser, m_configInfo.DBPass );
		m_manager->CreateConnection( l_result );
		m_db = m_manager->RetrieveConnection();
		m_db->SelectDatabase( m_configInfo.DBName );
		l_return = Update();
	}

	return l_return;
}

bool ChatDatabase::Update()
{
	Database::DatabaseResultSPtr l_result = m_db->ExecuteSelect( c_loadUsersQuery );

	if ( !l_result || l_result->GetRowCount() == 0 )
	{
		return false;
	}

	Database::DatabaseRowSPtr l_row;
	uint64_t l_rowCount = l_result->GetRowCount();

	m_clients.clear();
	uint32_t l_id;
	String l_name;

	for ( uint64_t i = 0 ; i < l_rowCount ; i++ )
	{
		l_row = l_result->GetNextRow();
		l_row->Get( 0, l_id );
		l_row->Get( 1, l_name );
		m_clients.insert( std::make_pair( l_id, l_name ) );
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

	for ( uint64_t i = 0 ; i < l_rowCount ; i++ )
	{
		l_row = l_result->GetNextRow();
		AddElement( m_allDresses, l_row->Get< uint32_t >( 0 ), l_row->Get< uint32_t >( 1 ), l_row->Get< uint32_t >( 2 ), l_row->Get< uint32_t >( 3 ) );
	}


	l_result = m_db->ExecuteSelect( c_loadAllTattoosQuery );

	if ( !l_result || l_result->GetRowCount() == 0 )
	{
		return false;
	}

	m_allTattoos.clear();
	l_rowCount = l_result->GetRowCount();

	for ( uint64_t i = 0 ; i < l_rowCount ; i++ )
	{
		l_row = l_result->GetNextRow();
		AddElement( m_allDresses, l_row->Get< uint32_t >( 0 ), l_row->Get< uint32_t >( 1 ), l_row->Get< uint32_t >( 2 ), l_row->Get< uint32_t >( 3 ) );
	}

	return true;
}

bool ChatDatabase::ConnectUser( LoginInformations & p_client )
{
	bool l_return = false;
	Database::String l_query = Format( c_connectQuery, MAX_QUERY_LEN, p_client.GetName().c_str(), p_client.GetPass().c_str() );

	Database::DatabaseResultSPtr l_result = m_db->ExecuteSelect( l_query );

	if ( l_result && l_result->GetRowCount() > 0 )
	{
		uint32_t l_id = l_result->GetNextRow()->Get< uint32_t >( 0 );
		//std::clog << "Connect User - ID : " << l_id << std::endl;
		p_client.SetId( static_cast <uint32_t>( l_id ) );
		l_return = true;
	}

	return l_return;
}

bool ChatDatabase::LoadClothes( Clothes & p_clothes, uint32_t p_id )
{
	Database::String l_query = Format( c_loadSexQuery, MAX_QUERY_LEN, p_id );

	Database::DatabaseResultSPtr l_result1 = m_db->ExecuteSelect( l_query );

	if ( !l_result1 || l_result1->GetRowCount() == 0 )
	{
		std::cout << "ChatDatabase::LoadDresses - can't retrieve sex" << std::endl;
		return NULL;
	}

	String l_sex = l_result1->GetNextRow()->Get< String >( 0 );
	//std::clog << "ChatDatabase::LoadDresses - Sex : " << l_sex << std::endl;

	p_clothes.m_sex = ( l_sex == "homme" ? 0 : 1 );

	{
		// DRESSES
		l_query = Format( c_loadDressesQuery, MAX_QUERY_LEN, p_id );
		Database::DatabaseResultSPtr l_result = m_db->ExecuteSelect( l_query );

		if ( !l_result || l_result->GetRowCount() <= 0 )
		{
			std::cout << "ChatDatabase::LoadDresses - can't retrieve dresses" << std::endl;
			return false;
		}

		uint32_t l_rowCount = l_result->GetRowCount();

		for ( uint32_t i = 0 ; i < l_rowCount ; i++ )
		{
			Database::DatabaseRowSPtr l_row = l_result->GetNextRow();
			int l_playerDressId = l_row->Get< uint32_t >( 0 );
			Dress l_dress;
			l_dress.m_slot = ChatClientDressSlot( l_row->Get< uint32_t >( 1 ) );
			l_dress.m_id = l_row->Get< uint32_t >( 2 );
			l_query = Format( c_loadDressMatQuery, MAX_QUERY_LEN, l_playerDressId );
			Database::DatabaseResultSPtr l_result2 = m_db->ExecuteSelect( l_query );

			if ( !l_result2 )
			{
				std::cout << "ChatDatabase::LoadDresses - can't retrieve dresses" << std::endl;
				return false;
			}

			uint32_t l_rowCount2 = l_result2->GetRowCount();

			for ( uint32_t j = 0 ; j < l_rowCount2 ; j++ )
			{
				Database::DatabaseRowSPtr l_row2 = l_result2->GetNextRow();
				uint32_t l_submatId = l_row2->Get< uint32_t >( 1 );
				uint32_t l_matIndex = l_row2->Get< uint32_t >( 0 );
				l_dress.m_materials.insert( std::make_pair( l_submatId, l_matIndex ) );
			}

			p_clothes.m_dresses.insert( std::make_pair( l_dress.m_slot, l_dress ) );
		}
	} // END DRESSES

	{
		// TATTOOS
		l_query = Format( c_loadTattoosQuery, MAX_QUERY_LEN, p_id );
		Database::DatabaseResultSPtr l_result = m_db->ExecuteSelect( l_query );

		if ( !l_result || l_result->GetRowCount() <= 0 )
		{
			std::cout << "ChatDatabase::LoadDresses - can't retrieve tattoos" << std::endl;
			return true;
		}

		uint32_t l_rowCount = l_result->GetRowCount();

		for ( uint32_t i = 0 ; i < l_rowCount ; i++ )
		{
			Database::DatabaseRowSPtr l_row = l_result->GetNextRow();
			uint32_t l_playerTattooId = l_row->Get< uint32_t >( 0 );
			Tattoo l_tattoo;
			l_tattoo.m_slot = ChatClientDressSlot( l_row->Get< uint32_t >( 1 ) );
			l_tattoo.m_id = l_row->Get< uint32_t >( 2 );
			p_clothes.m_tattoos.insert( std::make_pair( l_tattoo.m_slot, l_tattoo ) );
		}
	} // END TATTOOS

	return true;
}

bool ChatDatabase::SaveClothes( Clothes const & p_clothes, uint32_t p_id )
{
	if ( DoFlushDBFromPlayerInfos( p_id ) )
	{
		return DoFillDBWithPlayerInfos( p_clothes, p_id );
	}

	return false;
}

bool ChatDatabase::RemoveFriend( uint32_t p_id, uint32_t p_friend )
{
	Database::String l_query = Format( c_removeFriendQuery, MAX_QUERY_LEN,  p_id, p_friend );
	return m_db->ExecuteUpdate( l_query );
}

bool ChatDatabase::RemoveIgnored( uint32_t p_id, uint32_t p_ignored )
{
	Database::String l_query = Format( c_removeIgnoredQuery, MAX_QUERY_LEN, p_id, p_ignored );
	return m_db->ExecuteUpdate( l_query );
}

bool ChatDatabase::AddFriend( uint32_t p_id, uint32_t p_friend )
{
	Database::String l_query = Format( c_addFriendQuery, MAX_QUERY_LEN, p_id, p_friend );
	return m_db->ExecuteUpdate( l_query );
}

bool ChatDatabase::AddIgnored( uint32_t p_id, uint32_t p_ignored )
{
	Database::String l_query = Format( c_addIgnoredQuery, MAX_QUERY_LEN, p_id, p_ignored );
	return m_db->ExecuteUpdate( l_query );
}

bool ChatDatabase::LoadFriends( ClientStrIdMap & p_friends, uint32_t p_id )
{
	Database::String l_query = Format( c_loadFriendsQuery, MAX_QUERY_LEN, p_id );
	Database::DatabaseResultSPtr l_result = m_db->ExecuteSelect( l_query );

	if ( l_result != NULL && l_result->GetRowCount() > 0 )
	{
		uint64_t l_rowCount = l_result->GetRowCount();

		for ( size_t i = 0 ; i < l_rowCount ; i++ )
		{
			Database::DatabaseRowSPtr l_row = l_result->GetNextRow();
			uint32_t l_id = l_row->Get< uint32_t >( 0 );
			String l_name = l_row->Get< String >( 1 );
			p_friends.insert( std::make_pair( l_id, l_name ) );
		}
	}

	return true;
}

bool ChatDatabase::LoadIgnored( ClientStrIdMap & p_ignored, uint32_t p_id )
{
	Database::String l_query = Format( c_loadIgnoredQuery, MAX_QUERY_LEN, p_id );
	Database::DatabaseResultSPtr l_result = m_db->ExecuteSelect( l_query );

	if ( l_result != NULL && l_result->GetRowCount() > 0 )
	{
		uint64_t l_rowCount = l_result->GetRowCount();

		for ( size_t i = 0 ; i < l_rowCount ; i++ )
		{
			Database::DatabaseRowSPtr l_row = l_result->GetNextRow();
			uint32_t l_id = l_row->Get< uint32_t >( 0 );
			String l_name = l_row->Get< String >( 1 );
			p_ignored.insert( std::make_pair( l_id, l_name ) );
		}
	}

	return true;
}

bool ChatDatabase::LoadRooms( ChatWorld & p_world )
{
	bool l_return = false;
	std::cout << "******************************" << std::endl;
	std::cout << "LoadingRooms" << std::endl;

	//std::clog << "ChatDatabase::LoadRooms - " << c_loadRoomsQuery << std::endl;
	Database::DatabaseResultSPtr l_result = m_db->ExecuteSelect( c_loadRoomsQuery );

	if ( l_result )
	{
		uint64_t l_rowCount = l_result->GetRowCount();
		std::cout << "NbRooms : " << l_rowCount << std::endl;

		for ( size_t i = 0 ; i < l_rowCount ; i++ )
		{
			Database::DatabaseRowSPtr l_row = l_result->GetNextRow();
			String l_roomName = l_row->Get< String >( 1 );
			String l_sceneName = l_row->Get< String >( 2 );
			std::cout << l_roomName << " - " << l_sceneName << std::endl;
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
	std::cout << "LoadingGames" << std::endl;
	Database::DatabaseResultSPtr l_result = m_db->ExecuteSelect( c_loadGamesQuery );

	if ( l_result != NULL )
	{
		uint64_t l_rowCount = l_result->GetRowCount();
		std::cout << "NbGames : " << l_rowCount << std::endl;

		for ( size_t i = 0 ; i < l_rowCount ; i++ )
		{
			Database::DatabaseRowSPtr l_row = l_result->GetNextRow();
			String l_gameName = l_row->Get< String >( 0 );
			uint32_t l_gameId = l_row->Get< uint32_t >( 1 );
			uint32_t l_maxPlayers = l_row->Get< uint32_t >( 2 );

			if ( p_names.find( l_gameId ) == p_names.end() )
			{
				std::cout << l_gameId << " - " << l_gameName << " - " << l_maxPlayers << std::endl;
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
	return l_return;
}

uint32_t ChatDatabase::GetClientId( String const & p_name )
{
	uint32_t l_return = 0;
	auto l_it = std::find_if( m_clients.begin(), m_clients.end(), [&p_name]( ClientStrIdMap::value_type const & p_client )
	{
		return p_client.second == p_name;
	} );

	if ( l_it != m_clients.end() )
	{
		l_return = l_it->first;
	}

	return l_return;
}

void ChatDatabase::AddPlayerToGame( uint32_t p_gameId )
{
	Database::String l_query = Format( c_addPlayerToGameQuery, MAX_QUERY_LEN, p_gameId );
	m_db->ExecuteUpdate( l_query );
}

void ChatDatabase::RemovePlayerFromGame( uint32_t p_gameId )
{
	Database::String l_query = Format( c_removePlayerFromGameQuery, MAX_QUERY_LEN, p_gameId );
	m_db->ExecuteUpdate( l_query );
}

uint32_t ChatDatabase::GetDressesCount( uint32_t p_sex, uint32_t p_slotId )
{
	//std::cout << "ChatDatabase::GetDressesSize - Sex : " << p_sex << " - Slot " << p_slotId << std::endl;
	uint32_t l_return = 0;
	auto && l_it = m_allDresses.find( p_sex );

	if ( l_it != m_allDresses.end() )
	{
		auto && l_it2 = l_it->second.find( p_slotId );

		if ( l_it2 != l_it->second.end() )
		{
			l_return = uint32_t( l_it2->second.size() );
		}
	}

	return l_return;
}

uint32_t ChatDatabase::GetDress( uint32_t p_sex, uint32_t p_slotId, uint32_t p_index )
{
	uint32_t l_return = DEFAULT_DRESS_ID;
	std::clog << "ChatDatabase::GetDress - Sex : " << p_sex << " - Slot " << p_slotId << " - Index " << p_index << std::endl;
	auto && l_it = m_allDresses.find( p_sex );

	if ( l_it != m_allDresses.end() )
	{
		auto && l_it2 = l_it->second.find( p_slotId );

		if ( l_it2 != l_it->second.end() && p_index < l_it2->second.size() )
		{
			l_return = l_it2->second[p_index];
		}
	}

	return l_return;
}

bool ChatDatabase::DoFlushDBFromPlayerInfos( uint32_t p_id )
{
	bool l_return = false;
	// first, we flush the db infos about the player's dresses
	Database::String l_query = Format( c_retrievePlayerDressQuery, MAX_QUERY_LEN, p_id );
	Database::DatabaseResultSPtr l_result = m_db->ExecuteSelect( l_query );

	if ( !l_result )
	{
		std::cerr << "ChatDatabase::DoFlushDBFromPlayerInfos - Can't execute query #1" << std::endl;
		l_return = false;
	}
	else
	{
		uint64_t l_rowCount = l_result->GetRowCount();
		std::clog << "ChatDatabase::DoFlushDBFromPlayerInfos - Query #1 - Nb Rows : " << l_rowCount << std::endl;

		if ( l_rowCount > 0 )
		{
			l_query = Format( c_removePlayerDressMatsQuery, MAX_QUERY_LEN, l_result->GetNextRow()->Get< String >( 0 ).c_str() );

			for ( uint64_t i = 1 ; i < l_rowCount ; i++ )
			{
				l_query += " or ref_player_dress='";
				l_query += l_result->GetNextRow()->Get< String >( 0 );
				l_query += "'";
			}

			m_db->ExecuteUpdate( l_query );
			std::clog << "ChatDatabase::DoFlushDBFromPlayerInfos - Query #2 -> Success" << std::endl;

			l_query = Format( c_removeDressesQuery, MAX_QUERY_LEN, p_id );
			m_db->ExecuteUpdate( l_query );
			std::clog << "ChatDatabase::DoFlushDBFromPlayerInfos - Query #3 -> Success" << std::endl;
		}

		l_query = Format( c_removeTattoosQuery, MAX_QUERY_LEN, p_id );
		m_db->ExecuteUpdate( l_query );
		std::clog << "ChatDatabase::DoFlushDBFromPlayerInfos - Query #4 -> Success" << std::endl;
		l_return = true;
	}

	return l_return;
}

bool ChatDatabase::DoFillDBWithPlayerInfos( Clothes const & p_clothes, uint32_t p_id )
{
	bool l_return = false;
	Database::String l_query = Format( c_updateSexQuery, MAX_QUERY_LEN, ( p_clothes.m_sex == 1 ? "femme" : "homme" ), p_id );
	m_db->ExecuteUpdate( l_query );
	std::clog << "ChatDatabase::DoFillDBWithPlayerInfos - Query #1 -> Success" << std::endl;

	std::clog << "ChatDatabase::DoFillDBWithPlayerInfos - Nb Dresses : " << p_clothes.m_dresses.size() << "" << std::endl;
	auto && l_dressIt = p_clothes.m_dresses.begin();
	l_query = Format( c_insertDressQuery, MAX_QUERY_LEN, p_id, l_dressIt->second.m_id, l_dressIt->second.m_slot );
	l_dressIt++;

	size_t l_length = MAX_QUERY_LEN;

	while ( l_dressIt != p_clothes.m_dresses.end() )
	{
		l_query += ", ('";
		l_query += Database::StringUtils::ToString( p_id );
		l_query += "', '";
		l_query += Database::StringUtils::ToString( l_dressIt->second.m_id );
		l_query += "', '";
		l_query += Database::StringUtils::ToString( l_dressIt->second.m_slot );
		l_query += "')";
		l_dressIt++;
	}

	m_db->ExecuteUpdate( l_query );
	std::clog << "ChatDatabase::DoFillDBWithPlayerInfos - Query #2 -> Success" << std::endl;

	l_query = Format( c_retrievePlayerDressQuery, MAX_QUERY_LEN, p_id );
	Database::DatabaseResultSPtr l_result = m_db->ExecuteSelect( l_query );

	if ( !l_result || !l_result->GetRowCount() )
	{
		std::cerr << "ChatDatabase::DoFillDBWithPlayerInfos - can't execute query #6" << std::endl;
	}
	else
	{
		uint64_t l_rowCount = l_result->GetRowCount();
		std::clog << "ChatDatabase::DoFillDBWithPlayerInfos - Query #3 - Nb Rows : " << l_rowCount << std::endl;
		std::map< uint32_t, uint32_t > l_map;

		for ( uint64_t i = 0 ; i < l_rowCount ; i++ )
		{
			Database::DatabaseRowSPtr l_row = l_result->GetNextRow();
			uint32_t l_dress = l_row->Get< uint32_t >( 1 );
			uint32_t l_pdress = l_row->Get< uint32_t >( 0 );
			l_map.insert( std::make_pair( l_dress, l_pdress ) );
			std::clog << "ChatDatabase::DoFillDBWithPlayerInfos - DressId : " << l_dress << " - PlayerDressId : " << l_pdress << std::endl;
		}

		for ( auto && l_it : p_clothes.m_dresses )
		{
			std::clog << "ChatDatabase::DoFillDBWithPlayerInfos - Dress Id : " << l_it.second.m_id << std::endl;
			auto && l_mapIt = l_map.find( l_it.second.m_id );

			if ( l_mapIt != l_map.end() )
			{
				std::clog << "ChatDatabase::DoFillDBWithPlayerInfos - Found - Nb materials : " << l_it.second.m_materials.size() << std::endl;
				uint32_t l_playerDressId = l_mapIt->second;

				for ( auto && l_matit : l_it.second.m_materials )
				{
					std::clog << "ChatDatabase::DoFillDBWithPlayerInfos - " << l_playerDressId << " - " << l_matit.second << " - " << l_matit.first << std::endl;
					l_query = Format( c_insertDressMatQuery, MAX_QUERY_LEN, l_playerDressId, l_matit.second, l_matit.first );
					m_db->ExecuteUpdate( l_query );
					std::clog << "ChatDatabase::DoFillDBWithPlayerInfos - Query #4 : " << l_query << "-> Success" << std::endl;
				}
			}
		}

		auto && l_tattooIt = p_clothes.m_tattoos.begin();

		if ( l_tattooIt != p_clothes.m_tattoos.end() )
		{
			l_query = Format( c_insertTattooQuery, MAX_QUERY_LEN, p_id, l_tattooIt->second.m_id, l_tattooIt->second.m_slot );
			l_tattooIt++;

			while ( l_tattooIt != p_clothes.m_tattoos.end() )
			{
				std::clog << "ChatDatabase::DoFillDBWithPlayerInfos - Tattoo : ID : " << l_tattooIt->second.m_id << ", Slot : " << l_tattooIt->second.m_slot << std::endl;
				l_query += ", ('";
				l_query += Database::StringUtils::ToString( p_id );
				l_query += "', '";
				l_query += Database::StringUtils::ToString( l_tattooIt->second.m_id );
				l_query += "', '";
				l_query += Database::StringUtils::ToString( l_tattooIt->second.m_slot );
				l_query += "')";
				l_tattooIt++;
			}

			m_db->ExecuteUpdate( l_query );
			std::clog << "ChatDatabase::DoFillDBWithPlayerInfos - Query #5 -> Success" << std::endl;
		}

		l_return = true;
	}

	return l_return;
}
