#include "ChatClient.h"
#include "ChatPlugin.h"
#include "ChatGame.h"
#include "ChatRoom.h"
#include "ChatWorld.h"
#include "Buffer.h"
#include "StringConverter.h"
#include "ChatDatabase.h"

using namespace General::Templates;
using namespace General::Utils;

using namespace Chat;

unsigned int ChatTcpClient::sm_clientsNumber = 0;

ChatTcpClient::ChatTcpClient( ElypseTcpService * p_elypseService,
							  ChatPlugin * p_plugin )
	:	ElypseTcpClient( p_elypseService ),
		m_plugin( p_plugin ),
		m_room( NULL ),
		m_status( ccsNone ),
		m_database( m_plugin->GetDatabase() ),
		m_yaw( 0.0 ),
		m_clothes( NULL ),
		m_id( sm_clientsNumber++ ),
		m_game( NULL ),
		m_toDelete( false ),
		m_working( false ),
		m_anonymous( true ),
		m_sat( false ),
		m_stopped( false )
{
	std::cout << "ChatTcpClient()" << std::endl;
}

ChatTcpClient::~ChatTcpClient()
{
	std::cout << "ChatTcpClient - Deleting " << m_name << std::endl;

	if ( m_status == ccsInRoom && m_room != NULL )
	{
		WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
		l_buffer << msQuit << static_cast <int>( m_name.size() );
		l_buffer.writeArray( m_name.c_str(), m_name.size() );
		m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
		_forwardMessage( m_toSend );
		m_room->RemoveClient( this );
		m_room = NULL;
	}

	if ( m_status == ccsInGame && m_game != NULL )
	{
		std::cout << m_name << " was in a game\n";
		WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
		l_buffer << msQuitGame << static_cast <int>( m_name.size() );
		l_buffer.writeArray( m_name.c_str(), m_name.size() );
		m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
		_forwardGameMessage( m_toSend );
		m_game->RemovePlayer( this );

		if ( m_game->GetInitiatorId() == m_id )
		{
			std::cout << m_name << " was the initiator of the game, ending it\n";
			m_plugin->RemoveGame( m_game->GetGameName(), m_id );
		}
		else if ( m_game->GetNbPlayers() == 0 )
		{
			m_plugin->RemoveGame( m_game->GetGameName(), m_game->GetInitiatorId() );
		}

		m_game = NULL;
	}

	m_status = ccsNone;

	if ( m_plugin != NULL )
	{
		m_plugin->RemoveClient( this );
		m_plugin = NULL;
	}

//	std::cout << "ChatClient::~ChatClient - Before clothes\n";
	if ( m_clothes != NULL )
	{
		delete m_clothes;
		m_clothes = NULL;

	}

//	std::cout << "ChatClient::~ChatClient - After clothes\n";

	m_messages.clear();
//	TcpBaseClient::m_socket.close();
	std::cout << "ChatTcpClient - Client " << m_name << " deleted" << std::endl;
}

struct SocketKiller
{
public:
	ElypseTcpClient * m_instance;

public:
	SocketKiller( ElypseTcpClient * p_instance )
		:	m_instance( p_instance )
	{
	}
	void operator()()const
	{
		delete m_instance;
	}
};

void ChatTcpClient::Stop()
{
	if ( ! m_stopped )
	{
		m_stopped = true;
		m_noSend = true;
		TcpWriterBase::m_socket.close();
		TcpWriterBase::m_service.post( SocketKiller( this ) ); //boost::bind( & ElypseTcpClient::Suicide, this));
		//	m_service->post( GENLIB_THREAD_CLASS_FUNCTOR( this, ElypseTcpClient, Suicide));
	}
}

bool ChatTcpClient::LoadNameAndPass( ReadBuffer & p_buffer )
{
	if ( ! CheckValid( ccsNone ) )
	{
		return false;
	}

	int l_size;
	unsigned int l_usize;

	p_buffer >> l_size;
	l_usize = static_cast <unsigned int>( l_size );
	String l_name;

	char * l_cname = p_buffer.readArray<char>( l_usize );

	if ( l_cname == NULL )
	{
		return false;
	}

	l_name.assign( l_cname, l_usize );
	m_name = l_name;

	if ( m_name != "anonymous" )
	{
		m_anonymous = false;
		p_buffer >> l_size;
		l_usize = static_cast <unsigned int>( l_size );
		String l_pass;

		char * l_cpass = p_buffer.readArray<char>( l_usize );

		if ( l_cpass == NULL )
		{
			return false;
		}

		l_pass.assign( l_cpass, l_usize );
		m_password = l_pass;

//		std::cout << "LoadNameAndPass - Name : [" << m_name << "] - Pass : [" << m_password << "]\n";
	}
	else
	{
		char l_cname2[256];
		size_t l_written = snprintf( l_cname2, 256, "anonymous%i", m_id );
		m_name.assign( l_cname2, l_written );
//		std::cout << "LoadNameAndPass - Name : [" << m_name << "]\n";
	}

	m_working = true;

	if ( m_anonymous || m_database->ConnectUser( this ) )
	{
		if ( ! _endWork() )
		{
			return false;
		}

		return true;
	}

	_endWork();
	return false;
}
/*
bool ChatTcpClient::LoadDresses( Clothes * p_clothes)
{
	std::cout << "ChatTcpClient::LoadDresses\n";
	if ( ! CheckValid( ccsNone))
	{
		return false;
	}
	return true;
}
*/
bool ChatTcpClient::SaveDresses( General::Templates::ReadBuffer & p_buffer )
{
	if ( ! CheckValid( ccsNone ) )
	{
		return false;
	}

	if ( m_clothes != NULL )
	{
		delete m_clothes;
	}

	m_clothes = new Clothes;

	int l_numMats;
	int l_submatId;
	int l_matIndex;
	int l_numDresses;
	int l_slotId;
	int l_dressId;
	int l_tattooId;
	Dress * l_dress;
	Tattoo * l_tattoo;
	p_buffer >> m_clothes->m_sex;
	p_buffer >> l_numDresses;

//	std::cout << "ChatTcpClient::SaveDresses - nb dresses : " << l_numDresses << "\n";
	for ( int i = 0 ; i < l_numDresses ; i++ )
	{
		p_buffer >> l_slotId;
		p_buffer >> l_dressId;
		l_dress = new Dress( l_dressId, l_slotId );
		p_buffer >> l_numMats;

//		std::cout << "ChatTcpClient::SaveDresses - Dress N°" << i << " = "
//				  << l_dressId << " at slot " << l_slotId << " - Nb mats : " << l_numMats;
		for ( int j = 0 ; j < l_numMats ; j++ )
		{
			p_buffer >> l_submatId;
			p_buffer >> l_matIndex;
//			std::cout << " - Material at submat " << l_submatId << ", index : " << l_matIndex;
			l_dress->m_materials.insert( std::map <int, int>::value_type( l_submatId, l_matIndex ) );
		}

		std::cout << "\n";
		m_clothes->m_dresses.insert( std::map <int, Dress *>::value_type( l_slotId, l_dress ) );
	}

	p_buffer >> l_numDresses;

//	std::cout << "ChatTcpClient::SaveDresses - nb tattoos : " << l_numDresses << "\n";
	for ( int i = 0 ; i < l_numDresses ; i++ )
	{
		p_buffer >> l_slotId;
		p_buffer >> l_tattooId;
		l_tattoo = new Tattoo( l_tattooId, l_slotId );
//		std::cout << "ChatTcpClient::SaveDresses - Tattoo N°" << i << " = "
//				  << l_tattooId << " at slot " << l_slotId << "\n";
		m_clothes->m_tattoos.insert( std::map <int, Tattoo *>::value_type( l_slotId, l_tattoo ) );
	}

	m_working = true;
	m_database->UpdateClothes( this );

	if ( ! _endWork() )
	{
		return false;
	}

	return true;
}

const String & ChatTcpClient::GetRoomName()
{
	if ( ! CheckValid( ccsInRoom ) )
	{
		return ChatEmptyString;
	}

	if ( m_room != NULL )
	{
		return m_room->GetName();
	}

	return ChatEmptyString;
}

void ChatTcpClient::SetRoom( ChatRoom * p_room )
{
	if ( ! CheckValid( ccsNone ) )
	{
		return;
	}

	m_room = p_room;
	m_status = ccsInRoom;
}

void ChatTcpClient::RemoveRoom()
{
	if ( ! CheckValid( ccsInRoom ) )
	{
		return;
	}

	m_room = NULL;
	m_status = ccsDressed;
}

void ChatTcpClient::RemoveFriend( ChatTcpClient * p_toRemove )
{
	if ( ! CheckValid( ccsInRoom ) )
	{
		return;
	}

	RemoveFriend( p_toRemove->GetName() );
}

void ChatTcpClient::RemoveIgnored( ChatTcpClient * p_toRemove )
{
	if ( ! CheckValid( ccsInRoom ) )
	{
		return;
	}

	RemoveIgnored( p_toRemove->GetName() );
}

void ChatTcpClient::RemoveFriend( const String & p_toRemove )
{
	if ( ! CheckValid( ccsInRoom ) )
	{
		return;
	}

	ClientIdStrMap::iterator l_it = m_friends.find( p_toRemove );

	if ( l_it == m_friends.end() )
	{
		return;
	}

	m_friends.erase( l_it );
}

void ChatTcpClient::RemoveIgnored( const String & p_toRemove )
{
	if ( ! CheckValid( ccsInRoom ) )
	{
		return;
	}

	ClientIdStrMap::iterator l_it = m_ignored.find( p_toRemove );

	if ( l_it == m_ignored.end() )
	{
		return;
	}

	m_ignored.erase( l_it );

	m_working = true;
	m_database->RemoveIgnored( m_id, l_it->second );

	if ( ! _endWork() )
	{
		return;
	}
}

void ChatTcpClient::AddFriend( const String & p_name, unsigned int p_id )
{
	if ( ! CheckValid( ccsInRoom ) )
	{
		return;
	}

	ClientIdStrMap::iterator l_it = m_friends.find( p_name );

	if ( l_it != m_friends.end() )
	{
		return;
	}

	m_friends.insert( ClientIdStrMap::value_type( p_name, p_id ) );
}

void ChatTcpClient::AddIgnored( const String & p_name, unsigned int p_id )
{
	if ( ! CheckValid( ccsInRoom ) )
	{
		return;
	}

	ClientIdStrMap::iterator l_it = m_ignored.find( p_name );

	if ( l_it != m_ignored.end() )
	{
		return;
	}

	m_ignored.insert( ClientIdStrMap::value_type( p_name, p_id ) );
}

void ChatTcpClient::SendEndGameMessage()
{
	if ( ! CheckValid( ccsInGame ) )
	{
		return;
	}

	WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
	l_buffer << msEndGame;
	m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
	AsyncSend( m_toSend );
	m_game = NULL;
}

void ChatTcpClient::SendStartGameMessage()
{
	if ( ! CheckValid( ccsInGame ) )
	{
		return;
	}

	WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
	l_buffer << msGameStart;
	m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
	AsyncSend( m_toSend );
}

void ChatTcpClient::SendGameAlreadyCreatedMessage()
{
	if ( ! CheckValid( ccsNone ) )
	{
		return;
	}

	WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
	l_buffer << msGameAlreadyCreated;
	m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
	AsyncSend( m_toSend );
}

void ChatTcpClient::SendGameDontExistMessage()
{
	if ( ! CheckValid( ccsNone ) )
	{
		return;
	}

	WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
	l_buffer << msGameDontExist;
	m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
	AsyncSend( m_toSend );
}

void ChatTcpClient::SendCreateGameOK()
{
	if ( ! CheckValid( ccsNone ) )
	{
		return;
	}

	WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
	l_buffer << msCreateGameOK;
	m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
	AsyncSend( m_toSend );
	m_status = ccsInGame;
}

void ChatTcpClient::SendGameJoinMessage( int p_place, const String & p_name, Clothes * p_clothes )
{
	if ( ! CheckValid( ccsNone ) )
	{
		return;
	}

	WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
	l_buffer << msJoinGame;
	l_buffer << static_cast <int>( p_name.size() );
	l_buffer.writeArray<char>( p_name.c_str(), p_name.size() );
	l_buffer << p_place;
	l_buffer << p_clothes->m_sex;
	l_buffer << static_cast <int>( p_clothes->m_dresses.size() );

	std::map <int, Dress *>::iterator l_it;
	std::map <int, int>::iterator l_matit;

	for ( l_it = p_clothes->m_dresses.begin() ; l_it != p_clothes->m_dresses.end() ; l_it++ )
	{
//		std::cout << "Slot : " << l_it->second->m_slot;
//		std::cout << " - Dress : " << l_it->second->m_id;
		l_buffer << l_it->second->m_slot;
		l_buffer << l_it->second->m_id;
		l_buffer << static_cast <int>( l_it->second->m_materials.size() );

		for ( l_matit = l_it->second->m_materials.begin() ; l_matit != l_it->second->m_materials.end() ; l_matit++ )
		{
			l_buffer << l_matit->first;
			l_buffer << l_matit->second;
//			std::cout << " - SubMaterial " << l_matit->first << " : " << l_matit->second;
		}

//		std::cout << "\n";
	}

	std::map <int, Tattoo *>::iterator l_it2;

	for ( l_it2 = p_clothes->m_tattoos.begin() ; l_it2 != p_clothes->m_tattoos.end() ; l_it2++ )
	{
//		std::cout << "Slot : " << l_it2->second->m_slot;
//		std::cout << " - Tattoo : " << l_it2->second->m_id;
		l_buffer << l_it2->second->m_slot;
		l_buffer << l_it2->second->m_id;
//		std::cout << "\n";
	}

	m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
	AsyncSend( m_toSend );
}

void ChatTcpClient::SendGameQuitMessage( const String & p_name )
{
	if ( ! CheckValid( ccsInGame ) )
	{
		return;
	}

	WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
	l_buffer << msQuitGame;
	l_buffer << static_cast <int>( p_name.size() );
	l_buffer.writeArray<char>( p_name.c_str(), p_name.size() );
	m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
	AsyncSend( m_toSend );
	m_status = ccsNone;
}

void ChatTcpClient::SendGameJoinOkMessage( int p_place )
{
	if ( ! CheckValid( ccsNone ) )
	{
		return;
	}

	WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
	l_buffer << msJoinGameOK << p_place;
	m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
	AsyncSend( m_toSend );
	m_status = ccsInGame;
}

size_t ChatTcpClient::GetRandom( size_t p_max )
{
	if ( p_max == 0 )
	{
		return 0;
	}

	size_t l_rand = rand() % p_max;
	return l_rand;
}

bool ChatTcpClient::_endWork()
{
	m_working = false;

	if ( m_toDelete )
	{
		m_elypseService->EraseClient( this );
		return false;
	}

	return true;
}

bool ChatTcpClient::CheckValid( int p_comp )
{
	if ( m_toDelete )
	{
		m_elypseService->EraseClient( this );
		return false;
	}

	return m_status >= p_comp;
}

bool ChatTcpClient::_isIgnored( const String & p_name )
{
	if ( ! CheckValid( ccsInRoom ) )
	{
		return false;
	}

	return ( m_ignored.find( p_name ) != m_ignored.end() );
}

String ChatTcpClient::_buildJoinMessage()
{
	if ( ! CheckValid( ccsNone ) )
	{
		return String();
	}

	WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
	l_buffer << msJoin << static_cast <int>( m_name.size() );
	l_buffer.writeArray<char>( m_name.c_str(), m_name.size() );
	l_buffer << m_clothes->m_sex;
	l_buffer << static_cast <int>( m_clothes->m_dresses.size() );

	std::map <int, Dress *>::iterator l_it;
	std::map <int, int>::iterator l_matit;

	for ( l_it = m_clothes->m_dresses.begin() ; l_it != m_clothes->m_dresses.end() ; l_it++ )
	{
//		std::cout << "Slot : " << l_it->second->m_slot;
//		std::cout << " - Dress : " << l_it->second->m_id;
		l_buffer << l_it->second->m_slot;
		l_buffer << l_it->second->m_id;
		l_buffer << static_cast <int>( l_it->second->m_materials.size() );

		for ( l_matit = l_it->second->m_materials.begin() ; l_matit != l_it->second->m_materials.end() ; l_matit++ )
		{
			l_buffer << l_matit->first;
			l_buffer << l_matit->second;
//			std::cout << " - SubMaterial " << l_matit->first << " : " << l_matit->second;
		}

//		std::cout << "\n";
	}

	std::map <int, Tattoo *>::iterator l_it2;

	for ( l_it2 = m_clothes->m_tattoos.begin() ; l_it2 != m_clothes->m_tattoos.end() ; l_it2++ )
	{
//		std::cout << "Slot : " << l_it2->second->m_slot;
//		std::cout << " - Tattoo : " << l_it2->second->m_id;
		l_buffer << l_it2->second->m_slot;
		l_buffer << l_it2->second->m_id;
//		std::cout << "\n";
	}

	l_buffer << m_position.x << m_position.y << m_position.z;

	if ( m_sat )
	{
		l_buffer << 1;
	}
	else
	{
		l_buffer << 0;
	}

	String l_result;
	l_result.assign( l_buffer.c_str(), l_buffer.size() );
	return l_result;
}

void ChatTcpClient::_sendDressesMessage()
{
//	std::cout << "ChatTcpClient::_sendDressesMessage - " << m_name << " sends his dresses\n";
	if ( ! CheckValid( ccsNone ) || m_clothes == NULL )
	{
		std::cout << "ChatTcpClient::_sendDressesMessage - nothing to send\n";
		return;
	}

	WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
	l_buffer << msAvatar;
	l_buffer << m_clothes->m_sex;
	l_buffer << static_cast <int>( m_clothes->m_dresses.size() );

	std::map <int, Dress *>::iterator l_it;
	std::map <int, int>::iterator l_matit;

	for ( l_it = m_clothes->m_dresses.begin() ; l_it != m_clothes->m_dresses.end() ; l_it++ )
	{
//		std::cout << "Slot : " << l_it->second->m_slot;
//		std::cout << " - Dress : " << l_it->second->m_id;
		l_buffer << l_it->second->m_slot;
		l_buffer << l_it->second->m_id;
		l_buffer << static_cast <int>( l_it->second->m_materials.size() );

		for ( l_matit = l_it->second->m_materials.begin() ; l_matit != l_it->second->m_materials.end() ; l_matit++ )
		{
			l_buffer << l_matit->first;
			l_buffer << l_matit->second;
//			std::cout << " - SubMaterial " << l_matit->first << " : " << l_matit->second;
		}

//		std::cout << "\n";
	}

	l_buffer << static_cast <int>( m_clothes->m_tattoos.size() );
	std::map <int, Tattoo *>::iterator l_it2;

	for ( l_it2 = m_clothes->m_tattoos.begin() ; l_it2 != m_clothes->m_tattoos.end() ; l_it2++ )
	{
//		std::cout << "Slot : " << l_it2->second->m_slot;
//		std::cout << " - Tattoo : " << l_it2->second->m_id;
		l_buffer << l_it2->second->m_slot;
		l_buffer << l_it2->second->m_id;
//		std::cout << "\n";
	}

	m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
	AsyncSend( m_toSend );

//	std::cout << "ChatTcpClient::_sendDressesMessage - " << m_name << " has ended the sending of his dresses : " << l_buffer.size() << "\n";

//	AsyncSend( "Coin et pis Glop !!!!");
}

void ChatTcpClient::_sendRoomsMessage()
{
	if ( ! CheckValid( ccsNone ) )
	{
		return;
	}

	WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
	l_buffer << msRooms;
	ChatRoom * l_room;

	ChatRoomStrMap l_rooms = m_plugin->GetWorld()->GetRooms();
//	std::cout << "_sendRoomsMessage - " << l_rooms.size() << "\n";
	l_buffer << static_cast <int>( l_rooms.size() );
	ChatRoomStrMap::iterator l_it;

	for ( l_it = l_rooms.begin() ; l_it != l_rooms.end() ; ++l_it )
	{
		l_room = l_it->second;

		if ( l_room != NULL )
		{
//			std::cout << l_room->GetName() << " - " << l_room->GetSceneName()
//					  << " - " << l_room->GetNbPeople() << "\n";
			l_buffer << static_cast <int>( l_room->GetName().size() );
			l_buffer.writeArray<char>( l_room->GetName().c_str(), l_room->GetName().size() );
			l_buffer << static_cast <int>( l_room->GetSceneName().size() );
			l_buffer.writeArray<char>( l_room->GetSceneName().c_str(), l_room->GetSceneName().size() );
			l_buffer << static_cast <int>( l_room->GetNbPeople() );
		}
	}

	m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
	AsyncSend( m_toSend );
}

void ChatTcpClient::_sendKickMessage()
{
	if ( ! CheckValid( ccsNone ) )
	{
		return;
	}

	WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
	l_buffer << msKickUser;
	m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
	AsyncSend( m_toSend );
}

void ChatTcpClient::_sendGameJoinFail()
{
	if ( ! CheckValid( ccsNone ) )
	{
		return;
	}

	WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
	l_buffer << msJoinGameFail;
	m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
	AsyncSend( m_toSend );
}

void ChatTcpClient::_forwardMessage( const String & p_message )
{
	if ( ! CheckValid( ccsInRoom ) || m_room == NULL )
	{
		return;
	}

	ChatTcpClientStrMap l_clients = m_room->GetClients();
	ChatTcpClientStrMap::iterator l_it;
	ChatTcpClientStrMap::iterator l_myIt = l_clients.find( m_name );

	for ( l_it = l_clients.begin() ; l_it != l_clients.end() ; ++l_it )
	{
		if ( l_it != l_myIt && l_it->second != NULL
				&& ! l_it->second->IsToDelete() )
		{
//			std::cout << "forwarding message to ";
//			std::cout << l_it->second->GetName() << "\n";
			l_it->second->AsyncSend( p_message );
		}
	}
}

void ChatTcpClient::_forwardGameMessage( const String & p_message )
{
	if ( ! CheckValid( ccsInGame ) || m_game == NULL )
	{
		return;
	}

	ClientIdChatCLientMap l_players = m_game->GetPlayers();
	ClientIdChatCLientMap::iterator l_myIt = l_players.find( m_id );
	ClientIdChatCLientMap::iterator l_it;

	for ( l_it = l_players.begin() ; l_it != l_players.end() ; ++l_it )
	{
		if ( l_it != l_myIt && l_it->second != NULL
				&& ! l_it->second->IsToDelete() )
		{
			l_it->second->AsyncSend( p_message );
		}
	}
}

void ChatTcpClient::_forwardTalkMessage( const String & p_message )
{
	if ( ! CheckValid( ccsInRoom ) || m_room == NULL )
	{
		return;
	}

	ChatTcpClientStrMap l_clients = m_room->GetClients();
	ChatTcpClientStrMap::iterator l_it;
	ChatTcpClientStrMap::iterator l_myIt = l_clients.find( m_name );

	for ( l_it = l_clients.begin() ; l_it != l_clients.end() ; ++l_it )
	{
		if ( l_it != l_myIt && l_it->second != NULL
				&& ! l_it->second->IsToDelete()
				&& m_ignored.find( l_it->second->GetName() ) == m_ignored.end() )
		{
			l_it->second->AsyncSend( p_message );
		}
	}
}

void ChatTcpClient::_processConnectMessage( ReadBuffer & p_buffer )
{
	if ( ! CheckValid( ccsNone ) )
	{
		return;
	}

	if ( LoadNameAndPass( p_buffer ) )
	{
		if ( ! CheckValid( ccsNone ) )
		{
			return;
		}

		if ( ! m_anonymous )
		{
			ChatTcpClient * l_client = m_plugin->GetClient( m_name );

			if ( l_client != NULL )
			{
				std::cout << "ChatTcpClient::_processConnectMessage - " << m_name << " was already connected\n";
				l_client->_sendKickMessage();
				m_plugin->RemoveClient( l_client );
			}

			m_plugin->AddClient( this );
			m_working = true;

			if ( m_clothes != NULL )
			{
				delete m_clothes;
				m_clothes = NULL;
			}

			m_clothes = m_database->LoadDresses( this );

			if ( m_clothes == NULL )
			{
				std::cout << "ChatTcpClient::_processConnectMessage - Can't load dresses for " << m_name << "\n";
				return;
			}

			m_status = ccsDressed;

			if ( ! _endWork() )
			{
				std::cout << "ChatTcpClient::_processConnectMessage - " << m_name << " has not ended his work\n";
				return;
			}

			m_working = true;
			m_database->LoadFriends( this );

			if ( ! _endWork() )
			{
				std::cout << "ChatTcpClient::_processConnectMessage - " << m_name << " has not ended his work\n";
				return;
			}

			m_working = true;
			m_database->LoadIgnored( this );

			if ( ! _endWork() )
			{
				std::cout << "ChatTcpClient::_processConnectMessage - " << m_name << " has not ended his work\n";
				return;
			}
		}
		else
		{
//			std::cout << "ChatClient::_processConnectMessage - Anonymous\n";
			m_plugin->AddClient( this );
//			std::cout << "ChatClient::_processConnectMessage - Added\n";
			std::map <int, Dress *>::iterator l_it;
			size_t l_size;
			size_t l_sex = GetRandom( 2 );
			m_clothes = new Clothes();
			m_clothes->m_sex = l_sex;
			Dress * l_dress;

			for ( int i = 1 ; i <= m_database->GetNumDressSlots() ; i++ )
			{
				l_dress = new Dress( 137, i );
				m_clothes->m_dresses.insert( std::map <int, Dress *>::value_type( i, l_dress ) );
			}

			for ( l_it = m_clothes->m_dresses.begin() ; l_it != m_clothes->m_dresses.end() ; l_it++ )
			{
				l_size = m_database->GetDressesSize( l_sex, l_it->first );

				if ( l_size > 10 )
				{
					l_size = 10;
				}

				l_dress = l_it->second;
				l_dress->m_id = m_database->GetDress( l_sex, l_it->first, GetRandom( l_size ) );

				if ( l_it->first == 11 || l_it->first == 13 )
				{
					l_dress->m_materials.insert( std::map <int, int>::value_type( 0, GetRandom( 6 ) ) );
				}
			}
		}

		std::cout << "ChatClient " << m_name << " connected\n";
		WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
		l_buffer << msConnectOK;
		m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
		AsyncSend( m_toSend );
		m_status = ccsConnected;
		return;
	}

	if ( ! CheckValid( ccsNone ) )
	{
		return;
	}

//	std::cout << "ChatTcpClient::_processConnectMessage - Not connected\n";

	WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
	l_buffer << msConnectFail;
	m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
	AsyncSend( m_toSend );

}

void ChatTcpClient::_processDressMessage( ReadBuffer & p_buffer )
{
	if ( ! CheckValid( ccsConnected ) )
	{
		return;
	}

	if ( ! m_anonymous )
	{
//		std::cout << "ChatTcpClient::_processDressMessage - not anonymous\n";
		SaveDresses( p_buffer );
	}
	else
	{
//		std::cout << "ChatTcpClient::_processDressMessage - anonymous\n";
	}

	if ( m_status < ccsInRoom )
	{
		_sendRoomsMessage();
	}

	m_status = ccsDressed;
}

void ChatTcpClient::_processJoinMessage( ReadBuffer & p_buffer )
{
	if ( ! CheckValid( ccsDressed ) )
	{
		return;
	}

	int l_size;
	p_buffer >> l_size;
	String l_roomName;
	l_roomName.assign( p_buffer.readArray<char>( l_size ), l_size );

//	std::cout << "_processJoinMessage - " << l_roomName << "\n";
	ChatRoom * l_room = m_plugin->GetRoom( l_roomName );
	String l_joinMessage = _buildJoinMessage();

	if ( l_room != NULL )
	{
		ChatTcpClientStrMap l_clients = l_room->GetClients();
		ChatTcpClientStrMap::iterator l_it;

		for ( l_it = l_clients.begin() ; l_it != l_clients.end() ; ++l_it )
		{
			AsyncSend( l_it->second->_buildJoinMessage() );
			l_it->second->AsyncSend( l_joinMessage );
		}

		l_room->AddClient( this );
		m_status = ccsInRoom;
	}
	else
	{
		std::cout << "NULL Room\n";
	}
}

void ChatTcpClient::_processQuitMessage()
{
	if ( ! CheckValid( ccsInRoom ) || m_room == NULL )
	{
		return;
	}

//	std::cout << "Sending quit to room clients\n";
	WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
	l_buffer << msQuit << static_cast <int>( m_name.size() );
	l_buffer.writeArray( m_name.c_str(), m_name.size() );
	m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
	_forwardMessage( m_toSend );
	m_room->RemoveClient( this );
}

void ChatTcpClient::_processUpdateMessage( ReadBuffer & p_buffer )
{
	if ( ! CheckValid( ccsInRoom ) )
	{
		return;
	}

	p_buffer >> m_position.x >> m_position.y >> m_position.z >> m_yaw;

	WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
	l_buffer << msUpdate << static_cast <int>( m_name.size() );
	l_buffer.writeArray<char>( m_name.c_str(), m_name.size() );
	l_buffer << m_position.x << m_position.y << m_position.z << m_yaw;
	m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
	_forwardMessage( m_toSend );
}

void ChatTcpClient::_processWalkMessage( ReadBuffer & p_buffer )
{
	if ( ! CheckValid( ccsInRoom ) )
	{
		return;
	}

	int l_running;
	int l_backward;
	p_buffer >> m_position.x >> m_position.y >> m_position.z >> l_backward >> l_running;

	WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
	l_buffer << msWalk << static_cast <int>( m_name.size() );
	l_buffer.writeArray<char>( m_name.c_str(), m_name.size() );
	l_buffer << m_position.x << m_position.y << m_position.z << l_backward << l_running;

	m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
	_forwardMessage( m_toSend );
}

void ChatTcpClient::_processTurnMessage( ReadBuffer & p_buffer )
{
	if ( ! CheckValid( ccsInRoom ) )
	{
		return;
	}

	p_buffer >> m_yaw;

	WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
	l_buffer << msTurn << static_cast <int>( m_name.size() );
	l_buffer.writeArray<char>( m_name.c_str(), m_name.size() );
	l_buffer << m_yaw;

	m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
	_forwardMessage( m_toSend );
}

void ChatTcpClient::_processTalkMessage( ReadBuffer & p_buffer )
{
	if ( ! CheckValid( ccsInRoom ) )
	{
		return;
	}

	int l_size;
	p_buffer >> l_size;
//	std::cout << l_size << "\n";
	String l_message;
	l_message.assign( p_buffer.readArray<char>( l_size ), l_size );
	std::cout << m_name << " : " << l_message << "\n";

	WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
	l_buffer << msTalk << static_cast <int>( m_name.size() );
	l_buffer.writeArray<char>( m_name.c_str(), m_name.size() );
	l_buffer << l_size;
	l_buffer.writeArray<char>( l_message.c_str(), l_size );

	m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
	_forwardTalkMessage( m_toSend );
}

void ChatTcpClient::_processWhispMessage( ReadBuffer & p_buffer )
{
	if ( ! CheckValid( ccsInRoom ) )
	{
		return;
	}

	if ( m_anonymous )
	{
		WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
		l_buffer << msUnavailableToAnonymous;
		m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
		AsyncSend( m_toSend );
		return;
	}

	int l_size;
	p_buffer >> l_size;
	String l_message;
	l_message.assign( p_buffer.readArray<char>( l_size ), l_size );
	String l_distantName;
	size_t l_index = l_message.find_first_of( " " );
	l_distantName = l_message.substr( 0, l_index );
	l_message = l_message.substr( l_index + 1 );
	std::cout << "_processWhispMessage - " << l_distantName << " - " << l_message << "\n";

	WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
	l_buffer << msWhisp << static_cast <int>( m_name.size() );
	l_buffer.writeArray<char>( m_name.c_str(), m_name.size() );
	l_buffer << l_size;
	l_buffer.writeArray<char>( l_message.c_str(), l_message.size() );

	ChatRoomStrMap l_rooms = m_plugin->GetWorld()->GetRooms();
	ChatRoom * l_room;
	ChatTcpClientStrMap l_clients;
	bool l_found = false;
	ChatTcpClientStrMap::iterator l_it;
	ChatRoomStrMap::iterator l_rIt;
	ChatTcpClient * l_client;
	l_rIt = l_rooms.begin();
	String l_toSend;

	while ( l_rIt != l_rooms.end() && ! l_found )
	{
		l_room = l_rIt->second;
		l_clients = l_room->GetClients();
		l_it = l_clients.begin();

		while ( l_it != l_clients.end() && ! l_found )
		{
			l_client = l_it->second;

			if ( l_client->GetName() == l_distantName &&
					l_client->GetName() != m_name )
			{
				l_found = true;
				l_toSend.assign( l_buffer.c_str(), l_buffer.size() );

				if ( ! l_client->_isIgnored( m_name ) )
				{
					l_client->AsyncSend( l_toSend );
				}

				l_buffer = WriteBuffer( m_messageBuffer, MAX_MSG_LENGTH );
				l_buffer << msWhispOK;
				m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
				AsyncSend( m_toSend );
			}

			++l_it;
		}

		++l_rIt;
	}

	if ( ! l_found )
	{
		std::cout << "WhispFail\n";
		l_buffer = WriteBuffer( m_messageBuffer, MAX_MSG_LENGTH );
		l_buffer << msWhispFail;
		m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
		AsyncSend( m_toSend );
	}
}

void ChatTcpClient::_processBeginWalkMessage()
{
	if ( ! CheckValid( ccsInRoom ) )
	{
		return;
	}

	m_sat = false;
	WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
	l_buffer << msBeginWalk << static_cast <int>( m_name.size() );
	l_buffer.writeArray<char>( m_name.c_str(), m_name.size() );

	m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
	_forwardMessage( m_toSend );
}

void ChatTcpClient::_processEndWalkMessage( ReadBuffer & p_buffer )
{
	if ( ! CheckValid( ccsInRoom ) )
	{
		return;
	}

	float l_x, l_y, l_z, l_yaw;
	p_buffer >> l_x >> l_y >> l_z >> l_yaw;

	WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
	l_buffer << msEndWalk << static_cast <int>( m_name.size() );
	l_buffer.writeArray<char>( m_name.c_str(), m_name.size() );
	l_buffer << l_x << l_y << l_z << l_yaw;

	m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
	_forwardMessage( m_toSend );
}

void ChatTcpClient::_processBeginRunMessage()
{
	if ( ! CheckValid( ccsInRoom ) )
	{
		return;
	}

	m_sat = false;
	WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
	l_buffer << msBeginRun << static_cast <int>( m_name.size() );
	l_buffer.writeArray<char>( m_name.c_str(), m_name.size() );

	m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
	_forwardMessage( m_toSend );
}

void ChatTcpClient::_processEndRunMessage( General::Templates::ReadBuffer & p_buffer )
{
	if ( ! CheckValid( ccsInRoom ) )
	{
		return;
	}

	float l_x, l_y, l_z, l_yaw;
	int l_stillWalking;
	p_buffer >> l_stillWalking >> l_x >> l_y >> l_z >> l_yaw;

	WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
	l_buffer << msEndRun << l_stillWalking << static_cast <int>( m_name.size() );
	l_buffer.writeArray<char>( m_name.c_str(), m_name.size() );
	l_buffer << l_x << l_y << l_z << l_yaw;

	m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
	_forwardMessage( m_toSend );
}

void ChatTcpClient::_processEmoteMessage( ReadBuffer & p_buffer )
{
	if ( ! CheckValid( ccsInRoom ) )
	{
		return;
	}

	int l_size;
	p_buffer >> l_size;
	String l_message;
	l_message.assign( p_buffer.readArray<char>( l_size ), l_size );
	std::cout << "_processEmoteMessage - " << l_message << "\n";

	WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
	l_buffer << msEmote << static_cast <int>( m_name.size() );
	l_buffer.writeArray<char>( m_name.c_str(), m_name.size() );
	l_buffer << l_size;
	l_buffer.writeArray<char>( l_message.c_str(), l_size );

	m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
	_forwardMessage( m_toSend );
}

void ChatTcpClient::_processRemoveFriendMessage( ReadBuffer & p_buffer )
{
	if ( ! CheckValid( ccsInRoom ) )
	{
		return;
	}

	if ( m_anonymous )
	{
		WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
		l_buffer << msUnavailableToAnonymous;
		m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
		AsyncSend( m_toSend );
		return;
	}

	int l_size;
	p_buffer >> l_size;
	String l_toRemove;
	l_toRemove.assign( p_buffer.readArray<char>( l_size ), l_size );

	m_working = true;
	unsigned int l_friendId = m_database->GetClientId( l_toRemove );

	if ( ! _endWork() )
	{
		return;
	}

	if ( l_friendId != 0 )
	{
		m_working = true;
		m_database->RemoveFriend( m_id, l_friendId );

		if ( ! _endWork() )
		{
			return;
		}

		m_database->RemoveFriend( l_friendId, m_id );

		if ( ! _endWork() )
		{
			return;
		}

		ChatTcpClient * l_friend = m_plugin->GetClient( l_toRemove );

		if ( l_friend != NULL )
		{
			l_friend->RemoveFriend( this );
		}

		ClientIdStrMap::iterator l_it = m_friends.find( l_toRemove );

		if ( l_it != m_friends.end() )
		{
			m_friends.erase( l_it );
		}
	}
}

void ChatTcpClient::_processRemoveIgnoredMessage( ReadBuffer & p_buffer )
{
	if ( ! CheckValid( ccsInRoom ) )
	{
		return;
	}

	if ( m_anonymous )
	{
		WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
		l_buffer << msUnavailableToAnonymous;
		m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
		AsyncSend( m_toSend );
		return;
	}

	int l_size;
	p_buffer >> l_size;
	String l_toRemove;
	l_toRemove.assign( p_buffer.readArray<char>( l_size ), l_size );

	RemoveIgnored( l_toRemove );
}

void ChatTcpClient::_processNewFriendMessage( ReadBuffer & p_buffer )
{
	if ( ! CheckValid( ccsInRoom ) )
	{
		return;
	}

	if ( m_anonymous )
	{
		WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
		l_buffer << msUnavailableToAnonymous;
		m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
		AsyncSend( m_toSend );
		return;
	}

	int l_size;
	p_buffer >> l_size;
	String l_name;
	l_name.assign( p_buffer.readArray<char>( l_size ), l_size );
//	std::cout << "_processNewFriendMessage - " << l_name << "\n";

	if ( l_name != m_name )
	{
		ChatTcpClient * l_newFriend = m_plugin->GetClient( l_name );
		WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );

		if ( l_newFriend == NULL )
		{
			std::cout << "_processNewFriendMessage - Friend not found\n";
			l_buffer << msNewFriendDoesntExist << l_size;
			l_buffer.writeArray<char>( l_name.c_str(), l_size );
			m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
			AsyncSend( m_toSend );
		}
		else if ( m_friends.find( l_name ) != m_friends.end() )
		{
			std::cout << "_processNewFriendMessage - Friend found, already in friends\n";
			l_buffer << msNewFriendAlreadyFriend << l_size;
			l_buffer.writeArray<char>( l_name.c_str(), l_size );
			m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
			AsyncSend( m_toSend );
		}
		else
		{
//			std::cout << "_processNewFriendMessage - Friend found\n";
			l_buffer << msNewFriendAsk << static_cast <int>( m_name.size() );
			l_buffer.writeArray<char>( m_name.c_str(), m_name.size() );
			m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
			l_newFriend->AsyncSend( m_toSend );
		}
	}
}

void ChatTcpClient::_processNewIgnoredMessage( ReadBuffer & p_buffer )
{
	if ( ! CheckValid( ccsInRoom ) )
	{
		return;
	}

	if ( m_anonymous )
	{
		WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
		l_buffer << msUnavailableToAnonymous;
		m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
		AsyncSend( m_toSend );
		return;
	}

	int l_size;
	p_buffer >> l_size;
	String l_name;
	l_name.assign( p_buffer.readArray<char>( l_size ), l_size );

	if ( l_name != m_name )
	{
		m_working = true;
		unsigned int l_ignoredId = m_database->GetClientId( l_name );

		if ( ! _endWork() )
		{
			return;
		}

		WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );

		if ( l_ignoredId == 0 )
		{
			l_buffer << msNewIgnoredDoesntExist << l_size;
			l_buffer.writeArray<char>( l_name.c_str(), l_size );
			m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
			AsyncSend( m_toSend );
		}
		else if ( m_ignored.find( l_name ) != m_ignored.end() )
		{
			l_buffer << msNewIgnoredAlreadyIgnored << l_size;
			l_buffer.writeArray<char>( l_name.c_str(), l_size );
			m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
			AsyncSend( m_toSend );
		}
		else
		{
			m_ignored.insert( ClientIdStrMap::value_type( l_name, l_ignoredId ) );
			m_working = true;
			m_database->AddIgnored( m_id, l_ignoredId );

			if ( ! _endWork() )
			{
				return;
			}

			l_buffer << msNewIgnoredDone << l_size;
			l_buffer.writeArray<char>( l_name.c_str(), l_size );
			m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
			AsyncSend( m_toSend );
		}
	}
}

void ChatTcpClient::_processUpdateFriendsMessage()
{
	if ( ! CheckValid( ccsInRoom ) )
	{
		return;
	}

	if ( m_anonymous )
	{
		WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
		l_buffer << msUnavailableToAnonymous;
		m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
		AsyncSend( m_toSend );
		return;
	}

	ChatTcpClient * l_friend;
	ClientIdStrMap::iterator l_it;
	String l_noRoom = "Non connecte";

	WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
	l_buffer << msUpdateFriendsList;
	l_buffer << static_cast <int>( m_friends.size() );

	for ( l_it = m_friends.begin() ; l_it != m_friends.end() ; ++l_it )
	{
		l_friend = m_plugin->GetClient( l_it->first );
		l_buffer << static_cast <int>( l_it->first.size() );
		l_buffer.writeArray<char>( l_it->first.c_str(), l_it->first.size() );

		if ( l_friend != NULL && l_friend->GetStatus() == ccsInRoom )
		{
			l_buffer << static_cast <int>( l_friend->GetRoomName().size() );
			l_buffer.writeArray<char>( l_friend->GetRoomName().c_str(), l_friend->GetRoomName().size() );
		}
		else
		{
			l_buffer << static_cast <int>( l_noRoom.size() );
			l_buffer.writeArray<char>( l_noRoom.c_str(), l_noRoom.size() );
		}
	}

	m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
	AsyncSend( m_toSend );
}

void ChatTcpClient::_processUpdateIgnoredMessage()
{
	if ( ! CheckValid( ccsInRoom ) )
	{
		return;
	}

	if ( m_anonymous )
	{
		WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
		l_buffer << msUnavailableToAnonymous;
		m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
		AsyncSend( m_toSend );
		return;
	}

	ChatTcpClient * l_ignored;
	ClientIdStrMap::iterator l_it;

	WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
	l_buffer << msUpdateIgnoredList;
	l_buffer << static_cast <int>( m_ignored.size() );

	for ( l_it = m_ignored.begin() ; l_it != m_ignored.end() ; ++l_it )
	{
		l_ignored = m_plugin->GetClient( l_it->first );
		l_buffer << static_cast <int>( l_it->first.size() );
		l_buffer.writeArray<char>( l_it->first.c_str(), l_it->first.size() );

		if ( l_ignored != NULL && l_ignored->GetStatus() == ccsInRoom )
		{
			l_buffer << '1';
		}
		else
		{
			l_buffer << '0';
		}
	}

	m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
	AsyncSend( m_toSend );
}

void ChatTcpClient::_processFriendRefuseMessage( ReadBuffer & p_buffer )
{
	if ( ! CheckValid( ccsInRoom ) )
	{
		return;
	}

	if ( m_anonymous )
	{
		WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
		l_buffer << msUnavailableToAnonymous;
		m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
		AsyncSend( m_toSend );
		return;
	}

	int l_size;
	p_buffer >> l_size;
	String l_refusedName;
	l_refusedName.assign( p_buffer.readArray<char>( l_size ), l_size );

	ChatTcpClient * l_refused = m_plugin->GetClient( l_refusedName );

	if ( l_refused != NULL )
	{
		WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
		l_buffer << msNewFriendRefuse << static_cast <int>( m_name.size() );
		l_buffer.writeArray<char>( m_name.c_str(), m_name.size() );

		m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
		l_refused->AsyncSend( m_toSend );
	}
}

void ChatTcpClient::_processFriendAcceptMessage( ReadBuffer & p_buffer )
{
	if ( ! CheckValid( ccsInRoom ) )
	{
		return;
	}

	if ( m_anonymous )
	{
		WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
		l_buffer << msUnavailableToAnonymous;
		m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
		AsyncSend( m_toSend );
		return;
	}

	int l_size;
	p_buffer >> l_size;
	String l_acceptedName;
	l_acceptedName.assign( p_buffer.readArray<char>( l_size ), l_size );

	ChatTcpClient * l_accepted = m_plugin->GetClient( l_acceptedName );

	if ( l_accepted != NULL )
	{
		WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
		l_buffer << msNewFriendAccept << static_cast <int>( m_name.size() );
		l_buffer.writeArray<char>( m_name.c_str(), m_name.size() );

		l_accepted->m_friends.insert( ClientIdStrMap::value_type( m_name, m_id ) );
		m_friends.insert( ClientIdStrMap::value_type( l_acceptedName, l_accepted->GetId() ) );

		m_working = true;
		m_database->AddFriend( l_accepted->GetId(), m_id );

		if ( ! _endWork() )
		{
			return;
		}

		m_database->AddFriend( m_id, l_accepted->GetId() );

		if ( ! _endWork() )
		{
			return;
		}

		m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
		l_accepted->AsyncSend( m_toSend );
	}
}

void ChatTcpClient::_processRefreshGamesListMessage( ReadBuffer & p_buffer )
{
	if ( ! CheckValid( ccsConnected ) )
	{
		return;
	}

	int l_size = 0;
	p_buffer >> l_size;
	String l_name;
	l_name.assign( p_buffer.readArray<char>( l_size ), l_size );
	StrUIntIdMap l_map = m_plugin->GetGamesList( l_name );
	WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
	l_buffer << msRefreshGamesList;
	l_buffer << static_cast <int>( l_map.size() );

	for ( StrUIntIdMap::iterator l_it = l_map.begin() ; l_it != l_map.end() ; ++l_it )
	{
		l_buffer << l_it->first;//id creator
		l_buffer << l_it->second.second;//nb players
		l_buffer << static_cast <int>( l_it->second.first.size() );
		l_buffer.writeArray<char>( l_it->second.first.c_str(), l_it->second.first.size() ); //creator name
	}

	m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
	AsyncSend( m_toSend );
}

void ChatTcpClient::_processJoinGameMessage( ReadBuffer & p_buffer )
{
	if ( ! CheckValid( ccsNone ) )
	{
		return;
	}

	if ( m_anonymous )
	{
		WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
		l_buffer << msUnavailableToAnonymous;
		m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
		AsyncSend( m_toSend );
		return;
	}

	if ( m_status != ccsConnected )
	{
		_sendGameJoinFail();
		return;
	}

	int l_size = 0;
	p_buffer >> l_size;
	String l_name;
	l_name.assign( p_buffer.readArray<char>( l_size ), l_size );
	int l_id;
	p_buffer >> l_id;
	ChatGame * l_game = m_plugin->GetGame( l_name, l_id );

	if ( l_game == NULL ||  ! l_game->AddPlayer( this ) )
	{
		_sendGameJoinFail();
		return;
	}
	else
	{
		int l_place = l_game->GetPlayerPlace( m_name );
		SendGameJoinOkMessage( l_place );
		ClientIdChatCLientMap l_players = l_game->GetPlayers();
		ClientIdChatCLientMap::iterator l_myIt = l_players.find( m_id );

		for ( ClientIdChatCLientMap::iterator l_it = l_players.begin() ; l_it != l_players.end() ; ++l_it )
		{
			if ( l_it != l_myIt && ! l_it->second->IsToDelete() )
			{
				l_it->second->SendGameJoinMessage( l_place, m_name, m_clothes );
				SendGameJoinMessage( l_game->GetPlayerPlace( l_it->second->GetName() ),
									 l_it->second->GetName(),
									 l_it->second->GetClothes() );
			}
		}
	}

	m_game = l_game;
	m_status = ccsInGame;
	return;
}

void ChatTcpClient::_processQuitGameMessage()
{
	if ( ! CheckValid( ccsInGame ) || m_game == NULL )
	{
		return;
	}

//	std::cout << "Sending quit to game players\n";
	WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
	l_buffer << msQuitGame << static_cast <int>( m_name.size() );
	l_buffer.writeArray( m_name.c_str(), m_name.size() );
	m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
	_forwardGameMessage( m_toSend );
	m_game->RemovePlayer( this );

	if ( m_game->GetInitiatorId() == m_id )
	{
//		std::cout << m_name << " was the initiator of the game, deleting it\n";
		m_plugin->RemoveGame( m_game->GetGameName(), m_id );
	}
	else if ( m_game->GetNbPlayers() == 0 )
	{
		m_plugin->RemoveGame( m_game->GetGameName(), m_game->GetInitiatorId() );
	}

	m_game = NULL;
}

void ChatTcpClient::_processGameMessage( ReadBuffer & p_buffer )
{
	if ( ! CheckValid( ccsInGame ) || m_game == NULL )
	{
		return;
	}

	WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
	l_buffer << msGame;
	l_buffer << static_cast <int>( m_name.size() );
	l_buffer.writeArray<char>( m_name.c_str(), m_name.size() );
	l_buffer.writeArray<char>( p_buffer.c_str() + sizeof( MessageReceived ), p_buffer.left() );
	m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
	_forwardGameMessage( m_toSend );
}

void ChatTcpClient::_processCreateGameMessage( ReadBuffer & p_buffer )
{
	if ( ! CheckValid( ccsNone ) )
	{
		return;
	}

	if ( m_anonymous )
	{
		WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
		l_buffer << msUnavailableToAnonymous;
		m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
		AsyncSend( m_toSend );
		return;
	}

	if ( m_status != ccsConnected )
	{
		SendGameDontExistMessage();
		return;
	}

	int l_size = 0;
	p_buffer >> l_size;
	String l_name;
	l_name.assign( p_buffer.readArray<char>( l_size ), l_size );
	m_game = m_plugin->AddGame( l_name, this );
	m_status = ccsInGame;
}

void ChatTcpClient::_processSitDownMessage()
{
	if ( ! CheckValid( ccsInRoom ) )
	{
		return;
	}

	m_sat = true;

	WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
	l_buffer << msSitDown << static_cast <int>( m_name.size() );
	l_buffer.writeArray<char>( m_name.c_str(), m_name.size() );

	m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
	_forwardMessage( m_toSend );
}

void ChatTcpClient::_processSitUpMessage()
{
	if ( ! CheckValid( ccsInRoom ) )
	{
		return;
	}

	m_sat = false;

	WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
	l_buffer << msSitUp << static_cast <int>( m_name.size() );
	l_buffer.writeArray<char>( m_name.c_str(), m_name.size() );

	m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
	_forwardMessage( m_toSend );
}

void ChatTcpClient::_processGameInfoMessage( ReadBuffer & p_buffer )
{
	if ( ! CheckValid( ccsNone ) )
	{
		return;
	}

	int l_size;
	p_buffer >> l_size;
	String l_gameName;
	char * l_cName = p_buffer.readArray<char>( l_size );

	if ( l_cName == NULL )
	{
		return;
	}

	l_gameName.assign( l_cName, l_size );

	unsigned int l_maxPlayers = m_plugin->GetGameMaxPLayers( l_gameName );

	WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
	l_buffer << msGameInfo << static_cast <int>( l_maxPlayers );
	m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
	AsyncSend( m_toSend );
}

void ChatTcpClient::_processGameStartMessage()
{
	if ( ! CheckValid( ccsInGame ) || m_game == NULL )
	{
		return;
	}

	m_game->StartGame();
}

void ChatTcpClient::_receivedMessageCallback( const String & p_message )
{
//	if (_checkOk()) return;
	if ( ! CheckValid( ccsNone ) )
	{
		std::cout << m_name << " Received a message but is not valid\n";
		return;
	}

	char * l_msgBuf = CStrCopy( p_message );
	ReadBuffer l_buffer( l_msgBuf, p_message.size() );
	int l_header;
	l_buffer >> l_header;
//	std::cout << m_name << " Received a message : " << l_header << "\n";

	if ( l_header == mrConnect && m_status < ccsConnected )
	{
//		std::cout << m_name << " Received message Connect\n";
		_processConnectMessage( l_buffer );
	}
	else if ( l_header == mrAvatar )
	{
		std::cout << m_name << " Received message Avatar\n";
		_processDressMessage( l_buffer );
	}
	else if ( l_header == mrJoin && m_status < ccsInRoom )
	{
		std::cout << m_name << " Received message Join\n";
		_processJoinMessage( l_buffer );
	}
	else if ( l_header == mrQuit && m_status == ccsInRoom )
	{
		std::cout << m_name << " Received message Quit\n";
		_processQuitMessage();
	}
	else if ( l_header == mrUpdateRooms )
	{
		std::cout << m_name << " Received message UpdateRooms\n";
		_sendRoomsMessage();
	}
	else if ( l_header == mrUpdate )
	{
//		std::cout << "Received message Update\n";
		_processUpdateMessage( l_buffer );
	}
	else if ( l_header == mrWalk )
	{
//		std::cout << "Received message Walk\n";
		_processWalkMessage( l_buffer );
	}
	else if ( l_header == mrTurn )
	{
//		std::cout << "Received message Turn\n";
		_processTurnMessage( l_buffer );
	}
	else if ( l_header == mrTalk )
	{
//		std::cout << "Received message Talk\n";
		_processTalkMessage( l_buffer );
	}
	else if ( l_header == mrWhisp )
	{
//		std::cout << "Received message Whisp\n";
		_processWhispMessage( l_buffer );
	}
	else if ( l_header == mrBeginWalk )
	{
//		std::cout << "Received message BeginWalk\n";
		_processBeginWalkMessage();
	}
	else if ( l_header == mrEndWalk )
	{
//		std::cout << "Received message EndWalk\n";
		_processEndWalkMessage( l_buffer );
	}
	else if ( l_header == mrBeginRun )
	{
//		std::cout << "Received message BeginRun\n";
		_processBeginRunMessage();
	}
	else if ( l_header == mrEndRun )
	{
//		std::cout << "Received message EndRun\n";
		_processEndRunMessage( l_buffer );
	}
	else if ( l_header == mrEmote )
	{
		std::cout << m_name << " Received message Emote\n";
		_processEmoteMessage( l_buffer );
	}
	else if ( l_header == mrRemoveFriend )
	{
		std::cout << m_name << " Received message RemoveFriend\n";
		_processRemoveFriendMessage( l_buffer );
	}
	else if ( l_header == mrRemoveIgnored )
	{
		std::cout << m_name << " Received message RemoveIgnored\n";
		_processRemoveIgnoredMessage( l_buffer );
	}
	else if ( l_header == mrUpdateFriendsList )
	{
		std::cout << m_name << " Received message UpdateFriendsList\n";
		_processUpdateFriendsMessage();
	}
	else if ( l_header == mrUpdateIgnoredList )
	{
		std::cout << m_name << " Received message UpdateIgnoredList\n";
		_processUpdateIgnoredMessage();
	}
	else if ( l_header == mrNewFriend )
	{
		std::cout << m_name << " Received message NewFriend\n";
		_processNewFriendMessage( l_buffer );
	}
	else if ( l_header == mrNewIgnored )
	{
		std::cout << m_name << " Received message NewIgnored\n";
		_processNewIgnoredMessage( l_buffer );
	}
	else if ( l_header == mrNewFriendAccept )
	{
		std::cout << m_name << " Received message NewFriendAccept\n";
		_processFriendAcceptMessage( l_buffer );
	}
	else if ( l_header == mrNewFriendRefuse )
	{
		std::cout << m_name << " Received message NewFriendRefuse\n";
		_processFriendRefuseMessage( l_buffer );
	}
	else if ( l_header == mrRefreshGamesList )
	{
		std::cout << m_name << " Received message RefreshGamesList\n";
		_processRefreshGamesListMessage( l_buffer );
	}
	else if ( l_header == mrJoinGame )
	{
		std::cout << m_name << " Received message JoinGame\n";
		_processJoinGameMessage( l_buffer );
	}
	else if ( l_header == mrCreateGame )
	{
		std::cout << m_name << " Received message CreateGame\n";
		_processCreateGameMessage( l_buffer );
	}
	else if ( l_header == mrQuitGame )
	{
		std::cout << m_name << " Received message QuitGame\n";
		_processQuitGameMessage();
	}
	else if ( l_header == mrGame )
	{
//		std::cout << m_name << " Received message Game\n";
		_processGameMessage( l_buffer );
	}
	else if ( l_header == mrSitDown )
	{
		std::cout << m_name << " Received message Sitdown\n";
		_processSitDownMessage();
	}
	else if ( l_header == mrSitUp )
	{
		std::cout << m_name << " Received message Situp\n";
		_processSitUpMessage();
	}
	else if ( l_header == mrGameInfo )
	{
		std::cout << m_name << " Received message Game info\n";
		_processGameInfoMessage( l_buffer );
	}
	else if ( l_header == mrGameStart )
	{
		std::cout << m_name << " Received message Game start\n";
		_processGameStartMessage();
	}
	else if ( l_header == mrRequestDresses )
	{
		std::cout << m_name << " Received message Request dresses\n";
		_sendDressesMessage();
	}

	delete [] l_msgBuf;

	CheckValid( ccsNone );
}

bool ChatTcpClient::_readerErrorCB( const boost::system::error_code & p_err )
{
	/*
	if (_checkOk()) return true;

	//	std::cout << "ChatTcpClient::_readerErrorCB - Active" << std::endl;

	boost::asio::ip::tcp::endpoint l_endPoint = TcpBaseClient::m_socket.remote_endpoint();
	std::string l_remoteAddr = l_endPoint.address().to_string() + ":" + General::Utils::ToString( l_endPoint.port());
	l_endPoint = TcpBaseClient::m_socket.local_endpoint();
	std::string l_localAddr = l_endPoint.address().to_string() + ":" + General::Utils::ToString( l_endPoint.port());

	std::cout << "ChatTcpClient::_readerErrorCB => Error on socket [" << l_localAddr << "]-[" << l_remoteAddr << "] : [" << p_err.message() << "]" << std::endl;
	if (m_working)
	{
		m_toDelete = true;
	}
	else
	{
		m_elypseService->EraseClient( this);
	}
	*/
	TcpReaderBase::m_service.post( SocketKiller( this ) );


	return true;
}

bool ChatTcpClient::_writerErrorCB( const boost::system::error_code & p_err )
{
	Stop();
	/*
	if (_checkOk()) return false;

	//	std::cout << "ChatTcpClient::_writerErrorCB - Active" << std::endl;

	boost::asio::ip::tcp::endpoint l_endPoint = TcpBaseClient::m_socket.remote_endpoint();
	std::string l_remoteAddr = l_endPoint.address().to_string() + ":" + General::Utils::ToString( l_endPoint.port());
	l_endPoint = TcpBaseClient::m_socket.local_endpoint();
	std::string l_localAddr = l_endPoint.address().to_string() + ":" + General::Utils::ToString( l_endPoint.port());

	std::cout << "ChatTcpClient::_writerErrorCB => Error on socket [" << l_localAddr << "]-[" << l_remoteAddr << "] : [" << p_err.message() << "]" << std::endl;
	if (m_working)
	{
		m_toDelete = true;
	}
	else
	{
		m_elypseService->EraseClient( this);
	}*/

	return true;
}

bool ChatTcpClient::_connectorErrorCB( const boost::system::error_code & p_err )
{
	Stop();
	return true;
}
