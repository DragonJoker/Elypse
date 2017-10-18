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
#ifndef ___TCP_CLIENT_H___
#define ___TCP_CLIENT_H___

#include "Module_Network.h"

#include <thread>
#include <OwnedBy.h>
#include <TcpConnector.h>
#include <TcpReader.h>
#include <TcpWriter.h>

#include <boost/asio.hpp>

using boost::asio::ip::tcp;

namespace Elypse
{
	namespace Network
	{
		class d_dll_export ElypseTcpClient
			: public General::Network::TcpConnectorBase
			, public General::Network::TcpReaderBase
			, public General::Network::TcpWriterBase
			, public owned_by<NetworkManager>
			, public named
		{
		public:
			ElypseTcpClient( std::string const & p_name, boost::asio::io_service & p_service, NetworkManager & p_owner );
			~ElypseTcpClient();

			void Run();

			inline boost::asio::io_service & GetService()
			{
				return m_service;
			}

			inline bool IsConnected()
			{
				return m_connected;
			}

		private:
			virtual void CallbackFinishConnect();
			virtual bool CallbackConnectorError( boost::system::error_code const & p_err );
			virtual bool CallbackWriterError( boost::system::error_code const & p_err );
			virtual bool CallbackReaderError( boost::system::error_code const & p_err );
			virtual void CallbackReceivedMessage( std::string const & p_message );

			void DoMainLoop();

		private:
			bool m_connected{ false };
			std::thread * m_thread{ nullptr };
			boost::asio::io_service & m_service;
		};

		struct ElypseMessage
		{
			ElypseMessage( String const & p_name, String const & p_msg )
				: m_clientName{ p_name }
				, m_message{ p_msg }
			{
			}

			explicit operator bool()const
			{
				return !m_clientName.empty() || !m_message.empty();
			}

			String m_clientName;
			String m_message;
		};

		class d_dll_export NetworkManager
			: d_noncopyable
		{
		public:
			NetworkManager();
			~NetworkManager();

		public:
			bool CreateClient( String const & p_name );
			bool Connect( String const & p_name, String const & p_host, uint16_t p_port );
			bool Write( String const & p_name, String const & p_msg );
			ElypseTcpClient * GetTcpClient( String const & p_name )const;
			ElypseMessage GetLastMessage();
			bool CloseClient( String const & p_name );
			void AddMessage( String const & p_name, String const & p_msg );

		protected:
			ElypseTcpClientStrMap m_clients;
			std::vector< ElypseMessage > m_messages;

			bool m_stop;
		};
	}
}

using namespace Elypse::Network;

#endif
