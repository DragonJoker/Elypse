/*
See LICENSE file in root folder
*/
#ifndef ___TCP_ACCEPTOR_H___
#define ___TCP_ACCEPTOR_H___

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <set>

#include <STLSetMacros.h>

namespace Elypse
{
	namespace Network
	{
		class TcpBaseClient
		{
		protected:
			boost::asio::ip::tcp::socket m_socket;

		public:
			TcpBaseClient( boost::asio::io_service & p_service )
				:	m_socket( p_service )
			{
			}
			virtual ~TcpBaseClient()
			{
				m_socket.close();
			}

		public:
			virtual void Start() = 0;

		public:
			inline boost::asio::ip::tcp::socket & GetSocket()
			{
				return m_socket;
			}
		};

		class TcpAcceptor
		{
		protected:
			boost::asio::io_service m_service;
			boost::asio::ip::tcp::acceptor m_acceptor;

			std::set< TcpBaseClient * > m_clients;

			TcpBaseClient * m_currentClient;

		public:
			TcpAcceptor( uint16_t p_port )
				: m_service()
				, m_acceptor( m_service, boost::asio::ip::tcp::endpoint( boost::asio::ip::tcp::v4(), p_port ) )
			{
			}

			virtual ~TcpAcceptor()
			{
				m_acceptor.close();
				delete m_currentClient;
				General::Utils::set::deleteAll( m_clients );
			}

		protected:
			virtual void DoAccept()
			{
				m_currentClient = DoCreateClient();
				m_acceptor.async_accept( m_currentClient->GetSocket(), boost::bind( &TcpAcceptor::CallbackAccept, this, boost::asio::placeholders::error ) );
			}

		private:
			virtual TcpBaseClient * DoCreateClient() = 0;

			virtual void CallbackAccept( boost::system::error_code const & error )
			{
				if ( error )
				{
					return;
				}

				m_currentClient->Start();
				m_clients.insert( m_currentClient );
				DoAccept();
			}

		public:
			void Run()
			{
				m_service.run();
			}
		};
	}
}

#endif
