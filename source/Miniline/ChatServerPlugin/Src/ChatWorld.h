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
#ifndef ___ChatWorld___
#define ___ChatWorld___

#include "Common.h"

namespace Chat
{
	class ChatWorld
	{
	public:
		ChatWorld( String const & p_name, uint32_t p_maxPeople )
			:	m_maxPeople( p_maxPeople )
		{
		}
		~ChatWorld();

		void AddRoom( String const & p_name, String const & p_sceneName );
		void AddRoom( std::shared_ptr< ChatRoom > p_room );
		std::shared_ptr< ChatRoom > GetRoom( String const & p_name );
		ChatTcpClient * GetClient( String const & p_name )const;
		void Save( General::Templates::WriteBuffer & p_buffer )const;

		inline size_t GetRoomsCount()
		{
			return m_rooms.size();
		}
		inline ChatRoomStrMap GetRooms()
		{
			return m_rooms;
		}

	private:
		ChatRoomStrMap m_rooms;
		String m_name;
		uint32_t m_maxPeople;
	};
}

#endif
