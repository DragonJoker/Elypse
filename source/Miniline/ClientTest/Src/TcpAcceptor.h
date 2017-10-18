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
#ifndef ___TCP_ACCEPTOR_H___
#define ___TCP_ACCEPTOR_H___

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <set>

#include <STLSetMacros.h>

namespace Elypse
{
	namespace Network
	{
		class TcpBaseClient
		{
		protected:
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

		class TcpAcceptor
		{
		protected:
			boost::asio::io_service m_service;
			boost::asio::ip::tcp::acceptor m_acceptor;

			std::set< TcpBaseClient * > m_clients;

			TcpBaseClient * m_currentClient;

		public:
			TcpAcceptor( uint16_t p_port )
				: m_service()
				, m_acceptor( m_service, boost::asio::ip::tcp::endpoint( boost::asio::ip::tcp::v4(), p_port ) )
			{
			}

			virtual ~TcpAcceptor()
			{
				m_acceptor.close();
				delete m_currentClient;
				General::Utils::set::deleteAll( m_clients );
			}

		protected:
			virtual void DoAccept()
			{
				m_currentClient = DoCreateClient();
				m_acceptor.async_accept( m_currentClient->GetSocket(), boost::bind( &TcpAcceptor::CallbackAccept, this, boost::asio::placeholders::error ) );
			}

		private:
			virtual TcpBaseClient * DoCreateClient() = 0;

			virtual void CallbackAccept( boost::system::error_code const & error )
			{
				if ( error )
				{
					return;
				}

				m_currentClient->Start();
				m_clients.insert( m_currentClient );
				DoAccept();
			}

		public:
			void Run()
			{
				m_service.run();
			}
		};
	}
}

#endif
