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
#ifndef ___ELYPSECLIENT_H___
#define ___ELYPSECLIENT_H___

#include "ElypseService.h"

namespace Elypse
{
	namespace Server
	{
		class ElypseTcpClient
			: public General::Network::TcpBaseClient
			, public General::Network::TcpReaderBase
			, public General::Network::TcpWriterBase
		{
		protected:
			ElypseTcpClient( std::shared_ptr< ElypseTcpService > p_elypseService )
				: General::Network::TcpBaseClient( p_elypseService->m_service )
				, General::Network::TcpReaderBase( General::Network::TcpBaseClient::m_socket, p_elypseService->m_service )
				, General::Network::TcpWriterBase( General::Network::TcpBaseClient::m_socket, p_elypseService->m_service )
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

					std::cerr << "ElypseTcpClient::CallbackReaderError => Error on socket [" << l_localAddr << "]-[" << l_remoteAddr << "] : [" << p_err.message() << "]" << std::endl;
					l_service->EraseClient( this );
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

					std::cerr << "ElypseTcpClient::CallbackWriterError => Error on socket [" << l_localAddr << "]-[" << l_remoteAddr << "] : [" << p_err.message() << "]" << std::endl;
					l_service->EraseClient( this );
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
