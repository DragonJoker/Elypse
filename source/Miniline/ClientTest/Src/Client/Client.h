#ifndef ___TCP_CLIENT_H___
#define ___TCP_CLIENT_H___

#ifndef __GNUG__
#	define _WIN32_WINNT 0x0501
#endif

#include "../Common/Common.h"

#include <General/TcpWriter.h>
#include <General/TcpReader.h>
#include <General/TcpConnector.h>

#include <General/Thread.h>
#include <General/OwnedBy.h>

namespace General
{ namespace MultiThreading
{
	class ElypseTcpClient : public TcpConnectorBase, public TcpReaderBase, public TcpWriterBase
	{
	private:
		bool m_connected;
		Thread * m_boostThread;
		boost::asio::io_service * m_service;
		bool m_toDelete;
		bool m_stopped;

	private:
		virtual void _finishConnectCallback();
		virtual bool _connectorErrorCB( const boost::system::error_code & p_err);
		virtual bool _writerErrorCB( const boost::system::error_code & p_err);
		virtual bool _readerErrorCB( const boost::system::error_code & p_err);
		virtual void _receivedMessageCallback( const std::string & p_message);
//		virtual void _resolveCallBack( const boost::system::error_code & p_err, boost::asio::ip::tcp::resolver::iterator p_resolverIterator);
		virtual void _mainLoop();
//		virtual void _connectCallBack( const boost::system::error_code & p_err, boost::asio::ip::tcp::resolver::iterator p_resolverIterator);

		void _delete();

	public:
		/*
		virtual void AsyncConnect( const std::string & p_host, unsigned short p_port)
		{
			boost::asio::ip::tcp::resolver::query l_query( p_host, General::Utils::ToString( p_port));

			m_resolver.async_resolve(	l_query,
										boost::bind(	& ElypseTcpClient::_resolveCallBack,
														this,
														boost::asio::placeholders::error,
														boost::asio::placeholders::iterator));
		}
		*/



	public:
		ElypseTcpClient( boost::asio::io_service * p_service);
		~ElypseTcpClient();



		void Run();
		void Stop();
		void StopClient();
		
		void Suicide();
		inline boost::asio::io_service * GetService() { return m_service; }

	public:
		inline bool IsConnected() { return m_connected; }
	};

	struct ElypseMessage
	{
		std::string m_clientName;
		std::string m_message;

		ElypseMessage( const std::string & p_name, const std::string & p_msg)
			:	m_clientName( p_name),
				m_message( p_msg)
		{
		}
	};
}
}

using namespace General::MultiThreading;

#endif
