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

namespace Chat
{
	//************************************************************************************************

	namespace
	{
		struct SocketKiller
		{
		public:
			std::shared_ptr< ElypseTcpClient > & m_instance;

		public:
			SocketKiller( std::shared_ptr< ElypseTcpClient > & p_instance )
				:	m_instance( p_instance )
			{
			}
			void operator()()const
			{
				m_instance.reset();
			}
		};
	}

	//************************************************************************************************

	bool Dress::Load( General::Templates::ReadBuffer & p_buffer )
	{
		int l_count = 0;
		p_buffer >> m_slot;
		p_buffer >> m_id;
		p_buffer >> l_count;

		//std::cout << "ChatTcpClient::SaveDresses - Dress N°" << i << " = " << m_id << " at slot " << m_slot << " - Nb mats : " << l_count;
		for ( int j = 0 ; j < l_count ; j++ )
		{
			int l_submatId = 0;
			int l_matIndex = 0;
			p_buffer >> l_submatId;
			p_buffer >> l_matIndex;
			//std::cout << " - Material at submat " << l_submatId << ", index : " << l_matIndex;
			m_materials.insert( std::make_pair( l_submatId, l_matIndex ) );
		}

		//std::cout << "\n";
		return true;
	}

	void Dress::Save( General::Templates::WriteBuffer & p_buffer )const
	{
		//std::cout << "Slot : " << m_slot << " - Dress : " << m_id;
		p_buffer << m_slot;
		p_buffer << m_id;
		p_buffer << int( m_materials.size() );

		for ( auto && l_mat : m_materials )
		{
			p_buffer << l_mat.first;
			p_buffer << l_mat.second;
			//std::cout << " - SubMaterial " << l_mat.first << " : " << l_mat.second;
		}

		//std::cout << "\n";
	}

	//************************************************************************************************

	bool Tattoo::Load( General::Templates::ReadBuffer & p_buffer )
	{
		p_buffer >> m_slot;
		p_buffer >> m_id;
		//std::cout << "ChatTcpClient::SaveDresses - Tattoo N°" << i << " = " << m_id << " at slot " << m_slot << "\n";
		return true;
	}

	void Tattoo::Save( General::Templates::WriteBuffer & p_buffer )const
	{
		p_buffer << m_slot;
		p_buffer << m_id;
		//std::cout << "Slot : " << m_slot - Tattoo : " << m_id << "\n";
	}

	//************************************************************************************************

	bool Clothes::Load( General::Templates::ReadBuffer & p_buffer )
	{
		int l_count = 0;
		p_buffer >> m_sex;
		p_buffer >> l_count;

		//std::cout << "ChatTcpClient::SaveDresses - nb dresses : " << l_numDresses << "\n";
		for ( int i = 0 ; i < l_count ; i++ )
		{
			Dress l_dress;
			l_dress.Load( p_buffer );
			m_dresses.insert( std::make_pair( l_dress.m_slot, l_dress ) );
		}

		p_buffer >> l_count;

		//std::cout << "ChatTcpClient::SaveDresses - nb tattoos : " << l_numDresses << "\n";
		for ( int i = 0 ; i < l_count ; i++ )
		{
			Tattoo l_tattoo;
			l_tattoo.Load( p_buffer );
			m_tattoos.insert( std::make_pair( l_tattoo.m_slot, l_tattoo ) );
		}

		return true;
	}

	void Clothes::Save( General::Templates::WriteBuffer & p_buffer )const
	{
		p_buffer << m_sex;
		p_buffer << int( m_dresses.size() );

		for ( auto && l_dress : m_dresses )
		{
			l_dress.second.Save( p_buffer );
		}
		
		p_buffer << int( m_tattoos.size() );
		
		for ( auto && l_tattoo : m_tattoos )
		{
			l_tattoo.second.Save( p_buffer );
		}
	}

	//************************************************************************************************

	LoginInformations::LoginInformations( uint32_t p_id )
		: m_id( p_id )
	{
	}

	bool LoginInformations::Load( ReadBuffer & p_buffer )
	{
		int l_size;
		p_buffer >> l_size;
		String l_name;

		char * l_cname = p_buffer.readArray< char >( l_size );

		if ( !l_cname )
		{
			return false;
		}

		m_name.assign( l_cname, l_size );

		if ( m_name != "anonymous" )
		{
			m_anonymous = false;
			p_buffer >> l_size;
			String l_pass;

			char * l_cpass = p_buffer.readArray< char >( l_size );

			if ( !l_cpass )
			{
				return false;
			}

			m_password.assign( l_cpass, l_size );
		}
		else
		{
			char l_cname2[256];
			size_t l_written = snprintf( l_cname2, 256, "anonymous%i", m_id );
			m_name.assign( l_cname2, l_written );
		}

		return true;
	}

	void LoginInformations::Save( WriteBuffer & p_buffer )const
	{
		p_buffer << int( m_name.size() );
		p_buffer.writeArray( m_name.c_str(), int( m_name.size() ) );
	}

	//************************************************************************************************

	bool Avatar::Load( General::Templates::ReadBuffer & p_buffer )
	{
		return false;
	}

	void Avatar::Save( General::Templates::WriteBuffer & p_buffer )const
	{
		p_buffer << m_position.x << m_position.y << m_position.z;

		if ( m_sat )
		{
			p_buffer << 1;
		}
		else
		{
			p_buffer << 0;
		}
	}

	//************************************************************************************************

	uint32_t ChatTcpClient::ClientsCount = 0;

	ChatTcpClient::ChatTcpClient( std::shared_ptr< ElypseTcpService > p_elypseService, ChatPlugin * p_plugin )
		: ElypseTcpClient( p_elypseService )
		, m_plugin( p_plugin )
		, m_room()
		, m_status( ccsNone )
		, m_database( m_plugin->GetDatabase() )
		, m_login( ++ClientsCount )
		, m_game()
	{
		std::cout << "ChatTcpClient()" << std::endl;
	}

	ChatTcpClient::~ChatTcpClient()
	{
		std::cout << "ChatTcpClient - Deleting " << m_login.GetName() << std::endl;
		DoQuitRoom();
		DoQuitGame();

		m_status = ccsNone;

		if ( m_plugin != NULL )
		{
			m_plugin->RemoveClient( std::static_pointer_cast< ChatTcpClient >( shared_from_this() ) );
			m_plugin = NULL;
		}

		m_messages.clear();
		std::cout << "ChatTcpClient - Client " << m_login.GetName() << " deleted" << std::endl;
	}

	void ChatTcpClient::Stop()
	{
		if ( !m_avatar.IsStopped() )
		{
			m_avatar.Stop();
			m_noSend = true;
			TcpWriterBase::m_socket.close();
			TcpWriterBase::m_service.post( SocketKiller( shared_from_this() ) );
		}
	}

	bool ChatTcpClient::LoadNameAndPass( ReadBuffer & p_buffer )
	{
		bool l_return = false;

		if ( CheckValid( ccsNone ) )
		{
			if ( m_login.Load( p_buffer ) )
			{
				m_avatar.Work();

				if ( m_login.IsAnonymous() || DoGetDatabase()->ConnectUser( m_login ) )
				{
					l_return = DoEndWork();
				}
				else
				{
					DoEndWork();
				}
			}
		}

		return l_return;
	}

	bool ChatTcpClient::SaveDresses( General::Templates::ReadBuffer & p_buffer )
	{
		bool l_return = false;

		if ( CheckValid( ccsNone ) )
		{
			std::swap( m_clothes, Clothes() );
			m_clothes.Load( p_buffer );
			m_avatar.Work();
			DoGetDatabase()->SaveClothes( m_clothes, GetId() );
			l_return = DoEndWork();
		}

		return l_return;
	}

	const String & ChatTcpClient::GetRoomName()
	{
		String const * l_return = &ChatEmptyString;

		if ( CheckValid( ccsInRoom ) )
		{
			std::shared_ptr< ChatRoom > l_room = m_room.lock();

			if ( l_room )
			{
				l_return = &l_room->GetName();
			}
		}

		return *l_return;
	}

	void ChatTcpClient::SetRoom( std::shared_ptr< ChatRoom > p_room )
	{
		if ( CheckValid( ccsNone ) )
		{
			m_room = p_room;
			m_status = ccsInRoom;
		}
	}

	void ChatTcpClient::RemoveRoom()
	{
		if ( CheckValid( ccsInRoom ) )
		{
			m_room.reset();
			m_status = ccsDressed;
		}
	}

	void ChatTcpClient::RemoveFriend( ChatTcpClient * p_toRemove )
	{
		if ( CheckValid( ccsInRoom ) )
		{
			RemoveFriend( p_toRemove->GetName() );
		}
	}

	void ChatTcpClient::RemoveIgnored( ChatTcpClient * p_toRemove )
	{
		if ( !CheckValid( ccsInRoom ) )
		{
			return;
		}

		RemoveIgnored( p_toRemove->GetName() );
	}

	void ChatTcpClient::RemoveFriend( const String & p_toRemove )
	{
		if ( !CheckValid( ccsInRoom ) )
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
		if ( !CheckValid( ccsInRoom ) )
		{
			return;
		}

		ClientIdStrMap::iterator l_it = m_ignored.find( p_toRemove );

		if ( l_it == m_ignored.end() )
		{
			return;
		}

		m_ignored.erase( l_it );

		m_avatar.Work();
		DoGetDatabase()->RemoveIgnored( m_login.GetId(), l_it->second );

		if ( !DoEndWork() )
		{
			return;
		}
	}

	void ChatTcpClient::AddFriend( const String & p_name, unsigned int p_id )
	{
		if ( !CheckValid( ccsInRoom ) )
		{
			return;
		}

		ClientIdStrMap::iterator l_it = m_friends.find( p_name );

		if ( l_it != m_friends.end() )
		{
			return;
		}

		m_friends.insert( std::make_pair( p_name, p_id ) );
	}

	void ChatTcpClient::AddIgnored( const String & p_name, unsigned int p_id )
	{
		if ( !CheckValid( ccsInRoom ) )
		{
			return;
		}

		ClientIdStrMap::iterator l_it = m_ignored.find( p_name );

		if ( l_it != m_ignored.end() )
		{
			return;
		}

		m_ignored.insert( std::make_pair( p_name, p_id ) );
	}

	void ChatTcpClient::SendEndGameMessage()
	{
		if ( !CheckValid( ccsInGame ) )
		{
			return;
		}

		WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
		l_buffer << msEndGame;
		m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
		AsyncSend( m_toSend );
		m_game.reset();
	}

	void ChatTcpClient::SendStartGameMessage()
	{
		if ( !CheckValid( ccsInGame ) )
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
		if ( !CheckValid( ccsNone ) )
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
		if ( !CheckValid( ccsNone ) )
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
		if ( !CheckValid( ccsNone ) )
		{
			return;
		}

		WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
		l_buffer << msCreateGameOK;
		m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
		AsyncSend( m_toSend );
		m_status = ccsInGame;
	}

	void ChatTcpClient::SendGameJoinMessage( int p_place, const String & p_name, Clothes const & p_clothes )
	{
		if ( !CheckValid( ccsNone ) )
		{
			return;
		}

		WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
		l_buffer << msJoinGame;
		l_buffer << static_cast< int >( p_name.size() );
		l_buffer.writeArray< char >( p_name.c_str(), p_name.size() );
		l_buffer << p_place;
		p_clothes.Save( l_buffer );
		m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
		AsyncSend( m_toSend );
	}

	void ChatTcpClient::SendGameQuitMessage( const String & p_name )
	{
		if ( !CheckValid( ccsInGame ) )
		{
			return;
		}

		WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
		l_buffer << msQuitGame;
		l_buffer << static_cast< int >( p_name.size() );
		l_buffer.writeArray< char >( p_name.c_str(), p_name.size() );
		m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
		AsyncSend( m_toSend );
		m_status = ccsNone;
	}

	void ChatTcpClient::SendGameJoinOkMessage( int p_place )
	{
		if ( !CheckValid( ccsNone ) )
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

	bool ChatTcpClient::DoEndWork()
	{
		m_avatar.Work( false );

		if ( m_toDelete )
		{
			DoGetService()->EraseClient( shared_from_this() );
			return false;
		}

		return true;
	}

	bool ChatTcpClient::CheckValid( int p_comp )
	{
		if ( m_toDelete )
		{
			DoGetService()->EraseClient( shared_from_this() );
			return false;
		}

		return m_status >= p_comp;
	}

	void ChatTcpClient::DoQuitRoom()
	{
		std::shared_ptr< ChatRoom > l_room = m_room.lock();

		if ( m_status == ccsInRoom && !l_room )
		{
			WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
			l_buffer << msQuit;
			m_login.Save( l_buffer );
			m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
			DoForwardMessage( m_toSend );
			l_room->RemoveClient( std::static_pointer_cast< ChatTcpClient >( shared_from_this() ) );
			m_room.reset();
		}
	}

	void ChatTcpClient::DoQuitGame()
	{
		std::shared_ptr< ChatGame > l_game = m_game.lock();

		if ( m_status == ccsInGame && l_game )
		{
			std::cout << m_login.GetName() << " was in a game\n";
			WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
			l_buffer << msQuitGame;
			m_login.Save( l_buffer );
			m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
			DoForwardGameMessage( m_toSend );
			l_game->RemovePlayer( std::static_pointer_cast< ChatTcpClient >( shared_from_this() ) );

			if ( l_game->GetInitiatorId() == m_login.GetId() )
			{
				std::cout << m_login.GetName() << " was the initiator of the game, ending it\n";
				m_plugin->RemoveGame( l_game->GetGameName(), m_login.GetId() );
			}
			else if ( l_game->GetPlayersCount() == 0 )
			{
				m_plugin->RemoveGame( l_game->GetGameName(), l_game->GetInitiatorId() );
			}

			m_game.reset();
		}
	}

	bool ChatTcpClient::DoIsIgnored( const String & p_name )
	{
		if ( !CheckValid( ccsInRoom ) )
		{
			return false;
		}

		return ( m_ignored.find( p_name ) != m_ignored.end() );
	}

	String ChatTcpClient::DoBuildJoinMessage()
	{
		if ( !CheckValid( ccsNone ) )
		{
			return String();
		}

		WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
		l_buffer << msJoin;
		m_login.Save( l_buffer );
		m_clothes.Save( l_buffer );
		m_avatar.Save( l_buffer );
		String l_result;
		l_result.assign( l_buffer.c_str(), l_buffer.size() );
		return l_result;
	}

	void ChatTcpClient::DoSendDressesMessage()
	{
		//std::cout << "ChatTcpClient::DoSendDressesMessage - " << m_name << " sends his dresses\n";
		if ( !CheckValid( ccsNone ) )
		{
			std::cout << "ChatTcpClient::DoSendDressesMessage - nothing to send\n";
			return;
		}

		WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
		l_buffer << msAvatar;
		m_clothes.Save( l_buffer );
		m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
		AsyncSend( m_toSend );
		//std::cout << "ChatTcpClient::DoSendDressesMessage - " << m_name << " has ended the sending of his dresses : " << l_buffer.size() << "\n";
	}

	void ChatTcpClient::DoSendRoomsMessage()
	{
		if ( !CheckValid( ccsNone ) )
		{
			return;
		}

		WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
		l_buffer << msRooms;
		m_plugin->GetWorld()->Save( l_buffer );
		m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
		AsyncSend( m_toSend );
	}

	void ChatTcpClient::DoSendKickMessage()
	{
		if ( !CheckValid( ccsNone ) )
		{
			return;
		}

		WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
		l_buffer << msKickUser;
		m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
		AsyncSend( m_toSend );
	}

	void ChatTcpClient::DoSendGameJoinFail()
	{
		if ( !CheckValid( ccsNone ) )
		{
			return;
		}

		WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
		l_buffer << msJoinGameFail;
		m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
		AsyncSend( m_toSend );
	}

	void ChatTcpClient::DoForwardMessage( const String & p_message )
	{
		std::shared_ptr< ChatRoom > l_room = m_room.lock();

		if ( !CheckValid( ccsInRoom ) || l_room )
		{
			return;
		}

		l_room->ForwardMessage( p_message, GetName() );
	}

	void ChatTcpClient::DoForwardGameMessage( const String & p_message )
	{
		std::shared_ptr< ChatGame > l_game = m_game.lock();

		if ( !CheckValid( ccsInRoom ) || l_game )
		{
			return;
		}
		
		l_game->ForwardMessage( p_message, GetName() );
	}

	void ChatTcpClient::DoForwardTalkMessage( const String & p_message )
	{
		std::shared_ptr< ChatRoom > l_room = m_room.lock();

		if ( !CheckValid( ccsInRoom ) || l_room )
		{
			return;
		}

		l_room->ForwardMessage( p_message, GetName(), m_ignored );
	}

	void ChatTcpClient::DoProcessConnectMessage( ReadBuffer & p_buffer )
	{
		if ( !CheckValid( ccsNone ) )
		{
			return;
		}

		if ( LoadNameAndPass( p_buffer ) )
		{
			if ( !CheckValid( ccsNone ) )
			{
				return;
			}

			if ( !IsAnonymous() )
			{
				std::shared_ptr< ChatTcpClient > l_client = m_plugin->GetClient( GetName() );

				if ( l_client )
				{
					std::cout << "ChatTcpClient::DoProcessConnectMessage - " << GetName() << " was already connected\n";
					l_client->DoSendKickMessage();
					m_plugin->RemoveClient( l_client );
				}

				m_plugin->AddClient( std::static_pointer_cast< ChatTcpClient >( shared_from_this() ) );
				m_avatar.Work();

				std::swap( m_clothes, Clothes() );

				if ( !DoGetDatabase()->LoadClothes( m_clothes, GetId() ) )
				{
					std::cout << "ChatTcpClient::DoProcessConnectMessage - Can't load dresses for " << GetName() << "\n";
					return;
				}

				m_status = ccsDressed;

				if ( !DoEndWork() )
				{
					std::cout << "ChatTcpClient::DoProcessConnectMessage - " << GetName() << " has not ended his work\n";
					return;
				}

				m_avatar.Work();
				DoGetDatabase()->LoadFriends( m_friends, GetId() );

				if ( !DoEndWork() )
				{
					std::cout << "ChatTcpClient::DoProcessConnectMessage - " << GetName() << " has not ended his work\n";
					return;
				}

				m_avatar.Work();
				DoGetDatabase()->LoadIgnored( m_ignored, GetId() );

				if ( !DoEndWork() )
				{
					std::cout << "ChatTcpClient::DoProcessConnectMessage - " << GetName() << " has not ended his work\n";
					return;
				}
			}
			else
			{
				//std::cout << "ChatClient::DoProcessConnectMessage - Anonymous\n";
				m_plugin->AddClient( std::static_pointer_cast< ChatTcpClient >( shared_from_this() ) );
				//std::cout << "ChatClient::DoProcessConnectMessage - Added\n";
				std::map <int, Dress *>::iterator l_it;
				size_t l_size;
				size_t l_sex = GetRandom( 2 );
				std::swap( m_clothes, Clothes() );
				m_clothes.m_sex = l_sex;

				for ( int i = 1 ; i <= DoGetDatabase()->GetNumDressSlots() ; i++ )
				{
					Dress l_dress;
					l_dress.m_id = 137;
					l_dress.m_slot = ChatClientDressSlot( i );
					m_clothes.m_dresses.insert( std::make_pair( l_dress.m_slot, l_dress ) );
				}

				for ( auto && l_it : m_clothes.m_dresses )
				{
					l_size = DoGetDatabase()->GetDressesSize( l_sex, l_it.first );

					if ( l_size > 10 )
					{
						l_size = 10;
					}

					Dress & l_dress = l_it.second;
					l_dress.m_id = DoGetDatabase()->GetDress( l_sex, l_it.first, GetRandom( l_size ) );

					if ( l_it.first == 11 || l_it.first == 13 )
					{
						l_dress.m_materials.insert( std::make_pair( 0, GetRandom( 6 ) ) );
					}
				}
			}

			std::cout << "ChatClient " << GetName() << " connected\n";
			WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
			l_buffer << msConnectOK;
			m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
			AsyncSend( m_toSend );
			m_status = ccsConnected;
			return;
		}

		if ( !CheckValid( ccsNone ) )
		{
			return;
		}

		//std::cout << "ChatTcpClient::DoProcessConnectMessage - Not connected\n";

		WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
		l_buffer << msConnectFail;
		m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
		AsyncSend( m_toSend );

	}

	void ChatTcpClient::DoProcessDressMessage( ReadBuffer & p_buffer )
	{
		if ( !CheckValid( ccsConnected ) )
		{
			return;
		}

		if ( !IsAnonymous() )
		{
			//std::cout << "ChatTcpClient::DoProcessDressMessage - not anonymous\n";
			SaveDresses( p_buffer );
		}
		else
		{
			//std::cout << "ChatTcpClient::DoProcessDressMessage - anonymous\n";
		}

		if ( m_status < ccsInRoom )
		{
			DoSendRoomsMessage();
		}

		m_status = ccsDressed;
	}

	void ChatTcpClient::DoProcessJoinMessage( ReadBuffer & p_buffer )
	{
		if ( !CheckValid( ccsDressed ) )
		{
			return;
		}

		int l_size;
		p_buffer >> l_size;
		String l_roomName;
		l_roomName.assign( p_buffer.readArray< char >( l_size ), l_size );

		//std::cout << "DoProcessJoinMessage - " << l_roomName << "\n";
		std::shared_ptr< ChatRoom > l_room = m_plugin->GetRoom( l_roomName );
		String l_joinMessage = DoBuildJoinMessage();

		if ( l_room )
		{
			for ( auto && l_it : l_room->GetClients() )
			{
				AsyncSend( l_it.second->DoBuildJoinMessage() );
				l_it.second->AsyncSend( l_joinMessage );
			}

			l_room->AddClient( std::static_pointer_cast< ChatTcpClient >( shared_from_this() ) );
			m_status = ccsInRoom;
		}
		else
		{
			std::cout << "NULL Room\n";
		}
	}

	void ChatTcpClient::DoProcessQuitMessage()
	{
		std::shared_ptr< ChatRoom > l_room = m_room.lock();

		if ( !CheckValid( ccsInRoom ) || !l_room )
		{
			return;
		}

		//std::cout << "Sending quit to room clients\n";
		WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
		l_buffer << msQuit;
		m_login.Save( l_buffer );
		m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
		DoForwardMessage( m_toSend );
		l_room->RemoveClient( std::static_pointer_cast< ChatTcpClient >( shared_from_this() ) );
	}

	void ChatTcpClient::DoProcessUpdateMessage( ReadBuffer & p_buffer )
	{
		if ( !CheckValid( ccsInRoom ) )
		{
			return;
		}

		p_buffer >> m_avatar.GetPosition().x >> m_avatar.GetPosition().y >> m_avatar.GetPosition().z >> m_avatar.GetYaw();

		WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
		l_buffer << msUpdate;
		m_login.Save( l_buffer );
		l_buffer << GetPosition().x << GetPosition().y << GetPosition().z << GetYaw();
		m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
		DoForwardMessage( m_toSend );
	}

	void ChatTcpClient::DoProcessWalkMessage( ReadBuffer & p_buffer )
	{
		if ( !CheckValid( ccsInRoom ) )
		{
			return;
		}

		int l_running;
		int l_backward;
		p_buffer >> m_avatar.GetPosition().x >> m_avatar.GetPosition().y >> m_avatar.GetPosition().z >> l_backward >> l_running;

		WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
		l_buffer << msWalk;
		m_login.Save( l_buffer );
		l_buffer << GetPosition().x << GetPosition().y << GetPosition().z << l_backward << l_running;

		m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
		DoForwardMessage( m_toSend );
	}

	void ChatTcpClient::DoProcessTurnMessage( ReadBuffer & p_buffer )
	{
		if ( !CheckValid( ccsInRoom ) )
		{
			return;
		}

		p_buffer >> m_avatar.GetYaw();

		WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
		l_buffer << msTurn;
		m_login.Save( l_buffer );
		l_buffer << GetYaw();

		m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
		DoForwardMessage( m_toSend );
	}

	void ChatTcpClient::DoProcessTalkMessage( ReadBuffer & p_buffer )
	{
		if ( !CheckValid( ccsInRoom ) )
		{
			return;
		}

		int l_size;
		p_buffer >> l_size;
		//std::cout << l_size << "\n";
		String l_message;
		l_message.assign( p_buffer.readArray< char >( l_size ), l_size );
		std::cout << GetName() << " : " << l_message << "\n";

		WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
		l_buffer << msTalk;
		m_login.Save( l_buffer );
		l_buffer << l_size;
		l_buffer.writeArray< char >( l_message.c_str(), l_size );

		m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
		DoForwardTalkMessage( m_toSend );
	}

	void ChatTcpClient::DoProcessWhispMessage( ReadBuffer & p_buffer )
	{
		if ( !CheckValid( ccsInRoom ) )
		{
			return;
		}

		if ( IsAnonymous() )
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
		l_message.assign( p_buffer.readArray< char >( l_size ), l_size );
		String l_distantName;
		size_t l_index = l_message.find_first_of( " " );
		l_distantName = l_message.substr( 0, l_index );
		l_message = l_message.substr( l_index + 1 );
		std::cout << "DoProcessWhispMessage - " << l_distantName << " - " << l_message << "\n";

		WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
		l_buffer << msWhisp;
		m_login.Save( l_buffer );;
		l_buffer << l_size;
		l_buffer.writeArray< char >( l_message.c_str(), l_message.size() );

		ChatRoomStrMap l_rooms = m_plugin->GetWorld()->GetRooms();
		ChatTcpClientStrMap l_clients;
		bool l_found = false;
		auto && l_rIt = l_rooms.begin();
		String l_toSend;

		while ( l_rIt != l_rooms.end() && ! l_found )
		{
			std::shared_ptr< ChatRoom > l_room = l_rIt->second;
			ChatTcpClientStrMap l_clients = l_room->GetClients();
			auto && l_it = l_clients.begin();

			while ( l_it != l_clients.end() && ! l_found )
			{
				std::shared_ptr< ChatTcpClient > l_client = l_it->second;

				if ( l_client->GetName() == l_distantName && l_client->GetName() != GetName() )
				{
					l_found = true;
					l_toSend.assign( l_buffer.c_str(), l_buffer.size() );

					if ( !l_client->DoIsIgnored( GetName() ) )
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

		if ( !l_found )
		{
			std::cout << "WhispFail\n";
			l_buffer = WriteBuffer( m_messageBuffer, MAX_MSG_LENGTH );
			l_buffer << msWhispFail;
			m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
			AsyncSend( m_toSend );
		}
	}

	void ChatTcpClient::DoProcessBeginWalkMessage()
	{
		if ( !CheckValid( ccsInRoom ) )
		{
			return;
		}

		m_avatar.Sit( false );
		WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
		l_buffer << msBeginWalk;
		m_login.Save( l_buffer );
		m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
		DoForwardMessage( m_toSend );
	}

	void ChatTcpClient::DoProcessEndWalkMessage( ReadBuffer & p_buffer )
	{
		if ( !CheckValid( ccsInRoom ) )
		{
			return;
		}

		float l_x, l_y, l_z, l_yaw;
		p_buffer >> l_x >> l_y >> l_z >> l_yaw;

		WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
		l_buffer << msEndWalk;
		m_login.Save( l_buffer );
		l_buffer << l_x << l_y << l_z << l_yaw;

		m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
		DoForwardMessage( m_toSend );
	}

	void ChatTcpClient::DoProcessBeginRunMessage()
	{
		if ( !CheckValid( ccsInRoom ) )
		{
			return;
		}

		m_avatar.Sit( false );
		WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
		l_buffer << msBeginRun;
		m_login.Save( l_buffer );
		m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
		DoForwardMessage( m_toSend );
	}

	void ChatTcpClient::DoProcessEndRunMessage( General::Templates::ReadBuffer & p_buffer )
	{
		if ( !CheckValid( ccsInRoom ) )
		{
			return;
		}

		float l_x, l_y, l_z, l_yaw;
		int l_stillWalking;
		p_buffer >> l_stillWalking >> l_x >> l_y >> l_z >> l_yaw;

		WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
		l_buffer << msEndRun;
		m_login.Save( l_buffer );
		l_buffer << l_x << l_y << l_z << l_yaw;
		m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
		DoForwardMessage( m_toSend );
	}

	void ChatTcpClient::DoProcessEmoteMessage( ReadBuffer & p_buffer )
	{
		if ( !CheckValid( ccsInRoom ) )
		{
			return;
		}

		int l_size;
		p_buffer >> l_size;
		String l_message;
		l_message.assign( p_buffer.readArray< char >( l_size ), l_size );
		std::cout << "DoProcessEmoteMessage - " << l_message << "\n";

		WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
		l_buffer << msEmote;
		m_login.Save( l_buffer );
		l_buffer << l_size;
		l_buffer.writeArray< char >( l_message.c_str(), l_size );

		m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
		DoForwardMessage( m_toSend );
	}

	void ChatTcpClient::DoProcessRemoveFriendMessage( ReadBuffer & p_buffer )
	{
		if ( !CheckValid( ccsInRoom ) )
		{
			return;
		}

		if ( IsAnonymous() )
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
		l_toRemove.assign( p_buffer.readArray< char >( l_size ), l_size );

		m_avatar.Work();
		unsigned int l_friendId = DoGetDatabase()->GetClientId( l_toRemove );

		if ( !DoEndWork() )
		{
			return;
		}

		if ( l_friendId != 0 )
		{
			m_avatar.Work();
			DoGetDatabase()->RemoveFriend( m_login.GetId(), l_friendId );

			if ( !DoEndWork() )
			{
				return;
			}

			DoGetDatabase()->RemoveFriend( l_friendId, m_login.GetId() );

			if ( !DoEndWork() )
			{
				return;
			}

			std::shared_ptr< ChatTcpClient > l_friend = m_plugin->GetClient( l_toRemove );

			if ( l_friend )
			{
				l_friend->RemoveFriend( GetName() );
			}

			auto && l_it = m_friends.find( l_toRemove );

			if ( l_it != m_friends.end() )
			{
				m_friends.erase( l_it );
			}
		}
	}

	void ChatTcpClient::DoProcessRemoveIgnoredMessage( ReadBuffer & p_buffer )
	{
		if ( !CheckValid( ccsInRoom ) )
		{
			return;
		}

		if ( IsAnonymous() )
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
		l_toRemove.assign( p_buffer.readArray< char >( l_size ), l_size );

		RemoveIgnored( l_toRemove );
	}

	void ChatTcpClient::DoProcessNewFriendMessage( ReadBuffer & p_buffer )
	{
		if ( !CheckValid( ccsInRoom ) )
		{
			return;
		}

		if ( IsAnonymous() )
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
		l_name.assign( p_buffer.readArray< char >( l_size ), l_size );
		//std::cout << "DoProcessNewFriendMessage - " << l_name << "\n";

		if ( l_name != GetName() )
		{
			std::shared_ptr< ChatTcpClient > l_newFriend = m_plugin->GetClient( l_name );
			WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );

			if ( l_newFriend == NULL )
			{
				std::cout << "DoProcessNewFriendMessage - Friend not found\n";
				l_buffer << msNewFriendDoesntExist << l_size;
				l_buffer.writeArray< char >( l_name.c_str(), l_size );
				m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
				AsyncSend( m_toSend );
			}
			else if ( m_friends.find( l_name ) != m_friends.end() )
			{
				std::cout << "DoProcessNewFriendMessage - Friend found, already in friends\n";
				l_buffer << msNewFriendAlreadyFriend << l_size;
				l_buffer.writeArray< char >( l_name.c_str(), l_size );
				m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
				AsyncSend( m_toSend );
			}
			else
			{
				//std::cout << "DoProcessNewFriendMessage - Friend found\n";
				l_buffer << msNewFriendAsk;
				m_login.Save( l_buffer );
				m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
				l_newFriend->AsyncSend( m_toSend );
			}
		}
	}

	void ChatTcpClient::DoProcessNewIgnoredMessage( ReadBuffer & p_buffer )
	{
		if ( !CheckValid( ccsInRoom ) )
		{
			return;
		}

		if ( IsAnonymous() )
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
		l_name.assign( p_buffer.readArray< char >( l_size ), l_size );

		if ( l_name != GetName() )
		{
			m_avatar.Work();
			unsigned int l_ignoredId = DoGetDatabase()->GetClientId( l_name );

			if ( !DoEndWork() )
			{
				return;
			}

			WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );

			if ( l_ignoredId == 0 )
			{
				l_buffer << msNewIgnoredDoesntExist << l_size;
				l_buffer.writeArray< char >( l_name.c_str(), l_size );
				m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
				AsyncSend( m_toSend );
			}
			else if ( m_ignored.find( l_name ) != m_ignored.end() )
			{
				l_buffer << msNewIgnoredAlreadyIgnored << l_size;
				l_buffer.writeArray< char >( l_name.c_str(), l_size );
				m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
				AsyncSend( m_toSend );
			}
			else
			{
				m_ignored.insert( ClientIdStrMap::value_type( l_name, l_ignoredId ) );
				m_avatar.Work();
				DoGetDatabase()->AddIgnored( m_login.GetId(), l_ignoredId );

				if ( !DoEndWork() )
				{
					return;
				}

				l_buffer << msNewIgnoredDone << l_size;
				l_buffer.writeArray< char >( l_name.c_str(), l_size );
				m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
				AsyncSend( m_toSend );
			}
		}
	}

	void ChatTcpClient::DoProcessUpdateFriendsMessage()
	{
		if ( !CheckValid( ccsInRoom ) )
		{
			return;
		}

		if ( IsAnonymous() )
		{
			WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
			l_buffer << msUnavailableToAnonymous;
			m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
			AsyncSend( m_toSend );
			return;
		}

		String l_noRoom = "Non connecte";

		WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
		l_buffer << msUpdateFriendsList;
		l_buffer << int( m_friends.size() );

		for ( auto && l_it : m_friends )
		{
			std::shared_ptr< ChatTcpClient > l_friend = m_plugin->GetClient( l_it.first );
			l_buffer << int( l_it.first.size() );
			l_buffer.writeArray< char >( l_it.first.c_str(), l_it.first.size() );

			if ( l_friend && l_friend->GetStatus() == ccsInRoom )
			{
				l_buffer << int( l_friend->GetRoomName().size() );
				l_buffer.writeArray< char >( l_friend->GetRoomName().c_str(), l_friend->GetRoomName().size() );
			}
			else
			{
				l_buffer << int( l_noRoom.size() );
				l_buffer.writeArray< char >( l_noRoom.c_str(), l_noRoom.size() );
			}
		}

		m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
		AsyncSend( m_toSend );
	}

	void ChatTcpClient::DoProcessUpdateIgnoredMessage()
	{
		if ( !CheckValid( ccsInRoom ) )
		{
			return;
		}

		if ( IsAnonymous() )
		{
			WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
			l_buffer << msUnavailableToAnonymous;
			m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
			AsyncSend( m_toSend );
			return;
		}

		WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
		l_buffer << msUpdateIgnoredList;
		l_buffer << static_cast< int >( m_ignored.size() );

		for ( auto && l_it : m_ignored )
		{
			std::shared_ptr< ChatTcpClient > l_ignored = m_plugin->GetClient( l_it.first );
			l_buffer << int( l_it.first.size() );
			l_buffer.writeArray< char >( l_it.first.c_str(), l_it.first.size() );

			if ( l_ignored && l_ignored->GetStatus() == ccsInRoom )
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

	void ChatTcpClient::DoProcessFriendRefuseMessage( ReadBuffer & p_buffer )
	{
		if ( !CheckValid( ccsInRoom ) )
		{
			return;
		}

		if ( IsAnonymous() )
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
		l_refusedName.assign( p_buffer.readArray< char >( l_size ), l_size );

		std::shared_ptr< ChatTcpClient > l_refused = m_plugin->GetClient( l_refusedName );

		if ( l_refused )
		{
			WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
			l_buffer << msNewFriendRefuse;
			m_login.Save( l_buffer );
			m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
			l_refused->AsyncSend( m_toSend );
		}
	}

	void ChatTcpClient::DoProcessFriendAcceptMessage( ReadBuffer & p_buffer )
	{
		if ( !CheckValid( ccsInRoom ) )
		{
			return;
		}

		if ( IsAnonymous() )
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
		l_acceptedName.assign( p_buffer.readArray< char >( l_size ), l_size );

		std::shared_ptr< ChatTcpClient > l_accepted = m_plugin->GetClient( l_acceptedName );

		if ( l_accepted )
		{
			WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
			l_buffer << msNewFriendAccept;
			m_login.Save( l_buffer );

			l_accepted->m_friends.insert( std::make_pair( GetName(), m_login.GetId() ) );
			m_friends.insert( std::make_pair( l_acceptedName, l_accepted->GetId() ) );

			m_avatar.Work();
			DoGetDatabase()->AddFriend( l_accepted->GetId(), m_login.GetId() );

			if ( !DoEndWork() )
			{
				return;
			}

			DoGetDatabase()->AddFriend( m_login.GetId(), l_accepted->GetId() );

			if ( !DoEndWork() )
			{
				return;
			}

			m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
			l_accepted->AsyncSend( m_toSend );
		}
	}

	void ChatTcpClient::DoProcessRefreshGamesListMessage( ReadBuffer & p_buffer )
	{
		if ( !CheckValid( ccsConnected ) )
		{
			return;
		}

		int l_size = 0;
		p_buffer >> l_size;
		String l_name;
		l_name.assign( p_buffer.readArray< char >( l_size ), l_size );
		StrUIntIdMap l_map = m_plugin->GetGamesList( l_name );
		WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
		l_buffer << msRefreshGamesList;
		l_buffer << static_cast< int >( l_map.size() );

		for ( StrUIntIdMap::iterator l_it = l_map.begin() ; l_it != l_map.end() ; ++l_it )
		{
			l_buffer << l_it->first;//id creator
			l_buffer << l_it->second.second;//nb players
			l_buffer << static_cast< int >( l_it->second.first.size() );
			l_buffer.writeArray< char >( l_it->second.first.c_str(), l_it->second.first.size() ); //creator name
		}

		m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
		AsyncSend( m_toSend );
	}

	void ChatTcpClient::DoProcessJoinGameMessage( ReadBuffer & p_buffer )
	{
		if ( !CheckValid( ccsNone ) )
		{
			return;
		}

		if ( IsAnonymous() )
		{
			WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
			l_buffer << msUnavailableToAnonymous;
			m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
			AsyncSend( m_toSend );
			return;
		}

		if ( m_status != ccsConnected )
		{
			DoSendGameJoinFail();
			return;
		}

		int l_size = 0;
		p_buffer >> l_size;
		String l_name;
		l_name.assign( p_buffer.readArray< char >( l_size ), l_size );
		int l_id;
		p_buffer >> l_id;
		std::shared_ptr< ChatGame > l_game = m_plugin->GetGame( l_name, l_id );

		if ( l_game || !l_game->AddPlayer( std::static_pointer_cast< ChatTcpClient >( shared_from_this() ) ) )
		{
			DoSendGameJoinFail();
			return;
		}
		else
		{
			int l_place = l_game->GetPlayerPlace( GetName() );
			SendGameJoinOkMessage( l_place );
			ClientIdChatCLientMap l_players = l_game->GetPlayers();
			ClientIdChatCLientMap::iterator l_myIt = l_players.find( m_login.GetId() );

			for ( auto && l_it : l_players )
			{
				std::shared_ptr< ChatTcpClient > l_player = l_it.second.lock();

				if ( l_player && l_player->GetId() != GetId() && !l_player->IsToDelete() )
				{
					l_player->SendGameJoinMessage( l_place, GetName(), m_clothes );
					SendGameJoinMessage( l_game->GetPlayerPlace( l_player->GetName() ),
										 l_player->GetName(),
										 l_player->GetClothes() );
				}
			}
		}

		m_game = l_game;
		m_status = ccsInGame;
		return;
	}

	void ChatTcpClient::DoProcessQuitGameMessage()
	{
		std::shared_ptr< ChatGame > l_game = m_game.lock();

		if ( !CheckValid( ccsInGame ) || l_game )
		{
			return;
		}

		//std::cout << "Sending quit to game players\n";
		WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
		l_buffer << msQuitGame;
		m_login.Save( l_buffer );
		m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
		DoForwardGameMessage( m_toSend );
		l_game->RemovePlayer( std::static_pointer_cast< ChatTcpClient >( shared_from_this() ) );

		if ( l_game->GetInitiatorId() == m_login.GetId() )
		{
			//std::cout << m_name << " was the initiator of the game, deleting it\n";
			m_plugin->RemoveGame( l_game->GetGameName(), m_login.GetId() );
		}
		else if ( l_game->GetPlayersCount() == 0 )
		{
			m_plugin->RemoveGame( l_game->GetGameName(), l_game->GetInitiatorId() );
		}

		m_game.reset();
	}

	void ChatTcpClient::DoProcessGameMessage( ReadBuffer & p_buffer )
	{
		std::shared_ptr< ChatGame > l_game = m_game.lock();

		if ( !CheckValid( ccsInGame ) || l_game )
		{
			return;
		}

		WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
		l_buffer << msGame;
		m_login.Save( l_buffer );
		l_buffer.writeArray< char >( p_buffer.c_str() + sizeof( MessageReceived ), p_buffer.left() );
		m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
		DoForwardGameMessage( m_toSend );
	}

	void ChatTcpClient::DoProcessCreateGameMessage( ReadBuffer & p_buffer )
	{
		if ( !CheckValid( ccsNone ) )
		{
			return;
		}

		if ( IsAnonymous() )
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
		l_name.assign( p_buffer.readArray< char >( l_size ), l_size );
		m_game = m_plugin->AddGame( l_name, std::static_pointer_cast< ChatTcpClient >( shared_from_this() ) );
		m_status = ccsInGame;
	}

	void ChatTcpClient::DoProcessSitDownMessage()
	{
		if ( !CheckValid( ccsInRoom ) )
		{
			return;
		}
		
		m_avatar.Sit();

		WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
		l_buffer << msSitDown;
		m_login.Save( l_buffer );

		m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
		DoForwardMessage( m_toSend );
	}

	void ChatTcpClient::DoProcessSitUpMessage()
	{
		if ( !CheckValid( ccsInRoom ) )
		{
			return;
		}

		m_avatar.Sit( false );

		WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
		l_buffer << msSitUp;
		m_login.Save( l_buffer );

		m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
		DoForwardMessage( m_toSend );
	}

	void ChatTcpClient::DoProcessGameInfoMessage( ReadBuffer & p_buffer )
	{
		if ( !CheckValid( ccsNone ) )
		{
			return;
		}

		int l_size;
		p_buffer >> l_size;
		String l_gameName;
		char * l_cName = p_buffer.readArray< char >( l_size );

		if ( l_cName == NULL )
		{
			return;
		}

		l_gameName.assign( l_cName, l_size );

		unsigned int l_maxPlayers = m_plugin->GetGameMaxPLayers( l_gameName );

		WriteBuffer l_buffer( m_messageBuffer, MAX_MSG_LENGTH );
		l_buffer << msGameInfo << static_cast< int >( l_maxPlayers );
		m_toSend.assign( l_buffer.c_str(), l_buffer.size() );
		AsyncSend( m_toSend );
	}

	void ChatTcpClient::DoProcessGameStartMessage()
	{
		std::shared_ptr< ChatGame > l_game = m_game.lock();

		if ( !CheckValid( ccsInGame ) || l_game )
		{
			return;
		}

		l_game->StartGame();
	}

	void ChatTcpClient::CallbackReceivedMessage( const String & p_message )
	{
		if ( !CheckValid( ccsNone ) )
		{
			std::cout << GetName() << " Received a message but is not valid\n";
			return;
		}

		char * l_msgBuf = CStrCopy( p_message );
		ReadBuffer l_buffer( l_msgBuf, p_message.size() );
		int l_header;
		l_buffer >> l_header;
		//std::cout << m_name << " Received a message : " << l_header << "\n";

		switch ( l_header )
		{
		case mrConnect:
			if ( m_status < ccsConnected )
			{
				//std::cout << m_name << " Received message Connect\n";
				DoProcessConnectMessage( l_buffer );
			}
			break;
		case mrAvatar:
			std::cout << GetName() << " Received message Avatar\n";
			DoProcessDressMessage( l_buffer );
			break;
		case mrJoin:
			if ( m_status < ccsInRoom )
			{
				std::cout << GetName() << " Received message Join\n";
				DoProcessJoinMessage( l_buffer );
			}
			break;
		case mrQuit:
			if ( m_status == ccsInRoom )
			{
				std::cout << GetName() << " Received message Quit\n";
				DoProcessQuitMessage();
			}
			break;
		case mrUpdateRooms:
			std::cout << GetName() << " Received message UpdateRooms\n";
			DoSendRoomsMessage();
			break;
		case mrUpdate:
			//std::cout << "Received message Update\n";
			DoProcessUpdateMessage( l_buffer );
			break;
		case mrWalk:
			//std::cout << "Received message Walk\n";
			DoProcessWalkMessage( l_buffer );
			break;
		case mrTurn:
			//std::cout << "Received message Turn\n";
			DoProcessTurnMessage( l_buffer );
			break;
		case mrTalk:
			//std::cout << "Received message Talk\n";
			DoProcessTalkMessage( l_buffer );
			break;
		case mrWhisp:
			//std::cout << "Received message Whisp\n";
			DoProcessWhispMessage( l_buffer );
			break;
		case mrBeginWalk:
			//std::cout << "Received message BeginWalk\n";
			DoProcessBeginWalkMessage();
			break;
		case mrEndWalk:
			//std::cout << "Received message EndWalk\n";
			DoProcessEndWalkMessage( l_buffer );
			break;
		case mrBeginRun:
			//std::cout << "Received message BeginRun\n";
			DoProcessBeginRunMessage();
			break;
		case mrEndRun:
			//std::cout << "Received message EndRun\n";
			DoProcessEndRunMessage( l_buffer );
			break;
		case mrEmote:
			std::cout << GetName() << " Received message Emote\n";
			DoProcessEmoteMessage( l_buffer );
			break;
		case mrRemoveFriend:
			std::cout << GetName() << " Received message RemoveFriend\n";
			DoProcessRemoveFriendMessage( l_buffer );
			break;
		case mrRemoveIgnored:
			std::cout << GetName() << " Received message RemoveIgnored\n";
			DoProcessRemoveIgnoredMessage( l_buffer );
			break;
		case mrUpdateFriendsList:
			std::cout << GetName() << " Received message UpdateFriendsList\n";
			DoProcessUpdateFriendsMessage();
			break;
		case mrUpdateIgnoredList:
			std::cout << GetName() << " Received message UpdateIgnoredList\n";
			DoProcessUpdateIgnoredMessage();
			break;
		case mrNewFriend:
			std::cout << GetName() << " Received message NewFriend\n";
			DoProcessNewFriendMessage( l_buffer );
			break;
		case mrNewIgnored:
			std::cout << GetName() << " Received message NewIgnored\n";
			DoProcessNewIgnoredMessage( l_buffer );
			break;
		case mrNewFriendAccept:
			std::cout << GetName() << " Received message NewFriendAccept\n";
			DoProcessFriendAcceptMessage( l_buffer );
			break;
		case mrNewFriendRefuse:
			std::cout << GetName() << " Received message NewFriendRefuse\n";
			DoProcessFriendRefuseMessage( l_buffer );
			break;
		case mrRefreshGamesList:
			std::cout << GetName() << " Received message RefreshGamesList\n";
			DoProcessRefreshGamesListMessage( l_buffer );
			break;
		case mrJoinGame:
			std::cout << GetName() << " Received message JoinGame\n";
			DoProcessJoinGameMessage( l_buffer );
			break;
		case mrCreateGame:
			std::cout << GetName() << " Received message CreateGame\n";
			DoProcessCreateGameMessage( l_buffer );
			break;
		case mrQuitGame:
			std::cout << GetName() << " Received message QuitGame\n";
			DoProcessQuitGameMessage();
			break;
		case mrGame:
			//std::cout << m_name << " Received message Game\n";
			DoProcessGameMessage( l_buffer );
			break;
		case mrSitDown:
			std::cout << GetName() << " Received message Sitdown\n";
			DoProcessSitDownMessage();
			break;
		case mrSitUp:
			std::cout << GetName() << " Received message Situp\n";
			DoProcessSitUpMessage();
			break;
		case mrGameInfo:
			std::cout << GetName() << " Received message Game info\n";
			DoProcessGameInfoMessage( l_buffer );
			break;
		case mrGameStart:
			std::cout << GetName() << " Received message Game start\n";
			DoProcessGameStartMessage();
			break;
		case mrRequestDresses:
			std::cout << GetName() << " Received message Request dresses\n";
			DoSendDressesMessage();
			break;
		}

		delete [] l_msgBuf;

		CheckValid( ccsNone );
	}

	bool ChatTcpClient::CallbackReaderError( const boost::system::error_code & p_err )
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
		TcpReaderBase::m_service.post( SocketKiller( shared_from_this() ) );


		return true;
	}

	bool ChatTcpClient::CallbackWriterError( const boost::system::error_code & p_err )
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

	bool ChatTcpClient::CallbackConnectorError( const boost::system::error_code & p_err )
	{
		Stop();
		return true;
	}
}
