#ifndef ___ChatClient___
#define ___ChatClient___

#include "Common.h"
#include <ElypsePlugin.h>
#include <ElypseClient.h>
#include <ElypseService.h>

using namespace Elypse::ServerPlugin;

#define MAX_HEADER_LENGTH 3
#define MAX_MSG_LENGTH 4192

namespace Chat
{
	class ChatTcpClient : public ElypseTcpClient
	{
	private:
		static unsigned int sm_clientsNumber;

		enum
		{
			c_headerLength = sizeof( short ),
			c_maxMessageLength = 256
		};

	private:
		ChatPlugin * m_plugin;
		ChatRoom * m_room;
		ChatClientStatus m_status;
		ChatDatabase * m_database;
		String m_name;
		String m_password;
		Vector3 m_position;
		float m_yaw;
		Clothes * m_clothes;
		unsigned int m_id;
		char m_messageBuffer[MAX_MSG_LENGTH];
		String m_toSend;
		ClientIdStrMap m_friends;
		ClientIdStrMap m_ignored;
		ChatGame * m_game;
		bool m_toDelete;
		bool m_working;
		bool m_anonymous;
		bool m_sat;

		bool m_stopped;

	public:
		ChatTcpClient( ElypseTcpService * p_elypseService, ChatPlugin * p_plugin );
		virtual ~ChatTcpClient();

//		virtual bool _checkOk(){ if (m_toDelete) { Stop(); } return m_toDelete; }
		void Stop();
		void Start()
		{
//			std::cout << "ChatTcpClient::Start()" << std::endl;
			StartAsyncRead();
		}

		bool LoadNameAndPass( General::Templates::ReadBuffer & p_buffer );
//		bool LoadDresses( Clothes * p_clothes);
		bool SaveDresses( General::Templates::ReadBuffer & p_buffer );

		void SetRoom( ChatRoom * p_room );
		void RemoveRoom();

		void RemoveFriend( ChatTcpClient * p_toRemove );
		void RemoveIgnored( ChatTcpClient * p_toRemove );
		void RemoveFriend( const String & p_toRemove );
		void RemoveIgnored( const String & p_toRemove );

		void AddFriend( const String & p_name, unsigned int p_toAdd );
		void AddIgnored( const String & p_name, unsigned int p_toAdd );

		const String &	GetRoomName();

		void SendEndGameMessage();
		void SendStartGameMessage();
		void SendGameAlreadyCreatedMessage();
		void SendGameDontExistMessage();
		void SendCreateGameOK();
		void SendGameJoinMessage( int p_place, const String & p_name, Clothes * p_clothes );
		void SendGameQuitMessage( const String & p_name );
		void SendGameJoinOkMessage( int p_place );
		void SendGameMessage( General::Templates::ReadBuffer & p_buffer );
		void SendGameMessage( General::Templates::WriteBuffer & p_buffer );
		size_t GetRandom( size_t p_max );
		bool CheckValid( int p_comp );

	public:
		inline const String &	GetName()const
		{
			return m_name;
		}
		inline const String &	GetPass()const
		{
			return m_password;
		}
		inline const Vector3 &	GetPosition()const
		{
			return m_position;
		}
		inline float			GetYaw()const
		{
			return m_yaw;
		}
		inline unsigned int		GetId()const
		{
			return m_id;
		}
		inline ChatClientStatus GetStatus()const
		{
			return m_status;
		}
		inline Clothes 	*	GetClothes()const
		{
			return m_clothes;
		}
		inline bool				IsToDelete()const
		{
			return m_toDelete;
		}

		inline void SetStatus( ChatClientStatus p_status )
		{
			m_status = p_status;
		}
		inline void SetId( unsigned int p_id )
		{
			m_id = p_id;
		}

	private:
		bool _endWork();
		bool _isIgnored( const String & p_name );
		String _buildJoinMessage();
		void _sendDressesMessage();
		void _sendRoomsMessage();
		void _sendKickMessage();
		void _sendGameJoinFail();
		void _forwardMessage( const String & p_message );
		void _forwardGameMessage( const String & p_message );
		void _forwardTalkMessage( const String & p_message );
		void _processConnectMessage( General::Templates::ReadBuffer & p_buffer );
		void _processDressMessage( General::Templates::ReadBuffer & p_buffer );
		void _processJoinMessage( General::Templates::ReadBuffer & p_buffer );
		void _processQuitMessage();
		void _processUpdateMessage( General::Templates::ReadBuffer & p_buffer );
		void _processWalkMessage( General::Templates::ReadBuffer & p_buffer );
		void _processWhispMessage( General::Templates::ReadBuffer & p_buffer );
		void _processTurnMessage( General::Templates::ReadBuffer & p_buffer );
		void _processTalkMessage( General::Templates::ReadBuffer & p_buffer );
		void _processBeginWalkMessage();
		void _processEndWalkMessage( General::Templates::ReadBuffer & p_buffer );
		void _processBeginRunMessage();
		void _processEndRunMessage( General::Templates::ReadBuffer & p_buffer );
		void _processEmoteMessage( General::Templates::ReadBuffer & p_buffer );
		void _processRemoveFriendMessage( General::Templates::ReadBuffer & p_buffer );
		void _processRemoveIgnoredMessage( General::Templates::ReadBuffer & p_buffer );
		void _processNewFriendMessage( General::Templates::ReadBuffer & p_buffer );
		void _processNewIgnoredMessage( General::Templates::ReadBuffer & p_buffer );
		void _processUpdateFriendsMessage();
		void _processUpdateIgnoredMessage();
		void _processFriendRefuseMessage( General::Templates::ReadBuffer & p_buffer );
		void _processFriendAcceptMessage( General::Templates::ReadBuffer & p_buffer );
		void _processRefreshGamesListMessage( General::Templates::ReadBuffer & p_buffer );
		void _processJoinGameMessage( General::Templates::ReadBuffer & p_buffer );
		void _processCreateGameMessage( General::Templates::ReadBuffer & p_buffer );
		void _processGameMessage( General::Templates::ReadBuffer & p_buffer );
		void _processGameInfoMessage( General::Templates::ReadBuffer & p_buffer );
		void _processGameStartMessage();
		void _processQuitGameMessage();
		void _processSitDownMessage();
		void _processSitUpMessage();
		virtual void _receivedMessageCallback( const String & p_message );
		virtual bool _readerErrorCB( const boost::system::error_code & p_err );
		virtual bool _writerErrorCB( const boost::system::error_code & p_err );
		virtual bool _connectorErrorCB( const boost::system::error_code & p_err );

	};
}

#endif
