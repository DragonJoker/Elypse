/*
See LICENSE file in root folder
*/
#ifndef ___GENERAL_TCP_READER_H___
#define ___GENERAL_TCP_READER_H___

#include <string>

#include "Macros.h"
#include "CircularQueue.h"
#include "Exception.h"

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>

#include <iostream>

namespace General
{
	namespace Network
	{
		class TcpReaderBase
		{
		protected:
			enum
			{
				c_headerLength = sizeof( int16_t ),
				c_maxMessageLength = 8190
			};

		public:
			TcpReaderBase( boost::asio::ip::tcp::socket & p_socket, boost::asio::io_service & p_service )
				: m_socket{ p_socket }
				, m_service{ p_service }
			{
			}
			virtual ~TcpReaderBase()
			{
			}

		protected:
			virtual void CallbackReceivedMessage( std::string const & p_message ) = 0;
			virtual bool CallbackReaderError( boost::system::error_code const & p_err ) = 0;

			void DoStartReadHeader()
			{
				boost::asio::async_read( m_socket, boost::asio::buffer( m_headerBuffer, c_headerLength ),
										 boost::bind( &TcpReaderBase::CallbackEndHeader,
													  this,
													  boost::asio::placeholders::error ) );
			}

			void DoStartReadBody( uint16_t p_messageLength )
			{
				m_messageLength = p_messageLength;
				assert( m_messageLength <= m_bodyBuffer.size() );
				boost::asio::async_read( m_socket, boost::asio::buffer( m_bodyBuffer, p_messageLength ),
										 boost::bind( &TcpReaderBase::CallbackEndBody,
													  this,
													  boost::asio::placeholders::error ) );
			}

			void CallbackEndHeader( boost::system::error_code const & p_err )
			{
				if ( p_err && CallbackReaderError( p_err ) )
				{
					return;
				}

				uint16_t l_size( uint16_t( m_headerBuffer[0] ) + ( uint16_t( m_headerBuffer[1] ) << 8 ) );
				DoStartReadBody( l_size );
			}

			void CallbackEndBody( boost::system::error_code const & p_err )
			{
				if ( p_err && CallbackReaderError( p_err ) )
				{
					return;
				}

				if ( m_messageLength > 0 )
				{
					std::string l_string{ m_bodyBuffer.data(), m_messageLength };
					CallbackReceivedMessage( l_string );
				}

				DoStartReadHeader();
			}

		public:
			boost::system::error_code BlockingRead( std::string & p_contents )
			{
				boost::system::error_code l_error;
				std::array< char, 8192 > l_buffer;
				size_t l_length{ m_socket.read_some( boost::asio::buffer( l_buffer ), l_error ) };
				p_contents.clear();

				if ( l_length > 0 )
				{
					p_contents.append( l_buffer.data(), l_length );
				}

				return l_error;
			}

			void StartAsyncRead()
			{
				DoStartReadHeader();
			}

		protected:
			boost::asio::ip::tcp::socket & m_socket;
			boost::asio::io_service & m_service;
			std::array< char, c_headerLength > m_headerBuffer;
			std::array< char, c_maxMessageLength > m_bodyBuffer;
			uint16_t m_messageLength;
		};

		// Exemple de TcpReader qui merde fais par le Bubar à revoir avant usage
		class TcpQueuedReader
			: public TcpReaderBase
		{
		public:
			TcpQueuedReader( boost::asio::ip::tcp::socket & p_socket, boost::asio::io_service & p_service )
				: TcpReaderBase{ p_socket, p_service }
			{
			}
			virtual ~TcpQueuedReader()
			{
			}

		protected:
			virtual void CallbackReceivedMessage( std::string const & p_message )
			{
				m_messages.push_back( p_message );
			}

			virtual bool CallbackReaderError( boost::system::error_code const & p_err )
			{
				std::cerr << "CallbackReaderError: " << p_err.message() << std::endl;

				if ( boost::asio::error::eof != p_err && boost::asio::error::connection_reset != p_err )
				{
					GENLIB_EXCEPTION( "TcpQueuedReader -> error : " + p_err.message() );
				}

				return true;
			}

		public:
			std::string Get()
			{
				return m_messages.pop_front();
			}

		protected:
			General::Templates::CircularQueue< std::string > m_messages;
		};
	}
}

#endif
