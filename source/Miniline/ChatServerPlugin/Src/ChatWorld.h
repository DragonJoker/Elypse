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
		void AddRoom( ChatRoom * p_room );
		ChatRoom * GetRoom( const String & p_name );

	public:
		inline size_t 					GetNumRooms()
		{
			return m_rooms.size();
		}
		inline ChatRoomStrMap 			GetRooms()
		{
			return m_rooms;
		}
	};
}

#endif
