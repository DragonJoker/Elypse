#ifndef ___TCP_ACCEPTOR_H___
#define ___TCP_ACCEPTOR_H___

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <set>

#include <General/STLSetMacros.h>

namespace General
{ namespace MultiThreading
{
	class TcpBaseClient
	{
	protected:
		boost::asio::ip::tcp::socket m_socket;

	public:
		TcpBaseClient( boost::asio::io_service & p_service)
			:	m_socket( p_service)
		{
		}
		virtual ~TcpBaseClient(){}

	public:
		virtual void Start() = 0;
	public:
		inline boost::asio::ip::tcp::socket & GetSocket() { return m_socket; }
	};

	class TcpAcceptor
	{
	protected:
		boost::asio::io_service m_service;
		boost::asio::ip::tcp::acceptor m_acceptor;

		std::set<TcpBaseClient *> m_clients;

		TcpBaseClient * m_currentClient;

	public:
		TcpAcceptor( unsigned short p_port)
			:	m_service(),
				m_acceptor( m_service, boost::asio::ip::tcp::endpoint( boost::asio::ip::tcp::v4(), p_port))
		{
			_accept();
		}

		virtual ~TcpAcceptor()
		{
			m_acceptor.close();
			delete m_currentClient;
			General::Utils::set::deleteAll( m_clients);
		}

	private:
		virtual TcpBaseClient * _createNewClient() = 0;
		virtual void _accept()
		{
			m_currentClient = _createNewClient();
			m_acceptor.async_accept( m_currentClient->GetSocket(), boost::bind( & TcpAcceptor::_acceptCallback, this, boost::asio::placeholders::error));
		}

		virtual void _acceptCallback( const boost::system::error_code & error)
		{
			if (error)
			{
				return;
			}
			m_currentClient->Start();
			m_clients.insert( m_currentClient);
			_accept();
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
