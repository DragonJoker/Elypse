/* See Licence.txt in root folder */
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
using namespace Elypse::Server;

namespace Chat
{
	//************************************************************************************************

	namespace
	{
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
	}

	//************************************************************************************************

	bool Dress::Load( General::Templates::ReadBuffer & p_buffer )
	{
		int l_count = 0;
		p_buffer >> m_slot;
		p_buffer >> m_id;
		p_buffer >> l_count;

		std::clog << "ChatTcpClient::SaveDresses - Dress " << m_id << " at slot " << m_slot << " - Nb mats : " << l_count;

		for ( int j = 0 ; j < l_count ; j++ )
		{
			int l_submatId = 0;
			int l_matIndex = 0;
			p_buffer >> l_submatId;
			p_buffer >> l_matIndex;
			std::clog << " - Material at submat " << l_submatId << ", index : " << l_matIndex;
			m_materials.insert( std::make_pair( l_submatId, l_matIndex ) );
		}

		std::clog << std::endl;
		return true;
	}

	void Dress::Save( General::Templates::WriteBuffer & p_buffer )const
	{
		std::clog << "Slot : " << m_slot << " - Dress : " << m_id;
		p_buffer << m_slot;
		p_buffer << m_id;
		p_buffer << int( m_materials.size() );

		for ( auto && l_mat : m_materials )
		{
			p_buffer << l_mat.first;
			p_buffer << l_mat.second;
			std::clog << " - SubMaterial " << l_mat.first << " : " << l_mat.second;
		}

		std::clog << std::endl;
	}

	//************************************************************************************************

	bool Tattoo::Load( General::Templates::ReadBuffer & p_buffer )
	{
		p_buffer >> m_slot;
		p_buffer >> m_id;
		std::clog << "ChatTcpClient::SaveDresses - Tattoo " << m_id << " at slot " << m_slot << std::endl;
		return true;
	}

	void Tattoo::Save( General::Templates::WriteBuffer & p_buffer )const
	{
		p_buffer << m_slot;
		p_buffer << m_id;
		std::clog << "Slot : " << m_slot << " - Tattoo : " << m_id << std::endl;
	}

	//************************************************************************************************

	bool Clothes::Load( General::Templates::ReadBuffer & p_buffer )
	{
		int l_count = 0;
		p_buffer >> m_sex;
		p_buffer >> l_count;

		std::clog << "ChatTcpClient::SaveDresses - nb dresses : " << l_count << std::endl;

		for ( int i = 0 ; i < l_count ; i++ )
		{
			Dress l_dress;
			l_dress.Load( p_buffer );
			m_dresses.insert( std::make_pair( l_dress.m_slot, l_dress ) );
		}

		p_buffer >> l_count;

		std::clog << "ChatTcpClient::SaveDresses - nb tattoos : " << l_count << std::endl;

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
		bool l_return = false;
		m_name = p_buffer.readString();

		if ( m_name != "anonymous" )
		{
			m_anonymous = false;
			m_password = p_buffer.readString();
			l_return = true;
		}
		else
		{
			char l_cname2[256];
			size_t l_written = snprintf( l_cname2, 256, "anonymous%i", m_id );
			m_name.assign( l_cname2, l_written );
			l_return = true;
		}

		return l_return;
	}

	void LoginInformations::Save( WriteBuffer & p_buffer )const
	{
		p_buffer << m_name;
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
		, m_toDelete( false )
	{
		std::clog << "ChatTcpClient()" << std::endl;
	}

	ChatTcpClient::~ChatTcpClient()
	{
		std::clog << "ChatTcpClient - Deleting " << m_login.GetName() << std::endl;
		DoQuitRoom();
		DoQuitGame();

		m_status = ccsNone;

		if ( m_plugin != NULL )
		{
			m_plugin->RemoveClient( this );
			m_plugin = NULL;
		}

		m_messages.clear();
		std::clog << "ChatTcpClient - Client " << m_login.GetName() << " deleted" << std::endl;
	}

	void ChatTcpClient::Stop()
	{
		if ( !m_avatar.IsStopped() )
		{
			m_avatar.Stop();
			m_noSend = true;
			TcpWriterBase::m_socket.close();
			TcpWriterBase::m_service.post( SocketKiller( this ) );
		}
	}

	bool ChatTcpClient::LoadNameAndPass( ReadBuffer & p_buffer )
	{
		bool l_return = false;
		CheckValid( ccsNone );

		if ( m_login.Load( p_buffer ) )
		{
			if ( !m_login.IsAnonymous() )
			{
				l_return = DoDatabaseAction< bool >( std::bind( &ChatDatabase::ConnectUser, DoGetDatabase(), std::ref( m_login ) ) );
			}
		}

		return l_return;
	}

	bool ChatTcpClient::SaveDresses( General::Templates::ReadBuffer & p_buffer )
	{
		CheckValid( ccsNone );
		Clothes l_clothes;
		std::swap( m_clothes, l_clothes );
		m_clothes.Load( p_buffer );
		return DoDatabaseAction< bool >( std::bind( &ChatDatabase::SaveClothes, DoGetDatabase(), std::ref( m_clothes ), GetId() ) );
	}

	String const & ChatTcpClient::GetRoomName()
	{
		CheckValid( ccsInRoom );
		std::shared_ptr< ChatRoom > l_room = m_room.lock();
		String const * l_return = &ChatEmptyString;

		if ( l_room )
		{
			l_return = &l_room->GetName();
		}

		return *l_return;
	}

	void ChatTcpClient::SetRoom( std::shared_ptr< ChatRoom > p_room )
	{
		CheckValid( ccsNone );
		m_room = p_room;
		m_status = ccsInRoom;
	}

	void ChatTcpClient::RemoveRoom()
	{
		CheckValid( ccsInRoom );
		m_room.reset();
		m_status = ccsDressed;
	}

	void ChatTcpClient::RemoveFriend( ChatTcpClient * p_toRemove )
	{
		CheckValid( ccsInRoom );
		RemoveFriend( p_toRemove->GetId() );
	}

	void ChatTcpClient::RemoveIgnored( ChatTcpClient * p_toRemove )
	{
		CheckValid( ccsInRoom );
		RemoveIgnored( p_toRemove->GetId() );
	}

	void ChatTcpClient::RemoveFriend( uint32_t p_toRemove )
	{
		CheckValid( ccsInRoom );
		auto l_it = m_friends.find( p_toRemove );

		if ( l_it != m_friends.end() )
		{
			m_friends.erase( l_it );
		}
	}

	void ChatTcpClient::RemoveIgnored( uint32_t p_toRemove )
	{
		CheckValid( ccsInRoom );
		auto l_it = m_ignored.find( p_toRemove );

		if ( l_it != m_ignored.end() )
		{
			m_ignored.erase( l_it );
			DoDatabaseAction< bool >( std::bind( &ChatDatabase::RemoveIgnored, DoGetDatabase(), m_login.GetId(), p_toRemove ) );
		}
	}

	void ChatTcpClient::AddFriend( String const & p_name, uint32_t p_id )
	{
		CheckValid( ccsInRoom );
		auto l_it = m_friends.find( p_id );

		if ( l_it == m_friends.end() )
		{
			m_friends.insert( std::make_pair( p_id, p_name ) );
		}
	}

	void ChatTcpClient::AddIgnored( String const & p_name, uint32_t p_id )
	{
		CheckValid( ccsInRoom );
		auto l_it = m_ignored.find( p_id );

		if ( l_it == m_ignored.end() )
		{
			m_ignored.insert( std::make_pair( p_id, p_name ) );
		}
	}

	void ChatTcpClient::SendEndGameMessage()
	{
		CheckValid( ccsInGame );
		DoSendNoParamMessage( msEndGame );
		m_game.reset();
	}

	void ChatTcpClient::SendStartGameMessage()
	{
		CheckValid( ccsInGame );
		DoSendNoParamMessage( msGameStart );
	}

	void ChatTcpClient::SendGameAlreadyCreatedMessage()
	{
		CheckValid( ccsNone );
		DoSendNoParamMessage( msGameAlreadyCreated );
	}

	void ChatTcpClient::SendGameDontExistMessage()
	{
		CheckValid( ccsNone );
		DoSendNoParamMessage( msGameDontExist );
	}

	void ChatTcpClient::SendCreateGameOK()
	{
		CheckValid( ccsNone );
		DoSendNoParamMessage( msCreateGameOK );
		m_status = ccsInGame;
	}

	void ChatTcpClient::SendGameJoinMessage( int p_place, String const & p_name, Clothes const & p_clothes )
	{
		CheckValid( ccsNone );
		auto && l_buffer = make_wbuffer( m_messageBuffer );
		l_buffer << msJoinGame;
		l_buffer << p_name;
		l_buffer << p_place;
		p_clothes.Save( l_buffer );
		DoSendBuffer( l_buffer );
	}

	void ChatTcpClient::SendGameQuitMessage( String const & p_name )
	{
		CheckValid( ccsInGame );
		auto && l_buffer = make_wbuffer( m_messageBuffer );
		l_buffer << msQuitGame;
		l_buffer << p_name;
		DoSendBuffer( l_buffer );
		m_status = ccsNone;
	}

	void ChatTcpClient::SendGameJoinOkMessage( int p_place )
	{
		CheckValid( ccsNone );
		DoSendNoParamMessage( msJoinGameOK );
		m_status = ccsInGame;
	}

	uint32_t ChatTcpClient::GetRandom( uint32_t p_max )
	{
		uint32_t l_return = 0;

		if ( p_max )
		{
			l_return = std::uniform_int_distribution< uint32_t >( 0, p_max - 1 )( m_elypseService.lock()->GetRandomDevice() );
		}

		return l_return;
	}

	void ChatTcpClient::CheckValid( int p_comp )
	{
		if ( m_toDelete )
		{
			DoGetService()->EraseClient( this );
			throw std::runtime_error( "The client has been deleted" );
		}
		else if ( m_status < p_comp )
		{
			throw std::runtime_error( "The client is in a wrong state" );
		}
	}

	void ChatTcpClient::DoSendNoParamMessage( MessageSent p_type )
	{
		CheckValid( ccsNone );
		auto && l_buffer = make_wbuffer( m_messageBuffer );
		l_buffer << p_type;
		DoSendBuffer( l_buffer );
	}

	void ChatTcpClient::DoSendBuffer( WriteBuffer const & p_buffer )
	{
		String l_toSend( p_buffer.data(), p_buffer.size() );
		AsyncSend( l_toSend );
	}

	bool ChatTcpClient::DoEndWork()
	{
		bool l_return = true;
		m_avatar.Work( false );

		if ( m_toDelete )
		{
			DoGetService()->EraseClient( this );
			l_return = false;
		}

		return l_return;
	}

	void ChatTcpClient::DoQuitRoom()
	{
		std::shared_ptr< ChatRoom > l_room = m_room.lock();

		if ( m_status == ccsInRoom && !l_room )
		{
			auto && l_buffer = make_wbuffer( m_messageBuffer );
			l_buffer << msQuit;
			m_login.Save( l_buffer );
			DoForwardMessage( l_buffer );
			l_room->RemoveClient( this );
			m_room.reset();
		}
	}

	void ChatTcpClient::DoQuitGame()
	{
		std::shared_ptr< ChatGame > l_game = m_game.lock();

		if ( m_status == ccsInGame && l_game )
		{
			std::cout << m_login.GetName() << " was in a game" << std::endl;
			auto && l_buffer = make_wbuffer( m_messageBuffer );
			l_buffer << msQuitGame;
			m_login.Save( l_buffer );
			DoForwardGameMessage( l_buffer );
			l_game->RemovePlayer( this );

			if ( l_game->GetInitiatorId() == m_login.GetId() )
			{
				std::cout << m_login.GetName() << " was the initiator of the game, ending it" << std::endl;
				m_plugin->RemoveGame( l_game->GetGameName(), m_login.GetId() );
			}
			else if ( l_game->GetPlayersCount() == 0 )
			{
				m_plugin->RemoveGame( l_game->GetGameName(), l_game->GetInitiatorId() );
			}

			m_game.reset();
		}
	}

	bool ChatTcpClient::DoIsIgnored( uint32_t p_id )
	{
		CheckValid( ccsInRoom );
		return m_ignored.find( p_id ) != m_ignored.end();
	}

	String ChatTcpClient::DoBuildJoinMessage()
	{
		CheckValid( ccsNone );
		auto && l_buffer = make_wbuffer( m_messageBuffer );
		l_buffer << msJoin;
		m_login.Save( l_buffer );
		m_clothes.Save( l_buffer );
		m_avatar.Save( l_buffer );
		return String( l_buffer.data(), l_buffer.size() );
	}

	void ChatTcpClient::DoSendDressesMessage()
	{
		std::clog << "ChatTcpClient::DoSendDressesMessage - " << GetName() << " sends his dresses" << std::endl;
		CheckValid( ccsNone );
		auto && l_buffer = make_wbuffer( m_messageBuffer );
		l_buffer << msAvatar;
		m_clothes.Save( l_buffer );
		DoSendBuffer( l_buffer );
		std::clog << "ChatTcpClient::DoSendDressesMessage - " << GetName() << " has ended the sending of his dresses : " << l_buffer.size() << std::endl;
	}

	void ChatTcpClient::DoSendRoomsMessage()
	{
		CheckValid( ccsNone );
		auto && l_buffer = make_wbuffer( m_messageBuffer );
		l_buffer << msRooms;
		m_plugin->GetWorld()->Save( l_buffer );
		DoSendBuffer( l_buffer );
	}

	void ChatTcpClient::DoSendKickMessage()
	{
		CheckValid( ccsNone );
		DoSendNoParamMessage( msKickUser );
	}

	void ChatTcpClient::DoSendGameJoinFail()
	{
		CheckValid( ccsNone );
		DoSendNoParamMessage( msJoinGameFail );
	}

	void ChatTcpClient::DoForwardMessage( WriteBuffer const & p_message )
	{
		CheckValid( ccsInRoom );
		String l_message( p_message.data(), p_message.size() );
		std::shared_ptr< ChatRoom > l_room = m_room.lock();

		if ( l_room )
		{
			l_room->ForwardMessage( l_message, GetId() );
		}
	}

	void ChatTcpClient::DoForwardGameMessage( WriteBuffer const & p_message )
	{
		CheckValid( ccsInRoom );
		String l_message( p_message.data(), p_message.size() );
		std::shared_ptr< ChatGame > l_game = m_game.lock();

		if ( l_game )
		{
			l_game->ForwardMessage( l_message, GetId() );
		}
	}

	void ChatTcpClient::DoForwardTalkMessage( WriteBuffer const & p_message )
	{
		CheckValid( ccsInRoom );
		String l_message( p_message.data(), p_message.size() );
		std::shared_ptr< ChatRoom > l_room = m_room.lock();

		if ( l_room )
		{
			l_room->ForwardMessage( l_message, GetId(), m_ignored );
		}
	}

	void ChatTcpClient::DoProcessConnectMessage( ReadBuffer & p_buffer )
	{
		CheckValid( ccsNone );

		if ( LoadNameAndPass( p_buffer ) )
		{
			CheckValid( ccsNone );

			if ( !IsAnonymous() )
			{
				ChatTcpClient * l_client = m_plugin->GetClient( GetId() );

				if ( l_client )
				{
					std::cout << "ChatTcpClient::DoProcessConnectMessage - " << GetName() << " was already connected" << std::endl;
					l_client->DoSendKickMessage();
					m_plugin->RemoveClient( l_client );
				}

				m_plugin->AddClient( this );

				Clothes l_clothes;
				std::swap( m_clothes, l_clothes );

				if ( !DoDatabaseAction< bool >( std::bind( &ChatDatabase::LoadClothes, DoGetDatabase(), std::ref( m_clothes ), GetId() ) ) )
				{
					throw std::runtime_error( "ChatTcpClient::DoProcessConnectMessage - Can't load dresses for " + GetName() );
				}

				m_status = ccsDressed;

				DoDatabaseAction< bool >( std::bind( &ChatDatabase::LoadFriends, DoGetDatabase(), std::ref( m_friends ), GetId() ) );
				DoDatabaseAction< bool >( std::bind( &ChatDatabase::LoadIgnored, DoGetDatabase(), std::ref( m_ignored ), GetId() ) );
			}
			else
			{
				std::clog << "ChatClient::DoProcessConnectMessage - Anonymous" << std::endl;
				m_plugin->AddClient( this );
				std::clog << "ChatClient::DoProcessConnectMessage - Added" << std::endl;
				uint32_t l_sex = GetRandom( 2 );
				Clothes l_clothes;
				std::swap( m_clothes, l_clothes );
				m_clothes.m_sex = uint8_t( l_sex );
				uint64_t l_slots = DoDatabaseAction< uint64_t >( std::bind( &ChatDatabase::GetDressSlotsCount, DoGetDatabase() ) );

				for ( uint32_t i = 1 ; i <= l_slots ; i++ )
				{
					Dress l_dress;
					l_dress.m_id = DEFAULT_DRESS_ID;
					l_dress.m_slot = ChatClientDressSlot( i );
					m_clothes.m_dresses.insert( std::make_pair( l_dress.m_slot, l_dress ) );
				}

				for ( auto && l_it : m_clothes.m_dresses )
				{
					uint32_t l_size = DoDatabaseAction< uint32_t >( std::bind( &ChatDatabase::GetDressesCount, DoGetDatabase(), l_sex, l_it.first ) );

					if ( l_size > 10 )
					{
						l_size = 10;
					}

					Dress & l_dress = l_it.second;
					l_dress.m_id = DoDatabaseAction< uint32_t >( std::bind( &ChatDatabase::GetDress, DoGetDatabase(), l_sex, l_it.first, GetRandom( l_size ) ) );

					if ( l_it.first == 11 || l_it.first == 13 )
					{
						l_dress.m_materials.insert( std::make_pair( 0, GetRandom( 6 ) ) );
					}
				}
			}

			std::cout << "ChatClient " << GetName() << " connected" << std::endl;
			DoSendNoParamMessage( msConnectOK );
			m_status = ccsConnected;
		}
		else
		{
			CheckValid( ccsNone );
			std::clog << "ChatTcpClient::DoProcessConnectMessage - Not connected" << std::endl;
			DoSendNoParamMessage( msConnectFail );
		}
	}

	void ChatTcpClient::DoProcessDressMessage( ReadBuffer & p_buffer )
	{
		CheckValid( ccsConnected );

		if ( !IsAnonymous() )
		{
			std::clog << "ChatTcpClient::DoProcessDressMessage - not anonymous" << std::endl;
			SaveDresses( p_buffer );
		}
		else
		{
			std::clog << "ChatTcpClient::DoProcessDressMessage - anonymous" << std::endl;
		}

		if ( m_status < ccsInRoom )
		{
			DoSendRoomsMessage();
		}

		m_status = ccsDressed;
	}

	void ChatTcpClient::DoProcessJoinMessage( ReadBuffer & p_buffer )
	{
		CheckValid( ccsDressed );
		int l_size;
		p_buffer >> l_size;
		String l_roomName;
		l_roomName.assign( p_buffer.readArray< char >( l_size ), l_size );

		std::clog << "DoProcessJoinMessage - " << l_roomName << std::endl;
		std::shared_ptr< ChatRoom > l_room = m_plugin->GetRoom( l_roomName );
		String l_joinMessage = DoBuildJoinMessage();

		if ( l_room )
		{
			for ( auto && l_it : l_room->GetClients() )
			{
				AsyncSend( l_it.second->DoBuildJoinMessage() );
				l_it.second->AsyncSend( l_joinMessage );
			}

			l_room->AddClient( this );
			m_status = ccsInRoom;
		}
		else
		{
			std::cout << "NULL Room" << std::endl;
		}
	}

	void ChatTcpClient::DoProcessQuitMessage()
	{
		CheckValid( ccsInRoom );
		std::shared_ptr< ChatRoom > l_room = m_room.lock();

		if ( l_room )
		{
			std::clog << "Sending quit to room clients" << std::endl;
			auto && l_buffer = make_wbuffer( m_messageBuffer );
			l_buffer << msQuit;
			m_login.Save( l_buffer );
			DoForwardMessage( l_buffer );
			l_room->RemoveClient( this );
		}
	}

	void ChatTcpClient::DoProcessUpdateMessage( ReadBuffer & p_buffer )
	{
		CheckValid( ccsInRoom );
		p_buffer >> m_avatar.GetPosition().x >> m_avatar.GetPosition().y >> m_avatar.GetPosition().z >> m_avatar.GetYaw();

		auto && l_buffer = make_wbuffer( m_messageBuffer );
		l_buffer << msUpdate;
		m_login.Save( l_buffer );
		l_buffer << GetPosition().x << GetPosition().y << GetPosition().z << GetYaw();
		DoForwardMessage( l_buffer );
	}

	void ChatTcpClient::DoProcessWalkMessage( ReadBuffer & p_buffer )
	{
		CheckValid( ccsInRoom );
		int l_running;
		int l_backward;
		p_buffer >> m_avatar.GetPosition().x >> m_avatar.GetPosition().y >> m_avatar.GetPosition().z >> l_backward >> l_running;

		auto && l_buffer = make_wbuffer( m_messageBuffer );
		l_buffer << msWalk;
		m_login.Save( l_buffer );
		l_buffer << GetPosition().x << GetPosition().y << GetPosition().z << l_backward << l_running;
		DoForwardMessage( l_buffer );
	}

	void ChatTcpClient::DoProcessTurnMessage( ReadBuffer & p_buffer )
	{
		CheckValid( ccsInRoom );
		p_buffer >> m_avatar.GetYaw();

		auto && l_buffer = make_wbuffer( m_messageBuffer );
		l_buffer << msTurn;
		m_login.Save( l_buffer );
		l_buffer << GetYaw();
		DoForwardMessage( l_buffer );
	}

	void ChatTcpClient::DoProcessTalkMessage( ReadBuffer & p_buffer )
	{
		CheckValid( ccsInRoom );
		String l_message = p_buffer.readString();
		std::cout << GetName() << " : " << l_message << std::endl;

		auto && l_buffer = make_wbuffer( m_messageBuffer );
		l_buffer << msTalk;
		m_login.Save( l_buffer );
		l_buffer << l_message;
		DoForwardMessage( l_buffer );
	}

	void ChatTcpClient::DoProcessWhispMessage( ReadBuffer & p_buffer )
	{
		CheckValid( ccsInRoom );

		if ( IsAnonymous() )
		{
			DoSendNoParamMessage( msUnavailableToAnonymous );
		}
		else
		{
			String l_message = p_buffer.readString();
			String l_distantName;
			size_t l_index = l_message.find_first_of( " " );
			l_distantName = l_message.substr( 0, l_index );
			l_message = l_message.substr( l_index + 1 );
			std::cout << "DoProcessWhispMessage - " << l_distantName << " - " << l_message << std::endl;

			if ( l_distantName != GetName() )
			{
				ChatTcpClient * l_client = m_plugin->GetWorld()->GetClient( l_distantName );

				if ( l_client )
				{
					auto && l_buffer = make_wbuffer( m_messageBuffer );
					l_buffer << msWhisp;
					m_login.Save( l_buffer );
					l_buffer << l_message;
					String l_toSend;
					l_toSend.assign( l_buffer.data(), l_buffer.size() );

					if ( !l_client->DoIsIgnored( GetId() ) )
					{
						l_client->AsyncSend( l_toSend );
					}

					DoSendNoParamMessage( msWhispOK );
				}
				else
				{
					std::cout << "WhispFail" << std::endl;
					DoSendNoParamMessage( msWhispFail );
				}
			}
			else
			{
				std::cout << "WhispFail" << std::endl;
				DoSendNoParamMessage( msWhispFail );
			}
		}
	}

	void ChatTcpClient::DoProcessBeginWalkMessage()
	{
		CheckValid( ccsInRoom );
		m_avatar.Sit( false );
		auto && l_buffer = make_wbuffer( m_messageBuffer );
		l_buffer << msBeginWalk;
		m_login.Save( l_buffer );
		DoForwardMessage( l_buffer );
	}

	void ChatTcpClient::DoProcessEndWalkMessage( ReadBuffer & p_buffer )
	{
		CheckValid( ccsInRoom );
		float l_x, l_y, l_z, l_yaw;
		p_buffer >> l_x >> l_y >> l_z >> l_yaw;

		auto && l_buffer = make_wbuffer( m_messageBuffer );
		l_buffer << msEndWalk;
		m_login.Save( l_buffer );
		l_buffer << l_x << l_y << l_z << l_yaw;
		DoForwardMessage( l_buffer );
	}

	void ChatTcpClient::DoProcessBeginRunMessage()
	{
		CheckValid( ccsInRoom );
		m_avatar.Sit( false );
		auto && l_buffer = make_wbuffer( m_messageBuffer );
		l_buffer << msBeginRun;
		m_login.Save( l_buffer );
		DoForwardMessage( l_buffer );
	}

	void ChatTcpClient::DoProcessEndRunMessage( General::Templates::ReadBuffer & p_buffer )
	{
		CheckValid( ccsInRoom );
		float l_x, l_y, l_z, l_yaw;
		int l_stillWalking;
		p_buffer >> l_stillWalking >> l_x >> l_y >> l_z >> l_yaw;

		auto && l_buffer = make_wbuffer( m_messageBuffer );
		l_buffer << msEndRun;
		m_login.Save( l_buffer );
		l_buffer << l_x << l_y << l_z << l_yaw;
		DoForwardMessage( l_buffer );
	}

	void ChatTcpClient::DoProcessEmoteMessage( ReadBuffer & p_buffer )
	{
		CheckValid( ccsInRoom );
		String l_message = p_buffer.readString();
		std::cout << "DoProcessEmoteMessage - " << l_message << std::endl;

		auto && l_buffer = make_wbuffer( m_messageBuffer );
		l_buffer << msEmote;
		m_login.Save( l_buffer );
		l_buffer << l_message;
		DoForwardMessage( l_buffer );
	}

	void ChatTcpClient::DoProcessRemoveFriendMessage( ReadBuffer & p_buffer )
	{
		CheckValid( ccsInRoom );

		if ( IsAnonymous() )
		{
			DoSendNoParamMessage( msUnavailableToAnonymous );
		}
		else
		{
			String l_toRemove = p_buffer.readString();
			uint32_t l_friendId = DoDatabaseAction< uint32_t >( std::bind( &ChatDatabase::GetClientId, DoGetDatabase(), std::ref( l_toRemove ) ) );

			if ( l_friendId != 0 )
			{
				DoDatabaseAction< bool >( std::bind( &ChatDatabase::RemoveFriend, DoGetDatabase(), m_login.GetId(), l_friendId ) );
				DoDatabaseAction< bool >( std::bind( &ChatDatabase::RemoveFriend, DoGetDatabase(), l_friendId, m_login.GetId() ) );
				ChatTcpClient * l_friend = m_plugin->GetClient( l_friendId );

				if ( l_friend )
				{
					l_friend->RemoveFriend( GetId() );
				}

				auto && l_it = m_friends.find( l_friendId );

				if ( l_it != m_friends.end() )
				{
					m_friends.erase( l_it );
				}
			}
		}
	}

	void ChatTcpClient::DoProcessRemoveIgnoredMessage( ReadBuffer & p_buffer )
	{
		CheckValid( ccsInRoom );

		if ( IsAnonymous() )
		{
			DoSendNoParamMessage( msUnavailableToAnonymous );
		}
		else
		{
			RemoveIgnored( DoGetDatabase()->GetClientId( p_buffer.readString() ) );
		}
	}

	void ChatTcpClient::DoProcessNewFriendMessage( ReadBuffer & p_buffer )
	{
		CheckValid( ccsInRoom );

		if ( IsAnonymous() )
		{
			DoSendNoParamMessage( msUnavailableToAnonymous );
		}
		else
		{
			String l_name = p_buffer.readString();
			std::clog << "DoProcessNewFriendMessage - " << l_name << std::endl;

			if ( l_name != GetName() )
			{
				uint32_t l_id = DoDatabaseAction< uint32_t >( std::bind( &ChatDatabase::GetClientId, DoGetDatabase(), std::ref( l_name ) ) );
				ChatTcpClient * l_newFriend = m_plugin->GetClient( l_id );
				auto && l_buffer = make_wbuffer( m_messageBuffer );

				if ( !l_newFriend )
				{
					std::cout << "DoProcessNewFriendMessage - Friend not found" << std::endl;
					l_buffer << msNewFriendDoesntExist;
					l_buffer << l_name;
					DoSendBuffer( l_buffer );
				}
				else if ( m_friends.find( l_id ) != m_friends.end() )
				{
					std::cout << "DoProcessNewFriendMessage - Friend found, already in friends" << std::endl;
					l_buffer << msNewFriendAlreadyFriend;
					l_buffer << l_name;
					DoSendBuffer( l_buffer );
				}
				else
				{
					std::clog << "DoProcessNewFriendMessage - Friend found" << std::endl;
					l_buffer << msNewFriendAsk;
					m_login.Save( l_buffer );
					DoSendBuffer( l_buffer );
				}
			}
		}
	}

	void ChatTcpClient::DoProcessNewIgnoredMessage( ReadBuffer & p_buffer )
	{
		CheckValid( ccsInRoom );

		if ( IsAnonymous() )
		{
			DoSendNoParamMessage( msUnavailableToAnonymous );
		}
		else
		{
			String l_name = p_buffer.readString();
			std::clog << "DoProcessNewIgnoredMessage - " << l_name << std::endl;

			if ( l_name != GetName() )
			{
				uint32_t l_id = DoDatabaseAction< uint32_t >( std::bind( &ChatDatabase::GetClientId, DoGetDatabase(), std::ref( l_name ) ) );
				auto && l_buffer = make_wbuffer( m_messageBuffer );

				if ( !l_id )
				{
					l_buffer << msNewIgnoredDoesntExist;
					l_buffer << l_name;
					DoSendBuffer( l_buffer );
				}
				else if ( m_ignored.find( l_id ) != m_ignored.end() )
				{
					l_buffer << msNewIgnoredAlreadyIgnored;
					l_buffer << l_name;
					DoSendBuffer( l_buffer );
				}
				else
				{
					m_ignored.insert( std::make_pair( l_id, l_name ) );
					DoDatabaseAction< uint32_t >( std::bind( &ChatDatabase::AddIgnored, DoGetDatabase(), GetId(), l_id ) );
					l_buffer << msNewIgnoredDone;
					l_buffer << l_name;
					DoSendBuffer( l_buffer );
				}
			}
		}
	}

	void ChatTcpClient::DoProcessUpdateFriendsMessage()
	{
		CheckValid( ccsInRoom );

		if ( IsAnonymous() )
		{
			DoSendNoParamMessage( msUnavailableToAnonymous );
		}
		else
		{
			String l_noRoom = "Non connecte";

			auto && l_buffer = make_wbuffer( m_messageBuffer );
			l_buffer << msUpdateFriendsList;
			l_buffer << uint32_t( m_friends.size() );

			for ( auto && l_it : m_friends )
			{
				ChatTcpClient * l_friend = m_plugin->GetClient( l_it.first );
				l_buffer << l_friend->GetName();

				if ( l_friend && l_friend->GetStatus() == ccsInRoom )
				{
					l_buffer << l_friend->GetRoomName();
				}
				else
				{
					l_buffer << l_noRoom;
				}
			}

			DoSendBuffer( l_buffer );
		}
	}

	void ChatTcpClient::DoProcessUpdateIgnoredMessage()
	{
		CheckValid( ccsInRoom );

		if ( IsAnonymous() )
		{
			DoSendNoParamMessage( msUnavailableToAnonymous );
		}
		else
		{
			auto && l_buffer = make_wbuffer( m_messageBuffer );
			l_buffer << msUpdateIgnoredList;
			l_buffer << uint32_t( m_ignored.size() );

			for ( auto && l_it : m_ignored )
			{
				ChatTcpClient * l_ignored = m_plugin->GetClient( l_it.first );
				l_buffer << l_ignored->GetName();

				if ( l_ignored && l_ignored->GetStatus() == ccsInRoom )
				{
					l_buffer << '1';
				}
				else
				{
					l_buffer << '0';
				}
			}

			DoSendBuffer( l_buffer );
		}
	}

	void ChatTcpClient::DoProcessFriendRefuseMessage( ReadBuffer & p_buffer )
	{
		CheckValid( ccsInRoom );

		if ( IsAnonymous() )
		{
			DoSendNoParamMessage( msUnavailableToAnonymous );
		}
		else
		{
			String l_refusedName = p_buffer.readString();
			uint32_t l_id = DoDatabaseAction< uint32_t >( std::bind( &ChatDatabase::GetClientId, DoGetDatabase(), std::ref( l_refusedName ) ) );
			ChatTcpClient * l_refused = m_plugin->GetClient( l_id );

			if ( l_refused )
			{
				auto && l_buffer = make_wbuffer( m_messageBuffer );
				l_buffer << msNewFriendRefuse;
				m_login.Save( l_buffer );
				DoSendBuffer( l_buffer );
			}
		}
	}

	void ChatTcpClient::DoProcessFriendAcceptMessage( ReadBuffer & p_buffer )
	{
		CheckValid( ccsInRoom );

		if ( IsAnonymous() )
		{
			DoSendNoParamMessage( msUnavailableToAnonymous );
		}
		else
		{
			String l_acceptedName = p_buffer.readString();
			uint32_t l_id = DoDatabaseAction< uint32_t >( std::bind( &ChatDatabase::GetClientId, DoGetDatabase(), std::ref( l_acceptedName ) ) );
			ChatTcpClient * l_accepted = m_plugin->GetClient( l_id );

			if ( l_accepted )
			{
				auto && l_buffer = make_wbuffer( m_messageBuffer );
				l_buffer << msNewFriendAccept;
				m_login.Save( l_buffer );

				l_accepted->m_friends.insert( std::make_pair( m_login.GetId(), GetName() ) );
				m_friends.insert( std::make_pair( l_accepted->GetId(), l_acceptedName ) );
				DoDatabaseAction< uint32_t >( std::bind( &ChatDatabase::AddFriend, DoGetDatabase(), l_accepted->GetId(), m_login.GetId() ) );
				DoDatabaseAction< uint32_t >( std::bind( &ChatDatabase::AddFriend, DoGetDatabase(), m_login.GetId(), l_accepted->GetId() ) );
			}
		}
	}

	void ChatTcpClient::DoProcessRefreshGamesListMessage( ReadBuffer & p_buffer )
	{
		CheckValid( ccsConnected );
		String l_name = p_buffer.readString();
		StrUIntIdMap l_map = m_plugin->GetGamesList( l_name );
		auto && l_buffer = make_wbuffer( m_messageBuffer );
		l_buffer << msRefreshGamesList;
		l_buffer << static_cast< int >( l_map.size() );

		for ( StrUIntIdMap::iterator l_it = l_map.begin() ; l_it != l_map.end() ; ++l_it )
		{
			l_buffer << l_it->first;//id creator
			l_buffer << l_it->second.second;//nb players
			l_buffer << l_it->second.first; //creator name
		}

		DoSendBuffer( l_buffer );
	}

	void ChatTcpClient::DoProcessJoinGameMessage( ReadBuffer & p_buffer )
	{
		CheckValid( ccsNone );

		if ( IsAnonymous() )
		{
			DoSendNoParamMessage( msUnavailableToAnonymous );
		}
		else
		{
			if ( m_status != ccsConnected )
			{
				DoSendGameJoinFail();
			}
			else
			{
				String l_name = p_buffer.readString();
				int l_id;
				p_buffer >> l_id;
				std::shared_ptr< ChatGame > l_game = m_plugin->GetGame( l_name, l_id );

				if ( l_game || !l_game->AddPlayer( this ) )
				{
					DoSendGameJoinFail();
				}
				else
				{
					int l_place = l_game->GetPlayerPlace( GetName() );
					SendGameJoinOkMessage( l_place );
					auto && l_myIt = l_game->GetPlayers().find( m_login.GetId() );

					for ( auto && l_it : l_game->GetPlayers() )
					{
						ChatTcpClient * l_player = l_it.second;

						if ( l_player && l_player->GetId() != GetId() && !l_player->IsToDelete() )
						{
							l_player->SendGameJoinMessage( l_place, GetName(), m_clothes );
							SendGameJoinMessage( l_game->GetPlayerPlace( l_player->GetName() ), l_player->GetName(), l_player->GetClothes() );
						}
					}
				}

				m_game = l_game;
				m_status = ccsInGame;
			}
		}
	}

	void ChatTcpClient::DoProcessQuitGameMessage()
	{
		CheckValid( ccsInGame );
		std::shared_ptr< ChatGame > l_game = m_game.lock();

		if ( l_game )
		{
			std::clog << "Sending quit to game players" << std::endl;
			auto && l_buffer = make_wbuffer( m_messageBuffer );
			l_buffer << msQuitGame;
			m_login.Save( l_buffer );
			DoForwardGameMessage( l_buffer );
			l_game->RemovePlayer( this );

			if ( l_game->GetInitiatorId() == m_login.GetId() )
			{
				std::clog << GetName() << " was the initiator of the game, deleting it" << std::endl;
				m_plugin->RemoveGame( l_game->GetGameName(), m_login.GetId() );
			}
			else if ( l_game->GetPlayersCount() == 0 )
			{
				m_plugin->RemoveGame( l_game->GetGameName(), l_game->GetInitiatorId() );
			}

			m_game.reset();
		}
	}

	void ChatTcpClient::DoProcessGameMessage( ReadBuffer & p_buffer )
	{
		CheckValid( ccsInGame );
		std::shared_ptr< ChatGame > l_game = m_game.lock();

		if ( l_game )
		{
			auto && l_buffer = make_wbuffer( m_messageBuffer );
			l_buffer << msGame;
			m_login.Save( l_buffer );
			l_buffer.writeArray< char >( p_buffer.data() + sizeof( MessageReceived ), p_buffer.left() );
			DoForwardGameMessage( l_buffer );
		}
	}

	void ChatTcpClient::DoProcessCreateGameMessage( ReadBuffer & p_buffer )
	{
		CheckValid( ccsNone );

		if ( IsAnonymous() )
		{
			DoSendNoParamMessage( msUnavailableToAnonymous );
		}
		else
		{
			if ( m_status != ccsConnected )
			{
				SendGameDontExistMessage();
			}
			else
			{
				String l_name = p_buffer.readString();
				m_game = m_plugin->AddGame( l_name, this );
				m_status = ccsInGame;
			}
		}
	}

	void ChatTcpClient::DoProcessSitDownMessage()
	{
		CheckValid( ccsInRoom );
		m_avatar.Sit();

		auto && l_buffer = make_wbuffer( m_messageBuffer );
		l_buffer << msSitDown;
		m_login.Save( l_buffer );
		DoForwardMessage( l_buffer );
	}

	void ChatTcpClient::DoProcessSitUpMessage()
	{
		CheckValid( ccsInRoom );
		m_avatar.Sit( false );

		auto && l_buffer = make_wbuffer( m_messageBuffer );
		l_buffer << msSitUp;
		m_login.Save( l_buffer );
		DoForwardMessage( l_buffer );
	}

	void ChatTcpClient::DoProcessGameInfoMessage( ReadBuffer & p_buffer )
	{
		CheckValid( ccsNone );
		auto && l_buffer = make_wbuffer( m_messageBuffer );
		l_buffer << msGameInfo << static_cast< int >( m_plugin->GetGameMaxPLayers( p_buffer.readString() ) );
		DoSendBuffer( l_buffer );
	}

	void ChatTcpClient::DoProcessGameStartMessage()
	{
		CheckValid( ccsInGame );
		std::shared_ptr< ChatGame > l_game = m_game.lock();

		if ( l_game )
		{
			l_game->StartGame();
		}
	}

	void ChatTcpClient::CallbackReceivedMessage( String const & p_message )
	{
		try
		{
			CheckValid( ccsNone );
			String l_message = p_message;
			ReadBuffer l_buffer( l_message.data(), uint32_t( p_message.size() ) );
			int l_header;
			l_buffer >> l_header;
			std::clog << GetName() << " Received a message : " << l_header << std::endl;

			switch ( l_header )
			{
			case mrConnect:
				if ( m_status < ccsConnected )
				{
					std::clog << GetName() << " Received message Connect" << std::endl;
					DoProcessConnectMessage( l_buffer );
				}

				break;

			case mrAvatar:
				std::cout << GetName() << " Received message Avatar" << std::endl;
				DoProcessDressMessage( l_buffer );
				break;

			case mrJoin:
				if ( m_status < ccsInRoom )
				{
					std::cout << GetName() << " Received message Join" << std::endl;
					DoProcessJoinMessage( l_buffer );
				}

				break;

			case mrQuit:
				if ( m_status == ccsInRoom )
				{
					std::cout << GetName() << " Received message Quit" << std::endl;
					DoProcessQuitMessage();
				}

				break;

			case mrUpdateRooms:
				std::cout << GetName() << " Received message UpdateRooms" << std::endl;
				DoSendRoomsMessage();
				break;

			case mrUpdate:
				std::clog << "Received message Update" << std::endl;
				DoProcessUpdateMessage( l_buffer );
				break;

			case mrWalk:
				std::clog << "Received message Walk" << std::endl;
				DoProcessWalkMessage( l_buffer );
				break;

			case mrTurn:
				std::clog << "Received message Turn" << std::endl;
				DoProcessTurnMessage( l_buffer );
				break;

			case mrTalk:
				std::clog << "Received message Talk" << std::endl;
				DoProcessTalkMessage( l_buffer );
				break;

			case mrWhisp:
				std::clog << "Received message Whisp" << std::endl;
				DoProcessWhispMessage( l_buffer );
				break;

			case mrBeginWalk:
				std::clog << "Received message BeginWalk" << std::endl;
				DoProcessBeginWalkMessage();
				break;

			case mrEndWalk:
				std::clog << "Received message EndWalk" << std::endl;
				DoProcessEndWalkMessage( l_buffer );
				break;

			case mrBeginRun:
				std::clog << "Received message BeginRun" << std::endl;
				DoProcessBeginRunMessage();
				break;

			case mrEndRun:
				std::clog << "Received message EndRun" << std::endl;
				DoProcessEndRunMessage( l_buffer );
				break;

			case mrEmote:
				std::cout << GetName() << " Received message Emote" << std::endl;
				DoProcessEmoteMessage( l_buffer );
				break;

			case mrRemoveFriend:
				std::cout << GetName() << " Received message RemoveFriend" << std::endl;
				DoProcessRemoveFriendMessage( l_buffer );
				break;

			case mrRemoveIgnored:
				std::cout << GetName() << " Received message RemoveIgnored" << std::endl;
				DoProcessRemoveIgnoredMessage( l_buffer );
				break;

			case mrUpdateFriendsList:
				std::cout << GetName() << " Received message UpdateFriendsList" << std::endl;
				DoProcessUpdateFriendsMessage();
				break;

			case mrUpdateIgnoredList:
				std::cout << GetName() << " Received message UpdateIgnoredList" << std::endl;
				DoProcessUpdateIgnoredMessage();
				break;

			case mrNewFriend:
				std::cout << GetName() << " Received message NewFriend" << std::endl;
				DoProcessNewFriendMessage( l_buffer );
				break;

			case mrNewIgnored:
				std::cout << GetName() << " Received message NewIgnored" << std::endl;
				DoProcessNewIgnoredMessage( l_buffer );
				break;

			case mrNewFriendAccept:
				std::cout << GetName() << " Received message NewFriendAccept" << std::endl;
				DoProcessFriendAcceptMessage( l_buffer );
				break;

			case mrNewFriendRefuse:
				std::cout << GetName() << " Received message NewFriendRefuse" << std::endl;
				DoProcessFriendRefuseMessage( l_buffer );
				break;

			case mrRefreshGamesList:
				std::cout << GetName() << " Received message RefreshGamesList" << std::endl;
				DoProcessRefreshGamesListMessage( l_buffer );
				break;

			case mrJoinGame:
				std::cout << GetName() << " Received message JoinGame" << std::endl;
				DoProcessJoinGameMessage( l_buffer );
				break;

			case mrCreateGame:
				std::cout << GetName() << " Received message CreateGame" << std::endl;
				DoProcessCreateGameMessage( l_buffer );
				break;

			case mrQuitGame:
				std::cout << GetName() << " Received message QuitGame" << std::endl;
				DoProcessQuitGameMessage();
				break;

			case mrGame:
				std::clog << GetName() << " Received message Game" << std::endl;
				DoProcessGameMessage( l_buffer );
				break;

			case mrSitDown:
				std::cout << GetName() << " Received message Sitdown" << std::endl;
				DoProcessSitDownMessage();
				break;

			case mrSitUp:
				std::cout << GetName() << " Received message Situp" << std::endl;
				DoProcessSitUpMessage();
				break;

			case mrGameInfo:
				std::cout << GetName() << " Received message Game info" << std::endl;
				DoProcessGameInfoMessage( l_buffer );
				break;

			case mrGameStart:
				std::cout << GetName() << " Received message Game start" << std::endl;
				DoProcessGameStartMessage();
				break;

			case mrRequestDresses:
				std::cout << GetName() << " Received message Request dresses" << std::endl;
				DoSendDressesMessage();
				break;
			}

			CheckValid( ccsNone );
		}
		catch ( std::exception & exc )
		{
			std::cerr << exc.what() << std::endl;
		}
	}

	bool ChatTcpClient::CallbackReaderError( boost::system::error_code const & p_err )
	{
		TcpReaderBase::m_service.post( SocketKiller( this ) );
		return true;
	}

	bool ChatTcpClient::CallbackWriterError( boost::system::error_code const & p_err )
	{
		Stop();
		return true;
	}

	bool ChatTcpClient::CallbackConnectorError( boost::system::error_code const & p_err )
	{
		Stop();
		return true;
	}
}
