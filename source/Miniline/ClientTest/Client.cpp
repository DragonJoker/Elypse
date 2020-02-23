/*
See LICENSE file in root folder
*/

#include "Client.h"
#include <FunctionTracker.h>

#include <boost/exception/diagnostic_information.hpp>

using namespace boost;

namespace Elypse
{
	namespace Network
	{
		ElypseTcpClient::ElypseTcpClient( asio::io_service * p_service )
			: General::Network::TcpConnectorBase( *p_service )
			, General::Network::TcpReaderBase( General::Network::TcpConnectorBase::m_socket, *p_service )
			, General::Network::TcpWriterBase( General::Network::TcpConnectorBase::m_socket, *p_service )
			, m_service( p_service )
			, m_connected( false )
			, m_toDelete( false )
			, m_stopped( false )
		{
		}

		ElypseTcpClient::~ElypseTcpClient()
		{
			std::cout << "~ElypseTcpClient : " << std::endl;
			//TcpConnectorBase::m_socket.close();
			//m_service->stop();

			if ( m_boostThread.joinable() )
			{
				m_boostThread.join();
			}

			// delete service in main app
			//delete m_service;
			//m_service = nullptr;
		}


		struct SocketKiller
		{
		public:
			ElypseTcpClient * m_instance;

		public:
			SocketKiller( ElypseTcpClient * p_instance )
				:	m_instance( p_instance )
			{
			}
			void operator()()const
			{
				delete m_instance;
			}
		};

		void ElypseTcpClient::Stop()
		{
			if ( ! m_stopped )
			{
				TcpConnectorBase::m_socket.close();
				m_stopped = true;
				m_noSend = true;
				//m_service->post( SocketKiller( this ) );
				//m_service->post( [this](){ Suicide() } );
			}
		}

		void ElypseTcpClient::StopClient()
		{
			TcpConnectorBase::m_socket.close();
			m_toDelete = true;
		}

		void ElypseTcpClient::DoMainLoop()
		{
			try
			{
				m_service->run();
			}
			catch ( exception & p_exc )
			{
#if BOOST_VERSION > 103600
				std::cout << "ERROR : NetworkManager::DoMainLoop - [" + diagnostic_information( p_exc ) + "]\n";
#else
				std::cout << "ERROR : NetworkManager::DoMainLoop - [" << p_exc.diagnostic_information() << "]\n";
#endif
			}
		}

		//void ElypseTcpClient::CallbackResolve( boost::system::error_code const & p_err, boost::asio::ip::tcp::resolver::iterator p_resolverIterator)
		//{
		//	std::cout << "ElypseTcpClient::CallbackResolve - CallbackConnect\n";
		//	if (_checkOk()){ d_coucou; return;}
		//	if (p_err && ! CallbackError( p_err))
		//	{
		//		return;
		//	}
		//	try
		//	{
		//		boost::asio::ip::tcp::endpoint l_endpoint = * p_resolverIterator;
		//		TcpConnectorBase::m_socket.async_connect( l_endpoint,
		//			boost::bind(						& ElypseTcpClient::CallbackConnect,
		//												this,
		//												boost::asio::placeholders::error,
		//												++ p_resolverIterator));
		//	}
		//	catch ( std::exception & p_exc )
		//	{
		//		std::cout << "ElypseTcpClient::CallbackResolve - CallbackConnect - Uncaught exception : " << p_exc.what() << "\n";
		//		//AsyncConnect( m_host, m_port);
		//	}
		//	//std::cout << "ElypseTcpClient::CallbackResolve - CallbackConnect - End\n";
		//}

		//void ElypseTcpClient::CallbackConnect( boost::system::error_code const & p_err, boost::asio::ip::tcp::resolver::iterator p_resolverIterator)
		//{
		//	if ( ! p_err)
		//	{
		//		CallbackFinishConnect();
		//		return;
		//	}

		//	if (_checkOk()){ d_coucou; return;}
		//	else if (p_resolverIterator != boost::asio::ip::tcp::resolver::iterator())
		//	{
		//		std::cout << "CallbackConnect - retry\n";
		//		// The connection failed. Try the next endpoint in the list.
		//		TcpConnectorBase::m_socket.close();
		//		boost::asio::ip::tcp::endpoint l_endpoint = * p_resolverIterator;
		//		TcpConnectorBase::m_socket.async_connect( l_endpoint,
		//								boost::bind(	& ElypseTcpClient::CallbackConnect,
		//												this,
		//												boost::asio::placeholders::error,
		//												++ p_resolverIterator));
		//	}
		//	else
		//	{
		//		//std::cout << "CallbackConnect - else\n";
		//		//CallbackError( p_err);
		//	}
		//}

		void ElypseTcpClient::CallbackFinishConnect()
		{
			std::cout << "ElypseTcpClient::CallbackFinishConnect\n";
			//if (_checkOk()) return;
			m_connected = true;

			StartSending();
			StartAsyncRead();
		}

		void ElypseTcpClient::CallbackReceivedMessage( std::string const & p_message )
		{
			//if (_checkOk()){ d_coucou; return;}
			//std::cout << "Received a message\n";
		}

		bool ElypseTcpClient::CallbackWriterError( boost::system::error_code const & p_err )
		{
			Stop();

			//if (_checkOk()){ d_coucou; return true;}
			//try
			//{
			//	boost::asio::ip::tcp::endpoint l_endPoint = TcpConnectorBase::m_socket.remote_endpoint();
			//	std::string l_remoteAddr = l_endPoint.address().to_string() + ":" + General::Utils::ToString( l_endPoint.port());
			//	l_endPoint = TcpConnectorBase::m_socket.local_endpoint();
			//	std::string l_localAddr = l_endPoint.address().to_string() + ":" + General::Utils::ToString( l_endPoint.port());
			//	std::cout << "ElypseTcpClient::CallbackWriterError : [" << l_remoteAddr << "]-[" << l_localAddr << "]\n";
			//}
			//catch (...)
			//{
			//	std::cout << "ElypseTcpClient::CallbackWriterError : not connected" << std::endl;;
			//}

			m_connected = false;
			return true;
		}

		bool ElypseTcpClient::CallbackReaderError( boost::system::error_code const & p_err )
		{
			m_service->post( SocketKiller( this ) ); //boost::bind( & ElypseTcpClient::Suicide, this));
			//Stop();

			//if (_checkOk()){ d_coucou; return true;}
			//try
			//{
			//	if (TcpConnectorBase::m_socket.is_open())
			//	{
			//		boost::asio::ip::tcp::endpoint l_endPoint = TcpConnectorBase::m_socket.remote_endpoint();
			//		std::string l_remoteAddr = l_endPoint.address().to_string() + ":" + General::Utils::ToString( l_endPoint.port());
			//		l_endPoint = TcpConnectorBase::m_socket.local_endpoint();
			//		std::string l_localAddr = l_endPoint.address().to_string() + ":" + General::Utils::ToString( l_endPoint.port());
			//		std::cout << "ElypseTcpClient::CallbackReaderError : [" << l_remoteAddr << "]-[" << l_localAddr << "]\n";
			//	}
			//}
			//catch (...)
			//{
			//	std::cout << "ElypseTcpClient::CallbackReaderError : not connected" << std::endl;;
			//}

			m_connected = false;
			return true;
		}

		bool ElypseTcpClient::CallbackConnectorError( boost::system::error_code const & p_err )
		{
			std::cout << "ElypseTcpClient::CallbackConnectorError: " << p_err.message() << "\n";
			Stop();

			//if (_checkOk()) return true;
			//try
			//{
			//	boost::asio::ip::tcp::endpoint l_endPoint = TcpConnectorBase::m_socket.remote_endpoint();
			//	std::string l_remoteAddr = l_endPoint.address().to_string() + ":" + General::Utils::ToString( l_endPoint.port());
			//	l_endPoint = TcpConnectorBase::m_socket.local_endpoint();
			//	std::string l_localAddr = l_endPoint.address().to_string() + ":" + General::Utils::ToString( l_endPoint.port());
			//	std::cout << "ElypseTcpClient::CallbackError : [" << l_remoteAddr << "]-[" << l_localAddr << "]\n";
			//}
			//catch (...)
			//{
			//	std::cout << "ElypseTcpClient::CallbackError : not connected" << std::endl;;
			//}

			m_connected = false;

			return true;
		}
	}
}
