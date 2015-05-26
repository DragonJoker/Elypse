#ifndef ___TCP_ACCEPTOR_H___
#define ___TCP_ACCEPTOR_H___

#include <boost/exception/diagnostic_information.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <set>

#include "Thread.h"
#include "StringConverter.h"
#include "STLSetMacros.h"

#include <FunctionTracker.h>

namespace General
{
	namespace MultiThreading
	{
		class TcpBaseClient
		{
		public:
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

		typedef std::set <TcpBaseClient *>							ClientSet;
		typedef std::set <TcpBaseClient *> ::iterator				ClientSetIt;
		typedef const std::set <TcpBaseClient *> ::const_iterator &	ClientSetEndIt;

		class TcpAcceptor
		{
		private:
			Thread * m_boostThread;

		public:
			boost::asio::io_service m_service;

		protected:
			boost::asio::ip::tcp::acceptor m_acceptor;

			ClientSet m_clients;
			TcpBaseClient * m_currentClient;

		public:
			TcpAcceptor( unsigned short p_port )
				:	m_boostThread( NULL ),
					m_service(),
					m_acceptor( m_service, boost::asio::ip::tcp::endpoint( boost::asio::ip::tcp::v4(), p_port ) )
			{
			}

			virtual ~TcpAcceptor()
			{
				if ( m_boostThread != NULL )
				{
					GENLIB_THREAD_WAIT_FOR_END_OF( m_boostThread );
					delete m_boostThread;
				}

				m_service.stop();
				m_acceptor.close();
				DeleteClient( m_currentClient );

				for ( ClientSetIt l_it = m_clients.begin() ; l_it != m_clients.end() ; ++l_it )
				{
					DeleteClient( *l_it );
				}

				m_clients.clear();
			}

		protected:
			virtual TcpBaseClient * _createNewClient() = 0;
			virtual void _deleteClient( TcpBaseClient * p_toDelete ) = 0;

			void _accept()
			{
//			std::cout << "TCPAcceptor - Accepted somebody\n";
				m_currentClient = _createNewClient();
				m_acceptor.async_accept( m_currentClient->GetSocket(), boost::bind( & TcpAcceptor::_acceptCallback, this, boost::asio::placeholders::error ) );
			}

			virtual void _mainLoop()
			{
				_accept();

				while ( true )
				{
					try
					{
						m_service.run();
						break;
					}
					catch ( const boost::exception & p_except )
					{
						std::cout << "Catch boost::exception, diagnostic_information : [" << p_except.diagnostic_information() << "]" << std::endl;
						ClientSetIt l_it = m_clients.begin();
						ClientSetEndIt l_endIt = m_clients.end();

						while ( l_it != l_endIt )
						{
							TcpBaseClient * l_toTest = ( * l_it );

							try
							{
								boost::asio::ip::tcp::endpoint l_endPoint = l_toTest->GetSocket().remote_endpoint();
								std::cout << "Test socket [" << l_endPoint.address().to_string() << ":" << General::Utils::ToString( l_endPoint.port() ) << "]" << std::endl;
							}
							catch ( const boost::exception & p_except2 )
							{
								std::cout << "Catch boost::exception, diagnostic_information : [" << p_except2.diagnostic_information() << "]" << std::endl;
								General::Utils::set::eraseIterator( m_clients, l_it );
//							std::cout << "TCPAcceptor :: _mainLoop - coucou 1.1\n";
								DeleteClient( l_toTest );
//							std::cout << "TCPAcceptor :: _mainLoop - coucou 1.2\n";
								continue;
							}

							if ( ! l_toTest->GetSocket().is_open() )
							{
								std::cout << "A closed socket was detected in client list... Will be deleted..." << std::endl;
								General::Utils::set::eraseIterator( m_clients, l_it );
//							std::cout << "TCPAcceptor :: _mainLoop - coucou 2.1\n";
								DeleteClient( l_toTest );
//							std::cout << "TCPAcceptor :: _mainLoop - coucou 2.2\n";
								continue;
							}

							l_it ++;
						}
					}
				}
			}

			virtual void _acceptCallback( const boost::system::error_code & p_error )
			{
//			std::cout << "TCPAcceptor - _acceptCallback\n";
				if ( p_error )
				{
					std::cout << "Error at _acceptCallback" << std::endl;
					return;
				}

				m_clients.insert( m_currentClient );
				m_currentClient->Start();
//			Sleep( 100);
				_accept();
			}

		public:
			virtual void Run()
			{
				m_boostThread = new Thread( GENLIB_THREAD_CLASS_FUNCTOR( this, TcpAcceptor, _mainLoop ) );
			}

			virtual void DeleteClient( TcpBaseClient * p_toDelete )
			{
				std::cout << "DeleteClient : " << &( p_toDelete->m_socket ) << std::endl;
				_deleteClient( p_toDelete );
			}

			virtual void EraseClient( TcpBaseClient * p_toDelete )
			{
				std::cout << "EraseClient : " << &( p_toDelete->m_socket ) << std::endl;
				ClientSetIt l_toDel = m_clients.find( p_toDelete );
				ClientSetEndIt l_endIt = m_clients.end();

				if ( l_toDel != l_endIt )
				{
//				std::cout << "TCPAcceptor :: EraseClient - Found in clients" << std::endl;
					m_clients.erase( l_toDel );
					DeleteClient( p_toDelete );
//				std::cout << "TCPAcceptor :: EraseClient - Deleted" << std::endl;
				}

//			std::cout << "TCPAcceptor :: EraseClient - End" << std::endl;
			}
		};
	}
}


#endif
