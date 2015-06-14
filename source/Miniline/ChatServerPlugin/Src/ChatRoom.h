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
