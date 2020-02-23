/*
See LICENSE file in root folder
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
