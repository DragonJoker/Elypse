#ifndef ___ELYPSECLIENT_H___
#define ___ELYPSECLIENT_H___

#include "ElypseService.h"

namespace Elypse
{
	namespace ServerPlugin
	{
		class ElypseTcpClient
			: public Network::TcpBaseClient
			, public Network::TcpReaderBase
			, public Network::TcpWriterBase
			, public std::enable_shared_from_this< ElypseTcpClient >
		{
		protected:
			ElypseTcpClient( std::shared_ptr< ElypseTcpService > p_elypseService )
				: Network::TcpBaseClient( p_elypseService->m_service )
				, Network::TcpReaderBase( Network::TcpBaseClient::m_socket, p_elypseService->m_service )
				, Network::TcpWriterBase( Network::TcpBaseClient::m_socket, p_elypseService->m_service )
				, m_elypseService( p_elypseService )
			{
			}

			virtual bool CallbackReaderError( const boost::system::error_code & p_err )
			{
				std::shared_ptr< ElypseTcpService > l_service = m_elypseService.lock();

				if ( l_service )
				{
					boost::asio::ip::tcp::endpoint l_endPoint = TcpBaseClient::m_socket.remote_endpoint();
					std::string l_remoteAddr = l_endPoint.address().to_string() + ":" + std::to_string( l_endPoint.port() );
					l_endPoint = TcpBaseClient::m_socket.local_endpoint();
					std::string l_localAddr = l_endPoint.address().to_string() + ":" + std::to_string( l_endPoint.port() );

					std::cout << "ElypseTcpClient::CallbackReaderError => Error on socket [" << l_localAddr << "]-[" << l_remoteAddr << "] : [" << p_err.message() << "]" << std::endl;
					l_service->EraseClient( shared_from_this() );
				}

				return false;
			}

			virtual bool CallbackWriterError( const boost::system::error_code & p_err )
			{
				std::shared_ptr< ElypseTcpService > l_service = m_elypseService.lock();

				if ( l_service )
				{
					boost::asio::ip::tcp::endpoint l_endPoint = TcpBaseClient::m_socket.remote_endpoint();
					std::string l_remoteAddr = l_endPoint.address().to_string() + ":" + std::to_string( l_endPoint.port() );
					l_endPoint = TcpBaseClient::m_socket.local_endpoint();
					std::string l_localAddr = l_endPoint.address().to_string() + ":" + std::to_string( l_endPoint.port() );

					std::cout << "ElypseTcpClient::CallbackWriterError => Error on socket [" << l_localAddr << "]-[" << l_remoteAddr << "] : [" << p_err.message() << "]" << std::endl;
					l_service->EraseClient( shared_from_this() );
				}

				return false;
			}

			std::shared_ptr< ElypseTcpService > DoGetService()
			{
				return m_elypseService.lock();
			}

		public:
			virtual ~ElypseTcpClient()
			{
			}

		protected:
			std::weak_ptr< ElypseTcpService > m_elypseService;
		};
	}
}

#endif
