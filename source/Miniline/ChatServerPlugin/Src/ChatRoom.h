#ifndef ___ChatRoom___
#define ___ChatRoom___

#include "Common.h"

namespace Chat
{
	class ChatRoom
		: public std::enable_shared_from_this< ChatRoom >
	{
	public:
		ChatRoom( const String & p_name, const String & p_sceneName )
			: m_name( p_name )
			, m_sceneName( p_sceneName )
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
		void ForwardMessage( const String & p_message, const String & p_name, ClientIdStrMap const & p_ignored );
		void Save( General::Templates::WriteBuffer & p_buffer )const;

	public:
		inline void SetName( const String & p_name )
		{
			m_name = p_name;
		}
		inline void SetSceneName( const String & p_sceneName )
		{
			m_sceneName = p_sceneName;
		}

		inline const String & GetName()const
		{
			return m_name;
		}
		inline const String & GetSceneName()const
		{
			return m_sceneName;
		}
		inline uint32_t GetClientCount()const
		{
			return uint32_t( m_clients.size() );
		}
		inline const ChatTcpClientStrMap & GetClients()const
		{
			return m_clients;
		}

	private:
		String m_name;
		String m_sceneName;
		ChatTcpClientStrMap m_clients;
	};
}

#endif
