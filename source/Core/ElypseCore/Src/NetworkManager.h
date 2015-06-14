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

#include <Thread.h>
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
		private:
			bool m_connected;
			Thread * m_boostThread;
			boost::asio::io_service * m_service;

		private:
			virtual void CallbackFinishConnect();
			virtual bool CallbackConnectorError( const boost::system::error_code & p_err );
			virtual bool CallbackWriterError( const boost::system::error_code & p_err );
			virtual bool CallbackReaderError( const boost::system::error_code & p_err );
			virtual void CallbackReceivedMessage( const std::string & p_message );

			void DoMainLoop();

		public:
			ElypseTcpClient( const std::string & p_name, boost::asio::io_service * p_service, NetworkManager * p_owner );
			~ElypseTcpClient();

			void Run();
			inline boost::asio::io_service * GetService()
			{
				return m_service;
			}

		public:
			inline bool IsConnected()
			{
				return m_connected;
			}
		};

		struct ElypseMessage
		{
			String m_clientName;
			String m_message;

			ElypseMessage( const String & p_name, const String & p_msg )
				: m_clientName( p_name ),
					m_message( p_msg )
			{
			}
		};

		class d_dll_export NetworkManager
		{
		protected:
			ElypseTcpClientStrMap m_clients;
			std::vector <ElypseMessage *> m_messages;

			bool m_stop;

		public:
			NetworkManager();
			~NetworkManager();

		public:
			bool CreateClient( const String & p_name );
			bool Connect( const String & p_name, const String & p_host, unsigned short p_port );
			bool Write( const String & p_name, const String & p_msg );
			ElypseTcpClient * GetTcpClient( const String & p_name )const;
			ElypseMessage * GetLastMessage();
			bool CloseClient( const String & p_name );
			void AddMessage( const String & p_name, const String & p_msg );
		};
	}
}

using namespace Elypse::Network;

#endif
