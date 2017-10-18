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

#include <TcpWriter.h>
#include <TcpReader.h>
#include <TcpConnector.h>

#include <thread>

namespace Elypse
{
	namespace Network
	{
		class ElypseTcpClient
			: public General::Network::TcpConnectorBase
			, public General::Network::TcpReaderBase
			, public General::Network::TcpWriterBase
		{
		public:
			ElypseTcpClient( boost::asio::io_service * p_service );
			~ElypseTcpClient();

			void Run();
			void Stop();
			void StopClient();

			inline boost::asio::io_service * GetService()
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
			virtual void DoMainLoop();

		private:
			bool m_connected;
			std::thread m_boostThread;
			boost::asio::io_service * m_service;
			bool m_toDelete;
			bool m_stopped;
		};

		struct ElypseMessage
		{
			std::string m_clientName;
			std::string m_message;
		};
	}
}

#endif
