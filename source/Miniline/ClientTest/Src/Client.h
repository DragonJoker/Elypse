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
