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
#include <set>

#include <thread>
#include "StringConverter.h"
#include "STLSetMacros.h"
#include "FunctionTracker.h"

#include <boost/exception/diagnostic_information.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

namespace General
{
	namespace Network
	{
		class TcpBaseClient
		{
		public:
			TcpBaseClient( boost::asio::io_service & p_service );
			virtual ~TcpBaseClient();

			virtual void Start() = 0;

			inline boost::asio::ip::tcp::socket & GetSocket()
			{
				return m_socket;
			}

		public:
			boost::asio::ip::tcp::socket m_socket;
		};

		typedef std::set <TcpBaseClient *> ClientSet;
		typedef std::set <TcpBaseClient *> ::iterator ClientSetIt;
		typedef const std::set <TcpBaseClient *> ::const_iterator & ClientSetEndIt;

		class TcpAcceptor
		{
		public:
			TcpAcceptor( uint16_t p_port );

			virtual ~TcpAcceptor();

			virtual void Run();
			virtual void DeleteClient( TcpBaseClient * p_toDelete );
			virtual void EraseClient( TcpBaseClient * p_toDelete );

		protected:
			void DoDeleteClients();
			void DoAccept();

			virtual TcpBaseClient * DoCreateClient() = 0;
			virtual void DoDestroyClient( TcpBaseClient * p_toDelete ) = 0;

			virtual void DoMainLoop();
			virtual void CallbackAccept( boost::system::error_code const & p_error );

		private:
			std::unique_ptr< std::thread > m_thread;

		protected:
			std::unique_ptr< boost::asio::ip::tcp::acceptor > m_acceptor;

			ClientSet m_clients;
			TcpBaseClient * m_currentClient{ nullptr };

		public:
			boost::asio::io_service m_service;
		};
	}
}


#endif
