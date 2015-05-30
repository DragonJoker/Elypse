#ifndef ___GENERAL_TCP_READER_H___
#define ___GENERAL_TCP_READER_H___

#include <string>

#include "BoostHeaders.h"

#include "Macros.h"
#include "LocklessQueue.h"
#include "Exception.h"

namespace General
{
	namespace MultiThreading
	{
		class d_abstract TcpReaderBase
		{
		protected:
			boost::asio::ip::tcp::socket & m_socket;
			boost::asio::io_service & m_service;

			enum
			{
				c_headerLength = sizeof( short ),
				c_maxMessageLength = 8190
			};

			boost::array	<char, c_headerLength>		m_headerBuffer;
			boost::array	<char, c_maxMessageLength>	m_bodyBuffer;
			unsigned short m_messageLength;

		public:
			TcpReaderBase( boost::asio::ip::tcp::socket & p_socket, boost::asio::io_service & p_service )
				:	m_socket( p_socket ),
					m_service( p_service )
			{
			}
			virtual ~TcpReaderBase()
			{
			}

		protected:
			virtual void _receivedMessageCallback( const std::string & p_message ) = 0;
			virtual bool _readerErrorCB( const boost::system::error_code & p_err ) = 0;

			void _startReadHeader()
			{
				boost::asio::async_read(	m_socket, boost::asio::buffer( m_headerBuffer, c_headerLength ),
											boost::bind(	& TcpReaderBase::_endHeaderCallback,
													this,
													boost::asio::placeholders::error ) );
			}

			void _startReadBody( unsigned short p_messageLength )
			{
				m_messageLength = p_messageLength;
				boost::asio::async_read(	m_socket, boost::asio::buffer( m_bodyBuffer, p_messageLength ),
											boost::bind(	& TcpReaderBase::_endBodyCallback,
													this,
													boost::asio::placeholders::error ) );
			}

			void _endHeaderCallback( const boost::system::error_code & p_err )
			{
				if ( p_err && _readerErrorCB( p_err ) )
				{
					return;
				}

				unsigned short l_size;
				l_size = static_cast <unsigned char>( m_headerBuffer[0] ) + ( static_cast <unsigned char>( m_headerBuffer[1] ) << 8 );
//			std::cout << "_endHeaderCallback " << l_size << "\n";
				_startReadBody( l_size );
			}

			void _endBodyCallback( const boost::system::error_code & p_err )
			{
				if ( p_err && _readerErrorCB( p_err ) )
				{
					return;
				}

				if ( m_messageLength > 0 )
				{
					std::string l_string;
					l_string.append( m_bodyBuffer.data(), m_messageLength );

					_receivedMessageCallback( l_string );
				}

				_startReadHeader();
			}

		public:
			boost::system::error_code BlockingRead( std::string & p_contents )
			{
				boost::system::error_code l_error;
				boost::array <char, 8192> l_buffer;

				size_t l_length = m_socket.read_some( boost::asio::buffer( l_buffer ), l_error );

				p_contents.clear();

				if ( l_length > 0 )
				{
					p_contents.append( l_buffer.data(), l_length );
				}

				return l_error;
			}

			void StartAsyncRead()
			{
				_startReadHeader();
			}
		};

//	Exemple de TcpReader qui merde fais par le Bubar à revoir avant usage
		class TcpQueuedReader : public TcpReaderBase
		{
		protected:
			General::Templates::lockless_queue <std::string> m_messages;

		public:
			TcpQueuedReader( boost::asio::ip::tcp::socket & p_socket, boost::asio::io_service & p_service )
				:	TcpReaderBase( p_socket, p_service )
			{
			}
			virtual ~TcpQueuedReader()
			{
			}

		protected:
			virtual void _receivedMessageCallback( const std::string & p_message )
			{
//			std::cout << "message recu : " << p_message << std::endl;
				m_messages.push_back( p_message );
			}

			virtual bool _errorCallback( const boost::system::error_code & p_err )
			{
				GENLIB_EXCEPTION( "TcpQueuedReader -> error : " + p_err.message() );
				return false;
			}

		public:
			std::string Get()
			{
				return m_messages.pop_front();
			}
		};
	}
}

#endif
