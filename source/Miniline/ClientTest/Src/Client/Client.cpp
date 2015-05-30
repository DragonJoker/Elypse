//
// chat_client.cpp
// ~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "Client.h"
#include <General/FunctionTracker.h>

using namespace boost;

ElypseTcpClient :: ElypseTcpClient( asio::io_service * p_service)
	:	TcpConnectorBase			( *p_service),
		TcpReaderBase				( TcpConnectorBase::m_socket, *p_service),
		TcpWriterBase				( TcpConnectorBase::m_socket, *p_service),
		m_service					( p_service),
		m_connected					( false),
		m_boostThread				( NULL),
		m_toDelete					( false),
		m_stopped					( false)
{
}

ElypseTcpClient :: ~ElypseTcpClient()
{
	std::cout << "~ElypseTcpClient : " << std::endl;
//	TcpConnectorBase::m_socket.close();
//	m_service->stop();

	if (m_boostThread != NULL)
	{
		GENLIB_THREAD_WAIT_FOR_END_OF( m_boostThread);
		delete m_boostThread;
	}
	// delete service in main app
//	delete m_service;
//	m_service = NULL;
}


	struct SocketKiller
	{
	public:
		ElypseTcpClient * m_instance;

	public:
		SocketKiller( ElypseTcpClient * p_instance)
			:	m_instance (p_instance)
		{
		}
		void operator()()const
		{
			delete m_instance;
		}
	};

void ElypseTcpClient :: Stop()
{
	if ( ! m_stopped)
	{
		TcpConnectorBase::m_socket.close();
		m_stopped = true;
		m_noSend = true;
//		m_service->post( SocketKiller( this));//boost::bind( & ElypseTcpClient::Suicide, this));
	//	m_service->post( GENLIB_THREAD_CLASS_FUNCTOR( this, ElypseTcpClient, Suicide));
	}
}

void ElypseTcpClient :: StopClient()
{
	TcpConnectorBase::m_socket.close();
	m_toDelete = true;
}

void ElypseTcpClient :: _mainLoop()
{
	try
	{
		m_service->run();
	}
	catch ( exception & p_exc)
	{
#	if BOOST_VERSION > 103600
		std::cout << "ERROR : NetworkManager :: _mainLoop - [" + diagnostic_information( p_exc) + "]\n";
#	else
		std::cout << "ERROR : NetworkManager :: _mainLoop - [" << p_exc.diagnostic_information() << "]\n";
#	endif
	}
}
/*
void ElypseTcpClient :: _resolveCallBack( const boost::system::error_code & p_err, boost::asio::ip::tcp::resolver::iterator p_resolverIterator)
{
	std::cout << "ElypseTcpClient :: _resolveCallBack - _connectCallBack\n";
	if (_checkOk()){ d_coucou; return;}
	if (p_err && ! _errorCallback( p_err))
	{
		return;
	}
	try
	{
		boost::asio::ip::tcp::endpoint l_endpoint = * p_resolverIterator;
		TcpConnectorBase::m_socket.async_connect( l_endpoint,
			boost::bind(						& ElypseTcpClient::_connectCallBack,
												this,
												boost::asio::placeholders::error,
												++ p_resolverIterator));
	}
	catch ( std::exception & p_exc )
	{
		std::cout << "ElypseTcpClient :: _resolveCallBack - _connectCallBack - Uncaught exception : " << p_exc.what() << "\n";
//				AsyncConnect( m_host, m_port);
	}
//	std::cout << "ElypseTcpClient :: _resolveCallBack - _connectCallBack - End\n";
}


void ElypseTcpClient :: _connectCallBack( const boost::system::error_code & p_err, boost::asio::ip::tcp::resolver::iterator p_resolverIterator)
{
	if ( ! p_err)
	{
		_finishConnectCallback();
		return;
	}

	if (_checkOk()){ d_coucou; return;}
	else if (p_resolverIterator != boost::asio::ip::tcp::resolver::iterator())
	{
		std::cout << "_connectCallBack - retry\n";
		// The connection failed. Try the next endpoint in the list.
		TcpConnectorBase::m_socket.close();
		boost::asio::ip::tcp::endpoint l_endpoint = * p_resolverIterator;
		TcpConnectorBase::m_socket.async_connect( l_endpoint,
								boost::bind(	& ElypseTcpClient::_connectCallBack,
												this,
												boost::asio::placeholders::error,
												++ p_resolverIterator));
	}
	else
	{
//		std::cout << "_connectCallBack - else\n";
//				_errorCallback( p_err);
	}
}
*/

void ElypseTcpClient :: _finishConnectCallback()
{
	std::cout << "ElypseTcpClient :: _finishConnectCallback\n";
//	if (_checkOk()) return;
	m_connected = true;

	StartSending();
	StartAsyncRead();
}

void ElypseTcpClient :: _receivedMessageCallback( const std::string & p_message)
{
//	if (_checkOk()){ d_coucou; return;}
//	std::cout << "Received a message\n";
}

bool ElypseTcpClient :: _writerErrorCB( const boost::system::error_code & p_err)
{
	Stop();
/*
	if (_checkOk()){ d_coucou; return true;}
	try
	{
		boost::asio::ip::tcp::endpoint l_endPoint = TcpConnectorBase::m_socket.remote_endpoint();
		std::string l_remoteAddr = l_endPoint.address().to_string() + ":" + General::Utils::ToString( l_endPoint.port());
		l_endPoint = TcpConnectorBase::m_socket.local_endpoint();
		std::string l_localAddr = l_endPoint.address().to_string() + ":" + General::Utils::ToString( l_endPoint.port());
		std::cout << "ElypseTcpClient :: _writerErrorCB : [" << l_remoteAddr << "]-[" << l_localAddr << "]\n";
	}
	catch (...)
	{
		std::cout << "ElypseTcpClient :: _writerErrorCB : not connected" << std::endl;;
	}

	*/
	m_connected = false;
	return true;
}

bool ElypseTcpClient :: _readerErrorCB( const boost::system::error_code & p_err)
{
	m_service->post( SocketKiller( this));//boost::bind( & ElypseTcpClient::Suicide, this));
//	Stop();
	/*
	if (_checkOk()){ d_coucou; return true;}
	try
	{
		if (TcpConnectorBase::m_socket.is_open())
		{
			boost::asio::ip::tcp::endpoint l_endPoint = TcpConnectorBase::m_socket.remote_endpoint();
			std::string l_remoteAddr = l_endPoint.address().to_string() + ":" + General::Utils::ToString( l_endPoint.port());
			l_endPoint = TcpConnectorBase::m_socket.local_endpoint();
			std::string l_localAddr = l_endPoint.address().to_string() + ":" + General::Utils::ToString( l_endPoint.port());
			std::cout << "ElypseTcpClient :: _readerErrorCB : [" << l_remoteAddr << "]-[" << l_localAddr << "]\n";
		}
	}
	catch (...)
	{
		std::cout << "ElypseTcpClient :: _readerErrorCB : not connected" << std::endl;;
	}
	*/
	m_connected = false;
	return true;
}

bool ElypseTcpClient :: _connectorErrorCB( const boost::system::error_code & p_err)
{
	Stop();
	/*
	if (_checkOk()) return true;
	try
	{
		boost::asio::ip::tcp::endpoint l_endPoint = TcpConnectorBase::m_socket.remote_endpoint();
		std::string l_remoteAddr = l_endPoint.address().to_string() + ":" + General::Utils::ToString( l_endPoint.port());
		l_endPoint = TcpConnectorBase::m_socket.local_endpoint();
		std::string l_localAddr = l_endPoint.address().to_string() + ":" + General::Utils::ToString( l_endPoint.port());
		std::cout << "ElypseTcpClient :: _errorCallback : [" << l_remoteAddr << "]-[" << l_localAddr << "]\n";
	}
	catch (...)
	{
		std::cout << "ElypseTcpClient :: _errorCallback : not connected" << std::endl;;
	}
*/
	m_connected = false;

	return true;
}