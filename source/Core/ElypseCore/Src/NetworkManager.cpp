/*
This source file is part of ElypsePlayer (https://sourceforge.net/projects/elypse/)

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
the program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
*/
#include "PrecompiledHeader.h"

#include "NetworkManager.h"
#include "ElypseLogs.h"

#include <boost/exception/diagnostic_information.hpp>

using namespace boost;

ElypseTcpClient::ElypseTcpClient( const std::string & p_name, asio::io_service * p_service, NetworkManager * p_owner )
	: TcpConnectorBase( *p_service )
	, General::Network::TcpReaderBase( General::Network::TcpConnectorBase::m_socket, *p_service )
	, General::Network::TcpWriterBase( General::Network::TcpConnectorBase::m_socket, *p_service )
	, owned_by <NetworkManager>	( p_owner )
	, named( p_name )
	, m_service( p_service )
	, m_connected( false )
{
}

ElypseTcpClient::~ElypseTcpClient()
{
	m_service->stop();

	if ( m_boostThread != NULL )
	{
		GENLIB_THREAD_WAIT_FOR_END_OF( m_boostThread );
		delete m_boostThread;
	}

	// delete service in main app
}

void ElypseTcpClient::Run()
{
	m_boostThread = new Thread( GENLIB_THREAD_CLASS_FUNCTOR( this, ElypseTcpClient, DoMainLoop ) );
}

void ElypseTcpClient::DoMainLoop()
{
	try
	{
		m_service->run();
	}
	catch ( exception & p_exc )
	{
#	if BOOST_VERSION > 103600
		EMUSE_LOG_MESSAGE_RELEASE( "ERROR : NetworkManager::DoMainLoop - [" + diagnostic_information( p_exc ) + "]" );
#	else
		EMUSE_LOG_MESSAGE_RELEASE( "ERROR : NetworkManager::DoMainLoop - [" + String( p_exc.diagnostic_information() ) + "]" );
#	endif
		m_owner->AddMessage( m_name, "disconnected" );
	}
}

void ElypseTcpClient::CallbackFinishConnect()
{
//	std::cout << "ElypseTcpClient::CallbackFinishConnect\n";
	EMUSE_LOG_MESSAGE_RELEASE( "ElypseTcpClient::CallbackFinishConnect" );
	StartSending();
	StartAsyncRead();
	m_connected = true;
}

void ElypseTcpClient::CallbackReceivedMessage( const std::string & p_message )
{
	m_owner->AddMessage( m_name, p_message );
}

bool ElypseTcpClient::CallbackWriterError( const boost::system::error_code & p_err )
{
	try
	{
		boost::asio::ip::tcp::endpoint l_endPoint = TcpConnectorBase::m_socket.remote_endpoint();
		std::string l_remoteAddr = l_endPoint.address().to_string() + ":" + General::Utils::ToString( l_endPoint.port() );
		l_endPoint = TcpConnectorBase::m_socket.local_endpoint();
		std::string l_localAddr = l_endPoint.address().to_string() + ":" + General::Utils::ToString( l_endPoint.port() );
		EMUSE_LOG_MESSAGE_RELEASE( "ElypseTcpClient::CallbackWriterError : [" + l_remoteAddr + "]-[" + l_localAddr + "] -> " + p_err.message() );
	}
	catch ( ... )
	{
		EMUSE_LOG_MESSAGE_RELEASE( "ElypseTcpClient::CallbackError : not connected -> " + p_err.message() );
	}

	m_owner->AddMessage( m_name, "disconnected" );
	m_connected = false;
	return false;
}

bool ElypseTcpClient::CallbackReaderError( const boost::system::error_code & p_err )
{
	try
	{
		boost::asio::ip::tcp::endpoint l_endPoint = TcpConnectorBase::m_socket.remote_endpoint();
		std::string l_remoteAddr = l_endPoint.address().to_string() + ":" + General::Utils::ToString( l_endPoint.port() );
		l_endPoint = TcpConnectorBase::m_socket.local_endpoint();
		std::string l_localAddr = l_endPoint.address().to_string() + ":" + General::Utils::ToString( l_endPoint.port() );
		EMUSE_LOG_MESSAGE_RELEASE( "ElypseTcpClient::CallbackReaderError : [" + l_remoteAddr + "]-[" + l_localAddr + "] -> " + p_err.message() );
	}
	catch ( ... )
	{
		EMUSE_LOG_MESSAGE_RELEASE( "ElypseTcpClient::CallbackError : not connected -> "  + p_err.message() );
	}

	m_owner->AddMessage( m_name, "disconnected" );
	m_connected = false;
	return false;
}

bool ElypseTcpClient::CallbackConnectorError( const boost::system::error_code & p_err )
{
	try
	{
		boost::asio::ip::tcp::endpoint l_endPoint = TcpConnectorBase::m_socket.remote_endpoint();
		std::string l_remoteAddr = l_endPoint.address().to_string() + ":" + General::Utils::ToString( l_endPoint.port() );
		l_endPoint = TcpConnectorBase::m_socket.local_endpoint();
		std::string l_localAddr = l_endPoint.address().to_string() + ":" + General::Utils::ToString( l_endPoint.port() );
		EMUSE_LOG_MESSAGE_RELEASE( "ElypseTcpClient::CallbackError : [" + l_remoteAddr + "]-[" + l_localAddr + "] -> " + p_err.message() );
	}
	catch ( ... )
	{
		EMUSE_LOG_MESSAGE_RELEASE( "ElypseTcpClient::CallbackError : not connected -> " + p_err.message() );
	}

	m_owner->AddMessage( m_name, "disconnected" );
	m_connected = false;
	return false;
}

NetworkManager::NetworkManager()
	: m_stop( false )
{
}

NetworkManager::~NetworkManager()
{
	m_stop = true;
	asio::io_service * l_service;

	for ( ElypseTcpClientStrMap::iterator l_it = m_clients.begin() ; l_it != m_clients.end() ; ++l_it )
	{
		l_service = l_it->second->GetService();
		delete l_it->second;
		delete l_service;
	}

	m_clients.clear();
	General::Utils::vector::deleteAll( m_messages );
}

bool NetworkManager::CreateClient( const String & p_name )
{
	ElypseTcpClientStrMap::iterator l_it = m_clients.find( p_name );

	if ( l_it != m_clients.end() )
	{
		return false;
	}

	asio::io_service * l_service = new asio::io_service();
	ElypseTcpClient * l_client = new ElypseTcpClient( p_name, l_service, this );
	m_clients.insert( std::make_pair( p_name, l_client ) );
	return true;
}

bool NetworkManager::Connect( const String & p_name, const String & p_host, unsigned short p_port )
{
	ElypseTcpClientStrMap::iterator l_it = m_clients.find( p_name );

	if ( l_it == m_clients.end() )
	{
		return false;
	}

	l_it->second->AsyncConnect( p_host, p_port );
	l_it->second->Run();
	return true;
}

bool NetworkManager::Write( const String & p_name, const String & p_msg )
{
	ElypseTcpClientStrMap::iterator l_it = m_clients.find( p_name );

	if ( l_it == m_clients.end() )
	{
		return false;
	}

	if ( l_it->second->IsConnected() )
	{
		l_it->second->AsyncSend( p_msg );
	}
	else
	{
		l_it->second->StoreMessage( p_msg );
	}

	return true;
}

ElypseTcpClient * NetworkManager::GetTcpClient( const String & p_name )const
{
	ElypseTcpClientStrMap::const_iterator l_it = m_clients.find( p_name );

	if ( l_it == m_clients.end() )
	{
		return NULL;
	}

	return l_it->second;
}

ElypseMessage * NetworkManager::GetLastMessage()
{
	if ( m_messages.size() <= 0 )
	{
		return NULL;
	}

	ElypseMessage * l_msg = m_messages[0];
	General::Utils::vector::eraseValue( m_messages, l_msg );
	return l_msg;
}

bool NetworkManager::CloseClient( const String & p_name )
{
	ElypseTcpClientStrMap::iterator l_it = m_clients.find( p_name );

	if ( l_it == m_clients.end() )
	{
		return false;
	}

	ElypseTcpClient * l_client = l_it->second;
	m_clients.erase( l_it );
	delete l_client;
	return true;
}

void NetworkManager::AddMessage( const String & p_name, const String & p_msg )
{
	m_messages.push_back( new ElypseMessage( p_name, p_msg ) );
}
