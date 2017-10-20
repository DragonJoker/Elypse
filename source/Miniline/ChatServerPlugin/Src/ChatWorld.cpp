/* See Licence.txt in root folder */
#include "ChatWorld.h"
#include "ChatRoom.h"

#include <Buffer.h>

using namespace Chat;;

ChatWorld::~ChatWorld()
{
	m_rooms.clear();
}

void ChatWorld::AddRoom( String const & p_name, String const & p_sceneName )
{
	AddRoom( std::make_shared< ChatRoom >( p_name, p_sceneName ) );
}

void ChatWorld::AddRoom( std::shared_ptr< ChatRoom > p_room )
{
	if ( m_rooms.find( p_room->GetName() ) == m_rooms.end() )
	{
		m_rooms.insert( std::make_pair( p_room->GetName(), p_room ) );
	}
}

std::shared_ptr< ChatRoom > ChatWorld::GetRoom( String const & p_name )
{
	std::shared_ptr< ChatRoom > l_return;
	auto l_it = m_rooms.find( p_name );

	if ( l_it != m_rooms.end() )
	{
		l_return = l_it->second;
	}

	return l_return;
}

ChatTcpClient * ChatWorld::GetClient( String const & p_name )const
{
	ChatTcpClient * l_return = NULL;

	auto && l_it = std::find_if( m_rooms.begin(), m_rooms.end(), [&p_name, &l_return]( ChatRoomStrMap::value_type const & p_room )
	{
		return l_return = p_room.second->GetClient( p_name );
	} );

	return l_return;
}

void ChatWorld::Save( General::Templates::WriteBuffer & p_buffer )const
{
	std::clog << "DoSendRoomsMessage - " << m_rooms.size() << std::endl;
	p_buffer << int( m_rooms.size() );

	for ( auto && l_room : m_rooms )
	{
		l_room.second->Save( p_buffer );
	}
}
