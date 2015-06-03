#ifndef ___ChatGame___
#define ___ChatGame___

#include "Common.h"

namespace Chat
{
	class ChatGame
	{
	private:
		std::weak_ptr< ChatTcpClient > m_initiator;
		StringArray m_playersPlaces;
		unsigned int m_initiatorId;
		String m_gameName;
		ClientIdChatCLientMap m_players;
		unsigned int m_maxPlayers;
		bool m_started;
		unsigned int m_id;
		std::weak_ptr< ChatDatabase > m_database;

	public:
		ChatGame( unsigned int p_id, std::shared_ptr< ChatTcpClient > p_initiator,
				  const String & p_gameName, unsigned int p_maxPlayers,
				  std::shared_ptr< ChatDatabase > p_database );
		~ChatGame();

		bool AddPlayer( std::shared_ptr< ChatTcpClient > p_client );
		bool RemovePlayer( std::shared_ptr< ChatTcpClient > p_client );
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
