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
		class ChatClient : public TcpBaseClient
		{
		public:
			General::Network::TcpWriter m_writer;
			General::Network::TcpQueuedReader m_reader;

		public:
			ChatClient( boost::asio::io_service & p_service )
				:	TcpBaseClient( p_service ),
					m_writer( m_socket, p_service ),
					m_reader( m_socket, p_service )
			{
			}

		public:
			virtual void Start()
			{
				m_reader.StartAsyncRead();
			}
		};

		class TcpServer : public TcpAcceptor
		{
		public:
			TcpServer( unsigned short p_port )
				:	TcpAcceptor( p_port )
			{
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
