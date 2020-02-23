/* See Licence.txt in root folder */
#ifndef ___ChatGame___
#define ___ChatGame___

#include "Common.h"

namespace Chat
{
	class ChatGame
	{
	public:
		ChatGame( uint32_t p_id, ChatTcpClient * p_initiator,
				  String const & p_gameName, uint32_t p_maxPlayers,
				  std::shared_ptr< ChatDatabase > p_database );
		~ChatGame();

		bool AddPlayer( ChatTcpClient * p_client );
		bool RemovePlayer( ChatTcpClient * p_client );
		void StartGame();
		void EndGame();
		uint32_t GetPlayerPlace( String const & p_name );
		void ForwardMessage( String const & p_message, uint32_t p_name );

		inline uint32_t GetInitiatorId()const
		{
			return m_initiatorId;
		}
		inline bool IsFull()const
		{
			return GetPlayersCount() >= m_maxPlayers || m_started;
		}
		inline uint32_t GetPlayersCount()const
		{
			return uint32_t( m_players.size() );
		}
		inline String const & GetGameName()const
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

	private:
		ChatTcpClient * m_initiator;
		StringArray m_playersPlaces;
		uint32_t m_initiatorId;
		String m_gameName;
		ClientIdChatCLientMap m_players;
		uint32_t m_maxPlayers;
		bool m_started;
		uint32_t m_id;
		std::weak_ptr< ChatDatabase > m_database;
	};
}

#endif
