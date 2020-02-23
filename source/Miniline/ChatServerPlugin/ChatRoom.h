/* See Licence.txt in root folder */
#ifndef ___ChatRoom___
#define ___ChatRoom___

#include "Common.h"

namespace Chat
{
	class ChatRoom
		: public std::enable_shared_from_this< ChatRoom >
	{
	public:
		ChatRoom( String const & p_name, String const & p_sceneName )
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
		ChatTcpClient * GetClient( String const & p_name )const;
		void ForwardMessage( String const & p_message, uint32_t p_id );
		void ForwardMessage( String const & p_message, uint32_t p_id, ClientStrIdMap const & p_ignored );
		void Save( General::Templates::WriteBuffer & p_buffer )const;

	public:
		inline void SetName( String const & p_name )
		{
			m_name = p_name;
		}
		inline void SetSceneName( String const & p_sceneName )
		{
			m_sceneName = p_sceneName;
		}

		inline String const & GetName()const
		{
			return m_name;
		}
		inline String const & GetSceneName()const
		{
			return m_sceneName;
		}
		inline uint32_t GetClientCount()const
		{
			return uint32_t( m_clients.size() );
		}
		inline const ChatTcpClientIdMap & GetClients()const
		{
			return m_clients;
		}

	private:
		String m_name;
		String m_sceneName;
		ChatTcpClientIdMap m_clients;
	};
}

#endif
