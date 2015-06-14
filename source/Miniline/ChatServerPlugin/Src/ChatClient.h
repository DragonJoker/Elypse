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
#ifndef ___ChatClient___
#define ___ChatClient___

#include "Common.h"

#include <ElypsePlugin.h>
#include <ElypseClient.h>
#include <ElypseService.h>

#define MAX_HEADER_LENGTH 3
#define MAX_MSG_LENGTH 4192

namespace Chat
{
	class LoginInformations
	{
	public:
		LoginInformations( uint32_t p_id );
		bool Load( General::Templates::ReadBuffer & p_buffer );
		void Save( General::Templates::WriteBuffer & p_buffer )const;

		inline String const & GetName()const
		{
			return m_name;
		}

		inline String const & GetPass()const
		{
			return m_password;
		}

		inline bool IsAnonymous()const
		{
			return m_anonymous;
		}

		inline uint32_t GetId()const
		{
			return m_id;
		}

		inline void SetId( uint32_t p_id )
		{
			m_id = p_id;
		}

	private:
		uint32_t m_id;
		bool m_anonymous = true;
		String m_name;
		String m_password;
	};

	struct Avatar
	{
	public:
		bool Load( General::Templates::ReadBuffer & p_buffer );
		void Save( General::Templates::WriteBuffer & p_buffer )const;

		inline Vector3 const & GetPosition()const
		{
			return m_position;
		}

		inline float GetYaw()const
		{
			return m_yaw;
		}

		inline Vector3 & GetPosition()
		{
			return m_position;
		}

		inline float & GetYaw()
		{
			return m_yaw;
		}

		inline bool IsSat()const
		{
			return m_sat;
		}

		inline bool IsStopped()const
		{
			return m_stopped;
		}

		inline bool IsWorking()const
		{
			return m_working;
		}

		inline void Stop( bool p_stop = true )
		{
			m_stopped = p_stop;
		}

		inline void Work( bool p_work = true )
		{
			m_working = p_work;
		}

		inline void Sit( bool p_sit = true )
		{
			m_sat = p_sit;
		}

	private:
		Vector3 m_position;
		float m_yaw = 0;
		bool m_sat = false;
		bool m_stopped = false;
		bool m_working = false;
	};

	class ChatTcpClient
		: public Elypse::Server::ElypseTcpClient
	{
	private:
		enum
		{
			c_headerLength = sizeof( short ),
			c_maxMessageLength = 256
		};

	private:
		ChatPlugin * m_plugin;
		std::weak_ptr< ChatRoom > m_room;
		ChatClientStatus m_status;
		std::weak_ptr< ChatDatabase > m_database;
		std::weak_ptr< ChatGame > m_game;

		LoginInformations m_login;
		Avatar m_avatar;
		Clothes m_clothes;

		char m_messageBuffer[MAX_MSG_LENGTH];
		String m_toSend;
		ClientIdStrMap m_friends;
		ClientIdStrMap m_ignored;

		bool m_toDelete;

	public:
		ChatTcpClient( std::shared_ptr< Elypse::Server::ElypseTcpService > p_elypseService, ChatPlugin * p_plugin );
		virtual ~ChatTcpClient();

		void Stop();
		void Start()
		{
			StartAsyncRead();
		}

		bool LoadNameAndPass( General::Templates::ReadBuffer & p_buffer );
		bool SaveDresses( General::Templates::ReadBuffer & p_buffer );

		void SetRoom( std::shared_ptr< ChatRoom > p_room );
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
		void SendGameJoinMessage( int p_place, const String & p_name, Clothes const & p_clothes );
		void SendGameQuitMessage( const String & p_name );
		void SendGameJoinOkMessage( int p_place );
		void SendGameMessage( General::Templates::ReadBuffer & p_buffer );
		void SendGameMessage( General::Templates::WriteBuffer & p_buffer );
		size_t GetRandom( size_t p_max );
		bool CheckValid( int p_comp );

	public:
		inline const LoginInformations & GetLogin()const
		{
			return m_login;
		}
		inline ChatClientStatus GetStatus()const
		{
			return m_status;
		}
		inline Clothes const & GetClothes()const
		{
			return m_clothes;
		}
		inline bool IsToDelete()const
		{
			return m_toDelete;
		}
		inline void SetStatus( ChatClientStatus p_status )
		{
			m_status = p_status;
		}
		inline String const & GetName()const
		{
			return m_login.GetName();
		}
		inline String const & GetPass()const
		{
			return m_login.GetPass();
		}
		inline bool IsAnonymous()const
		{
			return m_login.IsAnonymous();
		}
		inline uint32_t GetId()const
		{
			return m_login.GetId();
		}
		inline void SetId( uint32_t p_id )
		{
			m_login.SetId( p_id );
		}
		inline Vector3 const & GetPosition()const
		{
			return m_avatar.GetPosition();
		}
		inline float GetYaw()const
		{
			return m_avatar.GetYaw();
		}
		inline bool IsSat()const
		{
			return m_avatar.IsSat();
		}
		inline bool IsStopped()const
		{
			return m_avatar.IsStopped();
		}
		inline bool IsWorking()const
		{
			return m_avatar.IsWorking();
		}

	private:
		void DoQuitRoom();
		void DoQuitGame();
		bool DoEndWork();
		bool DoIsIgnored( const String & p_name );
		String DoBuildJoinMessage();
		void DoSendDressesMessage();
		void DoSendRoomsMessage();
		void DoSendKickMessage();
		void DoSendGameJoinFail();
		void DoForwardMessage( const String & p_message );
		void DoForwardGameMessage( const String & p_message );
		void DoForwardTalkMessage( const String & p_message );
		void DoProcessConnectMessage( General::Templates::ReadBuffer & p_buffer );
		void DoProcessDressMessage( General::Templates::ReadBuffer & p_buffer );
		void DoProcessJoinMessage( General::Templates::ReadBuffer & p_buffer );
		void DoProcessQuitMessage();
		void DoProcessUpdateMessage( General::Templates::ReadBuffer & p_buffer );
		void DoProcessWalkMessage( General::Templates::ReadBuffer & p_buffer );
		void DoProcessWhispMessage( General::Templates::ReadBuffer & p_buffer );
		void DoProcessTurnMessage( General::Templates::ReadBuffer & p_buffer );
		void DoProcessTalkMessage( General::Templates::ReadBuffer & p_buffer );
		void DoProcessBeginWalkMessage();
		void DoProcessEndWalkMessage( General::Templates::ReadBuffer & p_buffer );
		void DoProcessBeginRunMessage();
		void DoProcessEndRunMessage( General::Templates::ReadBuffer & p_buffer );
		void DoProcessEmoteMessage( General::Templates::ReadBuffer & p_buffer );
		void DoProcessRemoveFriendMessage( General::Templates::ReadBuffer & p_buffer );
		void DoProcessRemoveIgnoredMessage( General::Templates::ReadBuffer & p_buffer );
		void DoProcessNewFriendMessage( General::Templates::ReadBuffer & p_buffer );
		void DoProcessNewIgnoredMessage( General::Templates::ReadBuffer & p_buffer );
		void DoProcessUpdateFriendsMessage();
		void DoProcessUpdateIgnoredMessage();
		void DoProcessFriendRefuseMessage( General::Templates::ReadBuffer & p_buffer );
		void DoProcessFriendAcceptMessage( General::Templates::ReadBuffer & p_buffer );
		void DoProcessRefreshGamesListMessage( General::Templates::ReadBuffer & p_buffer );
		void DoProcessJoinGameMessage( General::Templates::ReadBuffer & p_buffer );
		void DoProcessCreateGameMessage( General::Templates::ReadBuffer & p_buffer );
		void DoProcessGameMessage( General::Templates::ReadBuffer & p_buffer );
		void DoProcessGameInfoMessage( General::Templates::ReadBuffer & p_buffer );
		void DoProcessGameStartMessage();
		void DoProcessQuitGameMessage();
		void DoProcessSitDownMessage();
		void DoProcessSitUpMessage();

		virtual void CallbackReceivedMessage( const String & p_message );
		virtual bool CallbackReaderError( const boost::system::error_code & p_err );
		virtual bool CallbackWriterError( const boost::system::error_code & p_err );
		virtual bool CallbackConnectorError( const boost::system::error_code & p_err );

		inline std::shared_ptr< ChatDatabase > DoGetDatabase()
		{
			return m_database.lock();
		}

	private:
		static uint32_t ClientsCount;
	};
}

#endif
