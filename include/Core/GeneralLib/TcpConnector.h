/*
See LICENSE file in root folder
*/
#ifndef ___GENERAL_TCP_CONNECTOR_H___
#define ___GENERAL_TCP_CONNECTOR_H___

#include <string>
#include <iostream>

#include "Exception.h"
#include "StringConverter.h"

#include <boost/asio.hpp>
#include <boost/bind.hpp>

namespace General
{
	namespace Network
	{
		class TcpConnectorBase
		{
		public:
			TcpConnectorBase( boost::asio::io_service & p_service )
				: m_socket{ p_service }
				, m_service{ p_service }
				, m_resolver{ p_service }
			{
			}
			virtual ~TcpConnectorBase()
			{
			}

			virtual boost::system::error_code BlockingConnect( std::string const & p_host, uint16_t p_port )
			{
				m_host = p_host;
				m_port = p_port;
				boost::asio::ip::tcp::resolver::query l_query{ p_host, General::Utils::ToString( p_port ) };
				boost::asio::ip::tcp::resolver::iterator endpoint_iterator{ m_resolver.resolve( l_query ) };
				boost::asio::ip::tcp::resolver::iterator end;
				boost::system::error_code l_error{ boost::asio::error::host_not_found };

				while ( l_error && endpoint_iterator != end )
				{
					m_socket.close();
					m_socket.connect( * endpoint_iterator ++, l_error );
				}

				return l_error;
			}

			virtual void AsyncConnect( std::string const & p_host, uint16_t p_port )
			{
				boost::asio::ip::tcp::resolver::query l_query{ p_host, General::Utils::ToString( p_port ) };
				m_resolver.async_resolve( l_query,
										  boost::bind( &TcpConnectorBase::CallbackResolve,
													   this,
													   boost::asio::placeholders::error,
													   boost::asio::placeholders::iterator ) );
			}

		protected:
			virtual void CallbackFinishConnect() = 0;
			virtual bool CallbackConnectorError( boost::system::error_code const & p_err ) = 0;

			virtual void CallbackResolve( boost::system::error_code const & p_err, boost::asio::ip::tcp::resolver::iterator p_resolverIterator )
			{
				if ( p_err && CallbackConnectorError( p_err ) )
				{
					return;
				}

				try
				{
					auto l_endpoint = *p_resolverIterator;
					m_socket.async_connect( l_endpoint,
											boost::bind( &TcpConnectorBase::CallbackConnect,
														 this,
														 boost::asio::placeholders::error,
														 ++p_resolverIterator ) );
				}
				catch ( std::exception & p_exc )
				{
					std::cout << "CallbackResolve - CallbackConnect - Uncaught exception : " << p_exc.what() << "\n";
				}
			}

			void CallbackConnect( boost::system::error_code const & p_err, boost::asio::ip::tcp::resolver::iterator p_resolverIterator )
			{
				if ( !p_err )
				{
					CallbackFinishConnect();
				}
				else if ( p_resolverIterator != boost::asio::ip::tcp::resolver::iterator() )
				{
					// The connection failed. Try the next endpoint in the list.
					m_socket.close();
					auto l_endpoint = *p_resolverIterator;
					m_socket.async_connect( l_endpoint,
											boost::bind( &TcpConnectorBase::CallbackConnect,
														 this,
														 boost::asio::placeholders::error,
														 ++p_resolverIterator ) );
				}
				else if ( CallbackConnectorError( p_err ) )
				{
					//return
				}
			}

		protected:
			boost::asio::ip::tcp::socket m_socket;
			boost::asio::io_service & m_service;
			boost::asio::ip::tcp::resolver m_resolver;

		private:
			std::string m_host;
			uint16_t m_port{ 0 };
		};
	}
}

#endif
