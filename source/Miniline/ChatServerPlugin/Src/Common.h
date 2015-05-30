#ifndef ___Common___
#define ___Common___

#include <string>
#include <map>
#include <vector>
#include <iostream>

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

	class ChatPlugin;
	class ChatTcpClient;
	class ChatRoom;
	class ChatClient;
	class ChatWorld;
	class ChatGame;
	class ChatDatabase;

	typedef std::map 		<String, ChatTcpClient *>		ChatTcpClientStrMap;
	typedef std::map 		<String, unsigned int> 			ClientIdStrMap;
	typedef std::map 		<unsigned int, ChatTcpClient *>	ClientIdChatCLientMap;
	typedef std::map 		<String, ChatRoom *> 			ChatRoomStrMap;
	typedef std::map		<unsigned int, ChatGame *>		ChatGameIDMap;
	typedef std::multimap	<String, ChatGameIDMap>			ChatGameStrMap;
	typedef std::map		<unsigned int, String>			StringIdMap;
	typedef std::pair		<String, unsigned int>			StrUIntPair;
	typedef std::map		<unsigned int, StrUIntPair>		StrUIntIdMap;
	typedef std::vector		<String>						StringArray;

	struct Vector3
	{
		float x;
		float y;
		float z;

		Vector3( float p_x, float p_y, float p_z )
			:	x( p_x ),
				y( p_y ),
				z( p_z )
		{
		}

		Vector3()
			:	x( 0.0 ),
				y( 0.0 ),
				z( 0.0 )
		{
		}
	};

	enum ChatClientStatus
	{
		ccsNone,
		ccsConnected,
		ccsDressed,
		ccsInRoom,
		ccsInGame
	};

	enum ChatClientDress
	{
		ccdSex			= 0,
		ccdBody			= 1,
		ccdSkin			= 2,
		ccdHair			= 3,
		ccdHairColour	= 4,
		ccdDress		= 5,
		ccdEyesColour	= 6,
		ccdBrow			= 7,
		ccdBrowColour	= 8,
		ccdSpecific1	= 9,	// Beard for man, Eye lash for girl
		ccdSpecific2	= 10,	// Beard colour for man, Lipstick for girl
		ccdMaxDresses 	= 11
	};

	struct Dress
	{
		int m_id;
		std::map <int, int> m_materials;
		int m_slot;

		Dress( int p_id, int p_slot )
			:	m_id( p_id ),
				m_slot( p_slot )
		{
		}

		~Dress()
		{
			m_materials.clear();
		}
	};

	struct Tattoo
	{
		int m_id;
		int m_slot;

		Tattoo( int p_id, int p_slot )
			:	m_id( p_id ),
				m_slot( p_slot )
		{
		}

		~Tattoo()
		{
		}
	};

	struct Clothes
	{
		std::map <int, Dress *> m_dresses;
		std::map <int, Tattoo *> m_tattoos;
		int m_sex;

		Clothes()
			:	m_sex( 0 )
		{
		}

		~Clothes()
		{
			for ( std::map <int, Dress *>::iterator i = m_dresses.begin() ; i != m_dresses.end() ; i++ )
			{
				delete i->second;;
			}

			m_dresses.clear();

			for ( std::map <int, Tattoo *>::iterator i = m_tattoos.begin() ; i != m_tattoos.end() ; i++ )
			{
				delete i->second;;
			}

			m_tattoos.clear();
		}
	};

	struct ConfigInfo
	{
		String DBName;
		String DBPass;
		String DBUser;
		String DBHost;
		unsigned short DBPort;
	};

	enum MessageReceived
	{
		mrConnect			= 0,
		mrAvatar			= 1,
		mrJoin				= 2,
		mrQuit				= 3,
		mrUpdateRooms		= 4,
		mrUpdate			= 5,
		mrWalk				= 6,
		mrTurn				= 7,
		mrTalk				= 8,
		mrWhisp				= 9,
		mrBeginWalk			= 10,
		mrEndWalk			= 11,
		mrEmote				= 12,
		mrRemoveFriend		= 13,
		mrRemoveIgnored		= 14,
		mrUpdateFriendsList	= 15,
		mrUpdateIgnoredList	= 16,
		mrNewFriend			= 17,
		mrNewIgnored		= 18,
		mrNewFriendAccept	= 19,
		mrNewFriendRefuse	= 20,
		mrBeginRun			= 21,
		mrEndRun			= 22,
		mrRefreshGamesList	= 23,
		mrJoinGame			= 24,
		mrCreateGame		= 25,
		mrGame				= 26,
		mrQuitGame			= 27,
		mrSitDown			= 28,
		mrSitUp				= 29,
		mrGameInfo			= 30,
		mrGameStart			= 31,
		mrRequestDresses	= 32
	};

	enum MessageSent
	{
		msConnectOK					= 0,
		msConnectFail				= 1,
		msJoin						= 2,
		msQuit						= 3,
		msAvatar					= 4,
		msRooms						= 5,
		msUpdate					= 6,
		msWalk						= 7,
		msTurn						= 8,
		msWhisp						= 9,
		msWhispOK					= 10,
		msWhispFail					= 11,
		msBeginWalk					= 12,
		msEndWalk					= 13,
		msEmote						= 14,
		msNewFriendDoesntExist		= 15,
		msNewFriendAlreadyFriend	= 16,
		msNewFriendAsk				= 17,
		msNewIgnoredDoesntExist		= 18,
		msNewIgnoredAlreadyIgnored	= 19,
		msNewIgnoredDone			= 20,
		msUpdateFriendsList			= 21,
		msUpdateIgnoredList			= 22,
		msNewFriendRefuse			= 23,
		msNewFriendAccept			= 24,
		msTalk						= 25,
		msKickUser					= 26,
		msBeginRun					= 27,
		msEndRun					= 28,
		msJoinGameOK				= 29,
		msCreateGameOK				= 30,
		msGameDontExist				= 31,
		msGameAlreadyCreated		= 32,
		msGameStart					= 33,
		msRefreshGamesList			= 34,
		msJoinGameFail				= 35,
		msGame						= 36,
		msEndGame					= 37,
		msJoinGame					= 38,
		msQuitGame					= 39,
		msSitDown					= 40,
		msSitUp						= 41,
		msGameInfo					= 42,
		msUnavailableToAnonymous	= 43,
		msGameDeleted				= 44,
		msAvatarMats				= 45,
		msEndAvatar					= 46
	};
}

#ifndef __GNUG__
#define snprintf sprintf_s
#define strncat( a, b, c)	strcat_s( a, c, b)
#endif

#endif
