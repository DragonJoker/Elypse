#ifndef ___Common___
#define ___Common___

#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <memory>
#include <cstdint>

#include <Database/DatabasePrerequisites.h>

namespace General
{
	namespace Templates
	{
		class WriteBuffer;
		class ReadBuffer;
	}
}

namespace Chat
{
	typedef std::string String;

	static const String ChatEmptyString;

	class LoginInformations;
	struct Avatar;

	class ChatPlugin;
	class ChatTcpClient;
	class ChatRoom;
	class ChatClient;
	class ChatWorld;
	class ChatGame;
	class ChatDatabase;

	typedef std::map< String, ChatTcpClient * > ChatTcpClientStrMap;
	typedef std::map< String, uint32_t > ClientIdStrMap;
	typedef std::map< uint32_t, ChatTcpClient * > ClientIdChatCLientMap;
	typedef std::map< String, std::shared_ptr< ChatRoom > > ChatRoomStrMap;
	typedef std::map< uint32_t, std::shared_ptr< ChatGame > > ChatGameIDMap;
	typedef std::multimap< String, ChatGameIDMap > ChatGameStrMap;
	typedef std::map< uint32_t, String > StringIdMap;
	typedef std::pair< String, uint32_t > StrUIntPair;
	typedef std::map< uint32_t, StrUIntPair > StrUIntIdMap;
	typedef std::vector< String > StringArray;

	struct Vector3
	{
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
	};

	enum ChatClientStatus
	: int
	{
		ccsNone,
		ccsConnected,
		ccsDressed,
		ccsInRoom,
		ccsInGame
	};

	enum ChatClientDressSlot
	: int
	{
		ccdsSex,
		ccdsBody,
		ccdsSkin,
		ccdsHair,
		ccdsHairColour,
		ccdsDress,
		ccdsEyesColour,
		ccdsBrow,
		ccdsBrowColour,
		ccdsSpecific1,	// Beard for man, Eye lash for girl
		ccdsSpecific2,	// Beard colour for man, Lipstick for girl
		ccdsMaxDresses,
	};

	struct Dress
	{
		bool Load( General::Templates::ReadBuffer & p_buffer );
		void Save( General::Templates::WriteBuffer & p_buffer )const;
		int m_id = 0;
		ChatClientDressSlot m_slot = ccdsMaxDresses;
		std::map< int, int > m_materials;
	};

	struct Tattoo
	{
		bool Load( General::Templates::ReadBuffer & p_buffer );
		void Save( General::Templates::WriteBuffer & p_buffer )const;
		int m_id = 0;
		ChatClientDressSlot m_slot = ccdsMaxDresses;
	};

	struct Clothes
	{
		bool Load( General::Templates::ReadBuffer & p_buffer );
		void Save( General::Templates::WriteBuffer & p_buffer )const;
		std::map< ChatClientDressSlot, Dress > m_dresses;
		std::map< ChatClientDressSlot, Tattoo > m_tattoos;
		int8_t m_sex = 0;
	};

	struct ConfigInfo
	{
		String DBName;
		String DBPass;
		String DBUser;
		String DBHost;
		uint16_t DBPort = 0;
	};

	enum MessageReceived
	: int
	{
		mrConnect,
		mrAvatar,
		mrJoin,
		mrQuit,
		mrUpdateRooms,
		mrUpdate,
		mrWalk,
		mrTurn,
		mrTalk,
		mrWhisp,
		mrBeginWalk,
		mrEndWalk,
		mrEmote,
		mrRemoveFriend,
		mrRemoveIgnored,
		mrUpdateFriendsList,
		mrUpdateIgnoredList,
		mrNewFriend,
		mrNewIgnored,
		mrNewFriendAccept,
		mrNewFriendRefuse,
		mrBeginRun,
		mrEndRun,
		mrRefreshGamesList,
		mrJoinGame,
		mrCreateGame,
		mrGame,
		mrQuitGame,
		mrSitDown,
		mrSitUp,
		mrGameInfo,
		mrGameStart,
		mrRequestDresses
	};

	enum MessageSent
	: int
	{
		msConnectOK,
		msConnectFail,
		msJoin,
		msQuit,
		msAvatar,
		msRooms,
		msUpdate,
		msWalk,
		msTurn,
		msWhisp,
		msWhispOK,
		msWhispFail,
		msBeginWalk,
		msEndWalk,
		msEmote,
		msNewFriendDoesntExist,
		msNewFriendAlreadyFriend,
		msNewFriendAsk,
		msNewIgnoredDoesntExist,
		msNewIgnoredAlreadyIgnored,
		msNewIgnoredDone,
		msUpdateFriendsList,
		msUpdateIgnoredList,
		msNewFriendRefuse,
		msNewFriendAccept,
		msTalk,
		msKickUser,
		msBeginRun,
		msEndRun,
		msJoinGameOK,
		msCreateGameOK,
		msGameDontExist,
		msGameAlreadyCreated,
		msGameStart,
		msRefreshGamesList,
		msJoinGameFail,
		msGame,
		msEndGame,
		msJoinGame,
		msQuitGame,
		msSitDown,
		msSitUp,
		msGameInfo,
		msUnavailableToAnonymous,
		msGameDeleted,
		msAvatarMats,
		msEndAvatar
	};
}

#if defined( _MSC_VER )
#	define snprintf sprintf_s
#	define strncat( a, b, c)	strcat_s( a, c, b)
#endif

#endif
