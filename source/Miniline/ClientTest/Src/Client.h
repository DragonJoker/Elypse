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
		private:
			bool m_connected;
			std::thread m_boostThread;
			boost::asio::io_service * m_service;
			bool m_toDelete;
			bool m_stopped;

		private:
			virtual void CallbackFinishConnect();
			virtual bool CallbackConnectorError( const boost::system::error_code & p_err );
			virtual bool CallbackWriterError( const boost::system::error_code & p_err );
			virtual bool CallbackReaderError( const boost::system::error_code & p_err );
			virtual void CallbackReceivedMessage( const std::string & p_message );
			//virtual void CallbackResolve( const boost::system::error_code & p_err, boost::asio::ip::tcp::resolver::iterator p_resolverIterator);
			virtual void DoMainLoop();
			//virtual void CallbackConnect( const boost::system::error_code & p_err, boost::asio::ip::tcp::resolver::iterator p_resolverIterator);

			void _delete();

		public:
			/*
			virtual void AsyncConnect( const std::string & p_host, unsigned short p_port)
			{
				boost::asio::ip::tcp::resolver::query l_query( p_host, General::Utils::ToString( p_port));

				m_resolver.async_resolve(	l_query,
											boost::bind(	& ElypseTcpClient::CallbackResolve,
															this,
															boost::asio::placeholders::error,
															boost::asio::placeholders::iterator));
			}
			*/



		public:
			ElypseTcpClient( boost::asio::io_service * p_service );
			~ElypseTcpClient();

			void Run();
			void Stop();
			void StopClient();

			void Suicide();
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
			std::string m_clientName;
			std::string m_message;
		};
	}
}

#endif
