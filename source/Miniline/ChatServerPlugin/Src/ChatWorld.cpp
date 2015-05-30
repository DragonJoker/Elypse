#include "ChatWorld.h"
#include "ChatRoom.h"

using namespace Chat;;

ChatWorld::~ChatWorld()
{
	ChatRoomStrMap::iterator l_it = m_rooms.begin();
	ChatRoom * l_room;

	while ( l_it != m_rooms.end() )
	{
		l_room = l_it->second;
		delete l_room;
		++l_it;
	}

	m_rooms.clear();
}

void ChatWorld::AddRoom( const String & p_name, const String & p_sceneName )
{
	if ( m_rooms.find( p_name ) == m_rooms.end() )
	{
		ChatRoom * l_room = new ChatRoom( p_name, p_sceneName );
		m_rooms.insert( ChatRoomStrMap::value_type( p_name, l_room ) );
	}
}

void ChatWorld::AddRoom( ChatRoom * p_room )
{
	if ( m_rooms.find( p_room->GetName() ) == m_rooms.end() )
	{
		m_rooms.insert( ChatRoomStrMap::value_type( p_room->GetName(), p_room ) );
	}
}

ChatRoom * ChatWorld::GetRoom( const String & p_name )
{
	ChatRoomStrMap::iterator l_it = m_rooms.find( p_name );

	if ( l_it != m_rooms.end() )
	{
		return l_it->second;
	}

	return NULL;
}
