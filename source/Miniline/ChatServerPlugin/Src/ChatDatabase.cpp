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
	, m_modulePath( p_path )
{

	Database::String l_modulePath( m_modulePath );
	Database::CPluginManager::Instance().SetApplicationPath( l_modulePath );
	Database::CPluginManager::Instance().SetPluginsPath( l_modulePath );
	Database::CPluginManager::Instance().SetTranslationsPath( l_modulePath );
	Database::CPluginManager::Instance().LoadPlugin( l_modulePath + MYSQL_PLUGIN );
	m_manager = Database::CFactoryManager::Instance().CreateInstance( DATABASE_MYSQL_TYPE );

	m_configInfo = new ConfigInfo;
	/*
		m_configInfo->DBName = "kazugame_prod";
		m_configInfo->DBUser = "miniline";
		m_configInfo->DBPass = "b1cr0sS3r";
		m_configInfo->DBHost = "r22764.ovh.net";
	*/
	m_configInfo->DBName = "kazugame_prod";
	m_configInfo->DBUser = "root";
	m_configInfo->DBPass = "";
	m_configInfo->DBHost = "127.0.0.1";
	m_configInfo->DBPort = 3306;
}

ChatDatabase::~ChatDatabase()
{
	delete m_manager;
	Database::String l_modulePath( m_modulePath );
	Database::CPluginManager::Instance().UnloadPlugin( l_modulePath + MYSQL_PLUGIN );
}

bool ChatDatabase::Initialize()
{
	if ( ! m_configInfo->DBName.empty() && ! m_configInfo->DBUser.empty()
			&& m_configInfo->DBPort > 1024 )
	{
		Database::String l_result;
		m_manager->Initialize( m_configInfo->DBHost, m_configInfo->DBName, m_configInfo->DBUser, m_configInfo->DBPass );
		m_manager->CreateConnection( l_result );
		m_db = m_manager->RetrieveConnection();
		return Update();
	}

	return false;
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

		//std::cout << "ChatDatabase::Update - Dress : " << l_dressId << ", Sex : " << l_dressSex << " - Slot : " << l_slotId << ", Sex : " << l_slotSex << "\n";
		if ( l_slotsIt == m_allDresses.end() )
		{
			std::map <int, std::vector <int> > l_map;
			m_allDresses.insert( std::map <int, std::map <int, std::vector <int> > >::value_type( l_dressSex, l_map ) );
			l_slotsIt = m_allDresses.find( l_dressSex );
		}

		l_dressesIt = l_slotsIt->second.find( l_slotId );

		if ( l_slotsIt->second.find( l_slotId ) == l_slotsIt->second.end() )
		{
			std::vector <int> l_array;
			l_slotsIt->second.insert( std::map <int, std::vector <int> >::value_type( l_slotId, l_array ) );
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

//		std::cout << "ChatDatabase::Update - Tattoo : " << l_tattooId << ", Sex : " << l_tattooSex << " - Slot : " << l_slotId << ", Sex : " << l_slotSex << "\n";
		if ( l_slotsIt == m_allTattoos.end() )
		{
			std::map <int, std::vector <int> > l_map;
			m_allTattoos.insert( std::map <int, std::map <int, std::vector <int> > >::value_type( l_tattooSex, l_map ) );
			l_slotsIt = m_allTattoos.find( l_tattooSex );
		}

		l_tattoosIt = l_slotsIt->second.find( l_slotId );

		if ( l_slotsIt->second.find( l_slotId ) == l_slotsIt->second.end() )
		{
			std::vector <int> l_array;
			l_slotsIt->second.insert( std::map <int, std::vector <int> >::value_type( l_slotId, l_array ) );
			l_tattoosIt = l_slotsIt->second.find( l_slotId );
		}

		l_tattoosIt->second.push_back( l_tattooId );
	}

	return true;
}

bool ChatDatabase::ConnectUser( ChatTcpClient * p_client )
{
	if ( p_client != NULL )
	{
		Database::String l_query;
		Format( l_query, MAX_QUERY_LEN,
				c_connectQuery,
				p_client->GetName().c_str(),
				p_client->GetPass().c_str() );

		Database::DatabaseResultPtr l_result = m_db->ExecuteSelect( l_query );

		if ( l_result && l_result->GetRowCount() > 0 )
		{
			int l_id = l_result->GetNextRow()->Get< int >( 0 );
//			std::cout << "Connect User - ID : " << l_id << "\n";
			p_client->SetId( static_cast <unsigned int>( l_id ) );
			return true;
		}
	}

	return false;
}

Clothes * ChatDatabase::LoadDresses( ChatTcpClient * p_client )
{
	if ( p_client != NULL )
	{
		Database::String l_query;
		Format( l_query, MAX_QUERY_LEN,
				c_loadSexQuery,
				p_client->GetId() );

		Database::DatabaseResultPtr l_result1 = m_db->ExecuteSelect( l_query );

		if ( !l_result1 || l_result1->GetRowCount() == 0 )
		{
			std::cout << "ChatDatabase::LoadDresses - can't retrieve sex\n";
			return false;
		}

		Clothes * l_clothes = new Clothes();
		String l_sex = l_result1->GetNextRow()->Get< String >( 0 );
//		std::cout << "ChatDatabase::LoadDresses - Sex : " << l_sex << "\n";

		l_clothes->m_sex = ( l_sex == "homme" ? 0 : 1 );

		{
			// DRESSES
			Format( l_query, MAX_QUERY_LEN,
					c_loadDressesQuery,
					p_client->GetId() );
			Database::DatabaseResultPtr l_result = m_db->ExecuteSelect( l_query );

			if ( !l_result || l_result->GetRowCount() <= 0 )
			{
				std::cout << "ChatDatabase::LoadDresses - can't retrieve dresses\n";
				return l_clothes;
			}

			int l_playerDressId;
			int	l_slotId;
			int l_dressId;
			unsigned long long l_rowCount = l_result->GetRowCount();
			Database::DatabaseRowPtr l_row;
			Database::DatabaseResultPtr l_result2;
			unsigned long long l_rowCount2;
			Dress * l_dress;
			int l_submatId;
			int l_matIndex;

			for ( unsigned long long i = 0 ; i < l_rowCount ; i++ )
			{
				l_row = l_result->GetNextRow();
				l_playerDressId = l_row->Get< int >( 0 );
				l_slotId = l_row->Get< int >( 1 );
				l_dressId = l_row->Get< int >( 2 );
				Format( l_query, MAX_QUERY_LEN,
						c_loadDressMatQuery,
						l_playerDressId );
				l_dress = new Dress( l_dressId, l_slotId );
				l_result2 = m_db->ExecuteSelect( l_query );
				l_rowCount2 = l_result2->GetRowCount();

				for ( unsigned long long j = 0 ; j < l_rowCount2 ; j++ )
				{
					l_row = l_result2->GetNextRow();
					l_submatId = l_row->Get< int >( 1 );
					l_matIndex = l_row->Get< int >( 0 );
					l_dress->m_materials.insert( std::map <int, int>::value_type( l_submatId, l_matIndex ) );
				}

				l_clothes->m_dresses.insert( std::map <int, Dress *>::value_type( l_slotId, l_dress ) );
			}
		} // END DRESSES

		{
			// TATTOOS
			Format( l_query, MAX_QUERY_LEN,
					c_loadTattoosQuery,
					p_client->GetId() );

			Database::DatabaseResultPtr l_result = m_db->ExecuteSelect( l_query );

			if ( !l_result || l_result->GetRowCount() <= 0 )
			{
				std::cout << "ChatDatabase::LoadDresses - can't retrieve tattoos\n";
				return l_clothes;
			}

			int l_playerTattooId;
			int	l_slotId;
			int l_tattooId;
			unsigned long long l_rowCount = l_result->GetRowCount();
			Database::DatabaseRowPtr l_row;
			Tattoo * l_tattoo;

			for ( unsigned long long i = 0 ; i < l_rowCount ; i++ )
			{
				l_row = l_result->GetNextRow();
				l_playerTattooId = l_row->Get< int >( 0 );
				l_slotId = l_row->Get< int >( 1 );
				l_tattooId = l_row->Get< int >( 2 );
				l_tattoo = new Tattoo( l_tattooId, l_slotId );
				l_clothes->m_tattoos.insert( std::map <int, Tattoo *>::value_type( l_slotId, l_tattoo ) );
			}
		} // END TATTOOS

		return l_clothes;
	}

	return NULL;
}

bool ChatDatabase::UpdateClothes( ChatTcpClient * p_client )
{
	if ( p_client )
	{
		return false;
	}

	if ( _flushDBFromPlayerInfos( p_client ) )
	{
		return _fillDBWithPlayerInfos( p_client );
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

bool ChatDatabase::LoadFriends( ChatTcpClient * p_client )
{
	if ( p_client )
	{
		return false;
	}

	Database::String l_query;
	Format( l_query, MAX_QUERY_LEN,
			c_loadFriendsQuery,
			p_client->GetId() );

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
			p_client->AddFriend( l_name, l_id );
		}
	}

	return true;
}

bool ChatDatabase::LoadIgnored( ChatTcpClient * p_client )
{
	if ( p_client == NULL )
	{
		return false;
	}

	Database::String l_query;
	Format( l_query, MAX_QUERY_LEN,
			c_loadIgnoredQuery,
			p_client->GetId() );

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
			p_client->AddIgnored( l_name, l_id );
		}
	}

	return true;
}

bool ChatDatabase::LoadRooms( ChatWorld * p_world )
{
	std::cout << "******************************\nLoadingRooms\n";

	if ( p_world != NULL )
	{
//		std::cout << "ChatDatabase::LoadRooms - " << c_loadRoomsQuery << "\n";
		Database::DatabaseResultPtr l_result = m_db->ExecuteSelect( c_loadRoomsQuery );

		if ( l_result != NULL )
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
				p_world->AddRoom( l_roomName, l_sceneName );
			}

			std::cout << "******************************\n";
			return true;
		}
	}

	std::cout << "ChatDatabase Error\n******************************\n";
	return false;
}

bool ChatDatabase::LoadGames( StrUIntIdMap & p_names )
{
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
				p_names.insert( StrUIntIdMap::value_type( l_gameId, StrUIntPair( l_gameName, l_maxPlayers ) ) );
			}
		}

		std::cout << "******************************\n";
		return true;
	}

	std::cout << "ChatDatabase Error\n******************************\n";
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
//	std::cout << "ChatDatabase::GetDress - Sex : " << p_sex << " - Slot " << p_slotId << " - Index " << p_index << "\n";
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

bool ChatDatabase::_flushDBFromPlayerInfos( ChatTcpClient * p_client )
{
	if ( p_client )
	{
		return false;
	}

	Database::String l_query;
	// first, we flush the db infos about the player's dresses
	Format( l_query, MAX_QUERY_LEN,
			c_retrievePlayerDressQuery,
			p_client->GetId() );
	Database::DatabaseResultPtr l_result = m_db->ExecuteSelect( l_query );

	if ( !l_result )
	{
		std::cout << "ChatDatabase::_flushDBFromPlayerInfos - can't execute query #1\n";
		return false;
	}

	unsigned long long l_rowCount = l_result->GetRowCount();

//	std::cout << "ChatDatabase::_flushDBFromPlayerInfos - Query #1 - Nb Rows : " << l_rowCount << "\n";
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
//		std::cout << "ChatDatabase::_flushDBFromPlayerInfos - Query #2 -> Success\n";

		Format( l_query, MAX_QUERY_LEN,
				c_removeDressesQuery,
				p_client->GetId() );
		m_db->ExecuteUpdate( l_query );
//		std::cout << "ChatDatabase::_flushDBFromPlayerInfos - Query #3 -> Success\n";
	}

	Format( l_query, MAX_QUERY_LEN,
			c_removeTattoosQuery,
			p_client->GetId() );
	m_db->ExecuteUpdate( l_query );
//	std::cout << "ChatDatabase::_flushDBFromPlayerInfos - Query #4 -> Success\n";

	return true;
}

bool ChatDatabase::_fillDBWithPlayerInfos( ChatTcpClient * p_client )
{
	if ( p_client == NULL )
	{
		return false;
	}

	Database::String l_query;
	Clothes * l_clothes = p_client->GetClothes();
	std::map <int, Dress *>::iterator l_dressIt;
	std::map <int, int>::iterator l_matIt;

	Format( l_query, MAX_QUERY_LEN,
			c_updateSexQuery,
			( l_clothes->m_sex == 1 ? "femme" : "homme" ),
			p_client->GetId() );
	m_db->ExecuteUpdate( l_query );
//	std::cout << "ChatDatabase::_fillDBWithPlayerInfos - Query #1 -> Success\n";

//	std::cout << "ChatDatabase::_fillDBWithPlayerInfos - Nb Dresses : " << l_clothes->m_dresses.size() << "\n";
	l_dressIt = l_clothes->m_dresses.begin();
	Format( l_query, MAX_QUERY_LEN,
			c_insertDressQuery,
			p_client->GetId(),
			l_dressIt->second->m_id,
			l_dressIt->second->m_slot );
	l_dressIt++;

	size_t l_length = MAX_QUERY_LEN;

	while ( l_dressIt != l_clothes->m_dresses.end() )
	{
		l_query += ", ('";
		l_query += Database::CStrUtils::ToString( p_client->GetId() );
		l_query += "', '";
		l_query += Database::CStrUtils::ToString( l_dressIt->second->m_id );
		l_query += "', '";
		l_query += Database::CStrUtils::ToString( l_dressIt->second->m_slot );
		l_query += "')";
		l_dressIt++;
	}

	m_db->ExecuteUpdate( l_query );
//	std::cout << "ChatDatabase::_fillDBWithPlayerInfos - Query #2 -> Success\n";

	Format( l_query, MAX_QUERY_LEN,
			c_retrievePlayerDressQuery,
			p_client->GetId() );
	Database::DatabaseResultPtr l_result = m_db->ExecuteSelect( l_query );

	if ( !l_result )
	{
		std::cout << "ChatDatabase::_fillDBWithPlayerInfos - can't execute query #6\n";
		return false;
	}

	unsigned long long l_rowCount = l_result->GetRowCount();

	if ( l_rowCount == 0 )
	{
		std::cout << "ChatDatabase::_fillDBWithPlayerInfos - Query #3 - Fail\n";
		return false;
	}

//	std::cout << "ChatDatabase::_fillDBWithPlayerInfos - Query #3 - Nb Rows : " << l_rowCount << "\n";

	std::map <int, int> l_map;
	Database::DatabaseRowPtr l_row;

	for ( unsigned long long i = 0 ; i < l_rowCount ; i++ )
	{
		l_row = l_result->GetNextRow();
		l_map.insert( std::make_pair( l_row->Get< int >( 1 ), l_row->Get< int >( 0 ) ) );
//		std::cout << "ChatDatabase::_fillDBWithPlayerInfos - DressId : " << l_row->Get< int >( 1)) << " - PlayerDressId : " << l_row->Get< int >( 0)) << "\n";
	}

	std::map <int, int>::iterator l_mapIt;
	int l_playerDressId;

	for ( l_dressIt = l_clothes->m_dresses.begin() ; l_dressIt != l_clothes->m_dresses.end() ; l_dressIt++ )
	{
//		std::cout << "ChatDatabase::_fillDBWithPlayerInfos - Dress Id : " << l_dressIt->second->m_id << "\n";
		l_mapIt = l_map.find( l_dressIt->second->m_id );

		if ( l_mapIt != l_map.end() )
		{
//			std::cout << "ChatDatabase::_fillDBWithPlayerInfos - Found - Nb materials : " << l_dressIt->second->m_materials.size() <<"\n";
			l_playerDressId = l_mapIt->second;

			for ( l_matIt = l_dressIt->second->m_materials.begin() ; l_matIt != l_dressIt->second->m_materials.end() ; l_matIt++ )
			{
//				std::cout << "ChatDatabase::_fillDBWithPlayerInfos - " << l_playerDressId << " - " << l_matIt->second << " - " << l_matIt->first << "\n";
				Format( l_query, MAX_QUERY_LEN,
						c_insertDressMatQuery,
						l_playerDressId,
						l_matIt->second,
						l_matIt->first );
				m_db->ExecuteUpdate( l_query );
//				std::cout << "ChatDatabase::_fillDBWithPlayerInfos - Query #4 : " << l_query << "-> Success\n";
			}
		}
	}

	std::map <int, Tattoo *>::iterator l_tattooIt;
	l_tattooIt = l_clothes->m_tattoos.begin();

	if ( l_tattooIt == l_clothes->m_tattoos.end() )
	{
		return true;
	}

	Format( l_query, MAX_QUERY_LEN,
			c_insertTattooQuery,
			p_client->GetId(),
			l_tattooIt->second->m_id,
			l_tattooIt->second->m_slot );
	l_tattooIt++;

	while ( l_tattooIt != l_clothes->m_tattoos.end() )
	{
//		std::cout << "ChatDatabase::_fillDBWithPlayerInfos - Tattoo : ID : " << l_tattooIt->second->m_id << ", Slot : " << l_tattooIt->second->m_slot << "\n";
		l_query += ", ('";
		l_query += Database::CStrUtils::ToString( p_client->GetId() );
		l_query += "', '";
		l_query += Database::CStrUtils::ToString( l_tattooIt->second->m_id );
		l_query += "', '";
		l_query += Database::CStrUtils::ToString( l_tattooIt->second->m_slot );
		l_query += "')";
		l_tattooIt++;
	}

	m_db->ExecuteUpdate( l_query );
//	std::cout << "ChatDatabase::_fillDBWithPlayerInfos - Query #5 -> Success\n";

	return true;
}
