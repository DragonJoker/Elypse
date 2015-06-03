#ifndef ___ChatWorld___
#define ___ChatWorld___

#include "Common.h"

namespace Chat
{
	class ChatWorld
	{
	private:
		ChatRoomStrMap m_rooms;
		String m_name;
		unsigned int m_maxPeople;

	public:
		ChatWorld( const String & p_name, unsigned int p_maxPeople )
			:	m_maxPeople( p_maxPeople )
		{
		}
		~ChatWorld();

		void AddRoom( const String & p_name, const String & p_sceneName );
		void AddRoom( std::shared_ptr< ChatRoom > p_room );
		std::shared_ptr< ChatRoom > GetRoom( const String & p_name );
		void Save( General::Templates::WriteBuffer & p_buffer )const;

		inline size_t GetRoomsCount()
		{
			return m_rooms.size();
		}
		inline ChatRoomStrMap GetRooms()
		{
			return m_rooms;
		}
	};
}

#endif
