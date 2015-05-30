#ifndef ___ELYPSECLIENT_H___
#define ___ELYPSECLIENT_H___

#include "ElypseService.h"

namespace Elypse
{
	namespace ServerPlugin
	{
		class ElypseTcpClient : public TcpBaseClient, public TcpReaderBase, public TcpWriterBase
		{
		protected:
			ElypseTcpService * m_elypseService;

			ElypseTcpClient( ElypseTcpService * p_elypseService )
				:	TcpBaseClient( p_elypseService->m_service ),
					TcpReaderBase( TcpBaseClient::m_socket, p_elypseService->m_service ),
					TcpWriterBase( TcpBaseClient::m_socket, p_elypseService->m_service ),
					m_elypseService( p_elypseService )
			{
			}

		protected:
			virtual bool _readerErrorCB( const boost::system::error_code & p_err )
			{
				boost::asio::ip::tcp::endpoint l_endPoint = TcpBaseClient::m_socket.remote_endpoint();
				std::string l_remoteAddr = l_endPoint.address().to_string() + ":" + General::Utils::ToString( l_endPoint.port() );
				l_endPoint = TcpBaseClient::m_socket.local_endpoint();
				std::string l_localAddr = l_endPoint.address().to_string() + ":" + General::Utils::ToString( l_endPoint.port() );

				std::cout << "ElypseTcpClient::_errorCallback => Error on socket [" << l_localAddr << "]-[" << l_remoteAddr << "] : [" << p_err.message() << "]" << std::endl;
				m_elypseService->EraseClient( this );

				return false;
			}

			virtual bool _writerErrorCB( const boost::system::error_code & p_err )
			{
				boost::asio::ip::tcp::endpoint l_endPoint = TcpBaseClient::m_socket.remote_endpoint();
				std::string l_remoteAddr = l_endPoint.address().to_string() + ":" + General::Utils::ToString( l_endPoint.port() );
				l_endPoint = TcpBaseClient::m_socket.local_endpoint();
				std::string l_localAddr = l_endPoint.address().to_string() + ":" + General::Utils::ToString( l_endPoint.port() );

				std::cout << "ElypseTcpClient::_errorCallback => Error on socket [" << l_localAddr << "]-[" << l_remoteAddr << "] : [" << p_err.message() << "]" << std::endl;
				m_elypseService->EraseClient( this );

				return false;
			}

		public:
			virtual ~ElypseTcpClient()
			{
			}
		};
	}
}

#endif
