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
#ifndef ___ChatGame___
#define ___ChatGame___

#include "Common.h"

namespace Chat
{
	class ChatGame
	{
	private:
		ChatTcpClient * m_initiator;
		StringArray m_playersPlaces;
		unsigned int m_initiatorId;
		String m_gameName;
		ClientIdChatCLientMap m_players;
		unsigned int m_maxPlayers;
		bool m_started;
		unsigned int m_id;
		std::weak_ptr< ChatDatabase > m_database;

	public:
		ChatGame( unsigned int p_id, ChatTcpClient * p_initiator,
				  const String & p_gameName, unsigned int p_maxPlayers,
				  std::shared_ptr< ChatDatabase > p_database );
		~ChatGame();

		bool AddPlayer( ChatTcpClient * p_client );
		bool RemovePlayer( ChatTcpClient * p_client );
		void StartGame();
		void EndGame();
		int GetPlayerPlace( const String & p_name );
		void ForwardMessage( const String & p_message, const String & p_name );

		inline unsigned int GetInitiatorId()const
		{
			return m_initiatorId;
		}
		inline bool IsFull()const
		{
			return ( static_cast <unsigned int>( m_players.size() ) >= m_maxPlayers ) || m_started;
		}
		inline unsigned int GetPlayersCount()const
		{
			return static_cast <unsigned int>( m_players.size() );
		}
		inline const String & GetGameName()const
		{
			return m_gameName;
		}
		inline const ClientIdChatCLientMap & GetPlayers()const
		{
			return m_players;
		}

	private:
		inline std::shared_ptr< ChatDatabase > DoGetDatabase()
		{
			return m_database.lock();
		}
	};
}

#endif
