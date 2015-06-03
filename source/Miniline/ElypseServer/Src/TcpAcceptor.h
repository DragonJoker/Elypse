#ifndef ___TCP_ACCEPTOR_H___
#define ___TCP_ACCEPTOR_H___

#include <set>

#include "BoostHeaders.h"

#include <Thread.h>
#include <StringConverter.h>
#include <STLSetMacros.h>

namespace Elypse
{
	namespace Network
	{
		class TcpBaseClient
		{
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

		public:
			boost::asio::ip::tcp::socket m_socket;
		};

		typedef std::vector< std::shared_ptr< TcpBaseClient > > ClientSet;

		class TcpAcceptor
		{
		public:
			TcpAcceptor( unsigned short p_port )
				: m_service()
				, m_acceptor( m_service, boost::asio::ip::tcp::endpoint( boost::asio::ip::tcp::v4(), p_port ) )
			{
			}

			virtual ~TcpAcceptor()
			{
				if ( m_thread.joinable() )
				{
					m_thread.join();
				}

				m_service.stop();
				m_acceptor.close();
				DeleteClient( m_currentClient );

				for ( auto && l_client : m_clients )
				{
					DeleteClient( l_client );
				}

				m_clients.clear();
			}

		protected:
			virtual std::shared_ptr< TcpBaseClient > DoCreateNewClient() = 0;
			virtual void DoDestroyClient( std::shared_ptr< TcpBaseClient > & p_client ) = 0;

			void DoAccept()
			{
				std::cout << "TCPAcceptor - Accepted somebody\n";
				m_currentClient = DoCreateNewClient();
				m_acceptor.async_accept( m_currentClient->GetSocket(), boost::bind( &TcpAcceptor::CallbackAccept, this, boost::asio::placeholders::error ) );
			}

			virtual void DoMainLoop()
			{
				DoAccept();

				while ( true )
				{
					try
					{
						m_service.run();
						break;
					}
					catch ( const boost::exception & )
					{
						std::cout << "Catch boost::exception" << std::endl;//, diagnostic_information : [" << p_except.get_diagnostic_information() << "]" << std::endl;
						ClientSet l_clients;

						for ( auto && l_client : m_clients )
						{
							try
							{
								boost::asio::ip::tcp::endpoint l_endPoint = l_client->GetSocket().remote_endpoint();
								std::cout << "Test socket [" << l_endPoint.address().to_string() << ":" << General::Utils::ToString( l_endPoint.port() ) << "]" << std::endl;

								if ( !l_client->GetSocket().is_open() )
								{
									std::cout << "A closed socket was detected in client list... Will be deleted..." << std::endl;
									DeleteClient( l_client );
								}
								else
								{
									l_clients.push_back( l_client );
								}
							}
							catch ( const boost::exception & )
							{
								std::cout << "Re-catch boost::exception" << std::endl;//, diagnostic_information : [" << p_except2.get_diagnostic_information() << "]" << std::endl;
								DeleteClient( l_client );
							}
						}

						std::swap( m_clients, l_clients );
					}
				}
			}

			virtual void CallbackAccept( const boost::system::error_code & p_error )
			{
				std::cout << "TCPAcceptor - _acceptCallback\n";

				if ( p_error )
				{
					std::cout << "Error at _acceptCallback" << std::endl;
					return;
				}
				
				m_currentClient->Start();
				m_clients.push_back( std::move( m_currentClient ) );

				DoAccept();
			}

		public:
			virtual void Run()
			{
				m_thread = std::thread( std::bind( &TcpAcceptor::DoMainLoop, this ) );
			}

			virtual void DeleteClient( std::shared_ptr< TcpBaseClient > & p_client )
			{
				std::cout << "DeleteClient : " << &( p_client->m_socket ) << std::endl;
				DoDestroyClient( p_client );
			}

			virtual void EraseClient( std::shared_ptr< TcpBaseClient > p_client )
			{
				std::cout << "EraseClient : " << &( p_client->m_socket ) << std::endl;
				auto && l_it = std::find_if( m_clients.begin(), m_clients.end(), [&p_client]( std::shared_ptr< TcpBaseClient > const & l_client )
				{
					return p_client == l_client;
				} );

				auto && l_endIt = m_clients.end();

				if ( l_it != l_endIt )
				{
					m_clients.erase( l_it );
					DeleteClient( *l_it );
				}
			}

		private:
			std::thread m_thread;

		public:
			boost::asio::io_service m_service;

		protected:
			boost::asio::ip::tcp::acceptor m_acceptor;
			ClientSet m_clients;
			std::shared_ptr< TcpBaseClient > m_currentClient;
		};
	}
}


#endif
