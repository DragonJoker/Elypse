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

namespace EMuse
{
	namespace Network
	{
		class d_dll_export ElypseTcpClient : public TcpConnectorBase, public TcpReaderBase, public TcpWriterBase, public owned_by<NetworkManager>, public named
		{
		private:
			bool m_connected;
			Thread * m_boostThread;
			boost::asio::io_service * m_service;

		private:
			virtual void _finishConnectCallback();
			virtual bool _connectorErrorCB( const boost::system::error_code & p_err );
			virtual bool _writerErrorCB( const boost::system::error_code & p_err );
			virtual bool _readerErrorCB( const boost::system::error_code & p_err );
			virtual void _receivedMessageCallback( const std::string & p_message );

			void _mainLoop();

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
				:	m_clientName( p_name ),
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

using namespace EMuse::Network;

#endif
