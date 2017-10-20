/*
See LICENSE file in root folder
*/
#ifndef __GENLIB_TCP_SERVER_H___
#define __GENLIB_TCP_SERVER_H___

#include "TcpAcceptor.h"
#include <TcpWriter.h>
#include <TcpReader.h>
#include <Macros.h>

namespace Elypse
{
	namespace Network
	{
		// Exemple de TcpReader qui merde fais par le Bubar � revoir avant usage
		class TcpTestReader
			: public General::Network::TcpQueuedReader
		{
		protected:
			General::Templates::CircularQueue< std::string > m_messages;

		public:
			TcpTestReader( boost::asio::ip::tcp::socket & p_socket, boost::asio::io_service & p_service )
				: General::Network::TcpQueuedReader( p_socket, p_service )
			{
			}
			virtual ~TcpTestReader()
			{
			}

		protected:
			virtual void CallbackReceivedMessage( std::string const & p_message )
			{
				std::cout << "message recu : " << p_message << std::endl;
				m_messages.push_back( p_message );
			}
		};

		class ChatClient
			: public TcpBaseClient
		{
		public:
			General::Network::TcpWriter m_writer;
			TcpTestReader m_reader;

		public:
			ChatClient( boost::asio::io_service & p_service )
				: TcpBaseClient( p_service )
				, m_writer( m_socket, p_service )
				, m_reader( m_socket, p_service )
			{
			}

		public:
			virtual void Start()
			{
				m_reader.StartAsyncRead();
			}
		};

		class TcpServer
			: public TcpAcceptor
		{
		public:
			TcpServer( uint16_t p_port )
				: TcpAcceptor( p_port )
			{
				DoAccept();
			}

			~TcpServer()
			{
			}

		public:
			virtual TcpBaseClient * DoCreateClient()
			{
				return new ChatClient( m_service );
			}
		};
	}
}

#endif
