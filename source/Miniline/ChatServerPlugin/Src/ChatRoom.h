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
