#include "TcpAcceptor.h"

namespace General
{
	namespace Network
	{
		//***********************************************************************************************

		TcpBaseClient::TcpBaseClient( boost::asio::io_service & p_service )
			: m_socket{ p_service }
		{
		}

		TcpBaseClient::~TcpBaseClient()
		{
			m_socket.close();
		}

		//***********************************************************************************************

		TcpAcceptor::TcpAcceptor( uint16_t p_port )
			: m_service{}
			, m_currentClient{ nullptr }
		{
			m_acceptor = std::make_unique< boost::asio::ip::tcp::acceptor >( m_service, boost::asio::ip::tcp::endpoint( boost::asio::ip::tcp::v4(), p_port ) );
		}

		TcpAcceptor::~TcpAcceptor()
		{
			m_service.stop();

			if ( m_thread )
			{
				m_thread->join();
				m_thread.reset();
			}

			m_acceptor->close();
			m_acceptor.reset();
		}

		void TcpAcceptor::Run()
		{
			m_thread = std::make_unique< std::thread >( [this]()
			{
				DoMainLoop();
			} );
		}

		void TcpAcceptor::DeleteClient( TcpBaseClient * p_toDelete )
		{
			if ( p_toDelete )
			{
				std::cout << "DeleteClient : " << &( p_toDelete->m_socket ) << std::endl;
				DoDestroyClient( p_toDelete );
			}
		}

		void TcpAcceptor::EraseClient( TcpBaseClient * p_toDelete )
		{
			std::cout << "EraseClient : " << &( p_toDelete->m_socket ) << std::endl;
			ClientSetIt l_toDel = m_clients.find( p_toDelete );
			ClientSetEndIt l_endIt = m_clients.end();

			if ( l_toDel != l_endIt )
			{
				m_clients.erase( l_toDel );
				DeleteClient( p_toDelete );
			}
		}

		void TcpAcceptor::DoDeleteClients()
		{
			DeleteClient( m_currentClient );

			for ( auto && l_it : m_clients )
			{
				DeleteClient( l_it );
			}

			m_clients.clear();
		}

		void TcpAcceptor::DoAccept()
		{
			m_currentClient = DoCreateClient();
			m_acceptor->async_accept( m_currentClient->GetSocket(), boost::bind( & TcpAcceptor::CallbackAccept, this, boost::asio::placeholders::error ) );
		}

		void TcpAcceptor::DoMainLoop()
		{
			DoAccept();

			while ( true )
			{
				try
				{
					m_service.run();
					break;
				}
				catch ( const boost::exception & p_except )
				{
#if BOOST_VERSION > 103600
					std::cout << "Catch boost::exception, diagnostic_information : [" + diagnostic_information( p_except ) + "]\n";
#else
					std::cout << "Catch boost::exception, diagnostic_information : [" << p_except.diagnostic_information() << "]\n";
#endif
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
#if BOOST_VERSION > 103600
							std::cout << "Catch boost::exception, diagnostic_information : [" + diagnostic_information( p_except2 ) + "]\n";
#else
							std::cout << "Catch boost::exception, diagnostic_information : [" << p_except2.diagnostic_information() << "]\n";
#endif
							General::Utils::set::eraseIterator( m_clients, l_it );
							DeleteClient( l_toTest );
							continue;
						}

						if ( ! l_toTest->GetSocket().is_open() )
						{
							std::cout << "A closed socket was detected in client list... Will be deleted..." << std::endl;
							General::Utils::set::eraseIterator( m_clients, l_it );
							DeleteClient( l_toTest );
							continue;
						}

						l_it ++;
					}
				}
			}
		}

		void TcpAcceptor::CallbackAccept( boost::system::error_code const & p_error )
		{
			if ( p_error )
			{
				std::cout << "Error at CallbackAccept" << std::endl;
				return;
			}

			m_clients.insert( m_currentClient );
			m_currentClient->Start();
			DoAccept();
		}

		//***********************************************************************************************
	}
}
