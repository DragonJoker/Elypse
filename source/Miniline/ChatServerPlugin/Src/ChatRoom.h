#ifndef ___ChatRoom___
#define ___ChatRoom___

#include "Common.h"

namespace Chat
{
	class ChatRoom
	{
	private:
		String m_name;
		String m_sceneName;
		ChatTcpClientStrMap m_clients;

	public:
		ChatRoom( const String & p_name, const String & p_sceneName )
			:	m_name( p_name ),
				m_sceneName( p_sceneName )
		{
		}

		ChatRoom()
		{
		}

		~ChatRoom()
		{
		}

		bool AddClient( ChatTcpClient * p_client );
		bool RemoveClient( ChatTcpClient * p_client );
		void ForwardMessage( const String & p_message, const String & p_name );

	public:
		inline void SetName( const String & p_name )
		{
			m_name = p_name;
		}
		inline void SetSceneName( const String & p_sceneName )
		{
			m_sceneName = p_sceneName;
		}

		inline const String 		&		GetName()const
		{
			return m_name;
		}
		inline const String 		&		GetSceneName()const
		{
			return m_sceneName;
		}
		inline unsigned int					GetNbPeople()const
		{
			return m_clients.size();
		}
		inline const ChatTcpClientStrMap &	GetClients()const
		{
			return m_clients;
		}
	};
}

#endif
