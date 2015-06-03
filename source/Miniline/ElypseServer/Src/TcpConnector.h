#ifndef ___GENERAL_TCP_CONNECTOR_H___
#define ___GENERAL_TCP_CONNECTOR_H___

#include <string>

#include "BoostHeaders.h"

#include "Exception.h"
#include "StringConverter.h"

namespace Elypse
{
	namespace Network
	{
		class TcpConnectorBase
		{
		protected:
			virtual void CallbackResolve( const boost::system::error_code & p_err, boost::asio::ip::tcp::resolver::iterator p_resolverIterator )
			{
				if ( !p_err || !CallbackConnectorError( p_err ) )
				{
					std::cout << "CallbackResolve - CallbackConnect\n";

					try
					{
						boost::asio::ip::tcp::endpoint l_endpoint = * p_resolverIterator;
						m_socket.async_connect( l_endpoint,
												boost::bind( &TcpConnectorBase::CallbackConnect,
															 this,
															 boost::asio::placeholders::error,
															 ++p_resolverIterator ) );
					}
					catch ( std::exception & p_exc )
					{
						std::cout << "CallbackResolve - CallbackConnect - Uncaught exception : " << p_exc.what() << "\n";
						//AsyncConnect( m_host, m_port);
					}

					std::cout << "CallbackResolve - CallbackConnect - End\n";
				}
			}

			void CallbackConnect( const boost::system::error_code & p_err, boost::asio::ip::tcp::resolver::iterator p_resolverIterator )
			{
				if ( ! p_err )
				{
					std::cout << "CallbackConnect - CallbackFinishConnect\n";
					CallbackFinishConnect();
				}
				else if ( p_resolverIterator != boost::asio::ip::tcp::resolver::iterator() )
				{
					std::cout << "CallbackConnect - retry\n";
					// The connection failed. Try the next endpoint in the list.
					m_socket.close();
					boost::asio::ip::tcp::endpoint l_endpoint = * p_resolverIterator;
					m_socket.async_connect( l_endpoint,
											boost::bind( &TcpConnectorBase::CallbackConnect,
														 this,
														 boost::asio::placeholders::error,
														 ++ p_resolverIterator ) );
				}
				else if ( CallbackConnectorError( p_err ) )
				{
					//return
				}
			}

		public:
			TcpConnectorBase( boost::asio::io_service & p_service )
				: m_socket( p_service )
				, m_service( p_service )
				, m_resolver( p_service )
			{
			}

			virtual ~TcpConnectorBase()
			{
			}

			virtual boost::system::error_code BlockingConnect( const std::string & p_host, unsigned short p_port )
			{
				m_host = p_host;
				m_port = p_port;
				boost::asio::ip::tcp::resolver::query l_query( p_host, General::Utils::ToString( p_port ) );
				boost::asio::ip::tcp::resolver::iterator endpoint_iterator = m_resolver.resolve( l_query );
				boost::asio::ip::tcp::resolver::iterator end;

				boost::system::error_code l_error = boost::asio::error::host_not_found;

				while ( l_error && endpoint_iterator != end )
				{
					m_socket.close();
					m_socket.connect( *endpoint_iterator++, l_error );
				}

				return l_error;
			}

			virtual void AsyncConnect( const std::string & p_host, unsigned short p_port )
			{
				boost::asio::ip::tcp::resolver::query l_query( p_host, General::Utils::ToString( p_port ) );

				m_resolver.async_resolve( l_query,
										  boost::bind( &TcpConnectorBase::CallbackResolve,
													   this,
													   boost::asio::placeholders::error,
													   boost::asio::placeholders::iterator ) );
			}

			virtual void CallbackFinishConnect() = 0;
			virtual bool CallbackConnectorError( const boost::system::error_code & p_err ) = 0;

		private:
			std::string m_host;
			unsigned short m_port;

		protected:
			boost::asio::ip::tcp::socket m_socket;
			boost::asio::io_service & m_service;
			boost::asio::ip::tcp::resolver m_resolver;
		};
		////	Exemple de connector très simplifié
		//class TcpConnector: public TcpConnectorBase
		//{
		//public:
		//	TcpConnector( boost::asio::io_service & p_service)
		//		:	TcpConnectorBase( p_service)
		//	{
		//	}
		//	virtual ~TcpConnector()
		//	{
		//	}

		//public:
		//	virtual void _finishConnectCallback(){}
		//	virtual bool _errorCallback( const boost::system::error_code & p_err)
		//	{
		//		GENLIB_EXCEPTION( "TcpConnector -> error : " + p_err.message());
		//		return false;
		//	}
		//};
	}
}

#endif
