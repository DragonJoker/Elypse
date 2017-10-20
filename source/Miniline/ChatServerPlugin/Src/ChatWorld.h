/* See Licence.txt in root folder */
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
