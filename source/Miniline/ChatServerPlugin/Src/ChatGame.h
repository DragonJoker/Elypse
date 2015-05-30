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
		ChatDatabase * m_database;

	public:
		ChatGame( unsigned int p_id, ChatTcpClient * p_initiator,
				  const String & p_gameName, unsigned int p_maxPlayers,
				  ChatDatabase * p_database );
		~ChatGame();

		bool AddPlayer( ChatTcpClient * p_client );
		bool RemovePlayer( ChatTcpClient * p_client );
		void StartGame();
		void EndGame();
		int GetPlayerPlace( const String & p_name );

	public:
		inline unsigned int 					GetInitiatorId()const
		{
			return m_initiatorId;
		}
		inline bool 							IsFull()const
		{
			return ( static_cast <unsigned int>( m_players.size() ) >= m_maxPlayers ) || m_started;
		}
		inline unsigned int						GetNbPlayers()const
		{
			return static_cast <unsigned int>( m_players.size() );
		}
		inline const String 		&			GetGameName()const
		{
			return m_gameName;
		}
		inline const ClientIdChatCLientMap &	GetPlayers()const
		{
			return m_players;
		}
	};
}

#endif
