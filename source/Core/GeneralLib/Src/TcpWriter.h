/*
See LICENSE file in root folder
*/
#ifndef ___GENERAL_TCP_WRITER_H___
#define ___GENERAL_TCP_WRITER_H___

#include <string>
#include <deque>

#include "Macros.h"
#include "Exception.h"

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>

namespace General
{
	namespace Network
	{
		class TcpWriterBase
		{
		protected:
			bool m_noSend;
			enum
			{
				c_headerLength = sizeof( short ),
				c_maxMessageLength = 8190
			};

		public:
			TcpWriterBase( boost::asio::ip::tcp::socket & p_socket, boost::asio::io_service & p_service )
				: m_socket( p_socket )
				, m_service( p_service )
				, m_noSend( false )
			{
			}
			virtual ~TcpWriterBase()
			{
			}
		protected:
			virtual bool CallbackWriterError( boost::system::error_code const & p_err ) = 0;

			void DoEndWrite( boost::system::error_code const & p_err )
			{
				if ( p_err )
				{
					return;
				}

				//if (p_err && CallbackWriterError( p_err))
				//{
				//	return;
				//}

				m_messages.pop_front();

				if ( ! m_messages.empty() )
				{
					DoAsyncWrite();
				}
			}

			void DoAsyncWrite()
			{
				uint16_t l_length = static_cast <uint16_t>( m_messages.front().size() );
				m_buffer.data()[0] = static_cast< unsigned char >( l_length & 0xFF );
				m_buffer.data()[1] = static_cast< unsigned char >( l_length >> 8 );
				//memcpy( m_buffer.data(), &l_length, c_headerLength);
				memcpy( m_buffer.data() + c_headerLength, m_messages.front().c_str(), m_messages.front().size() );
				boost::asio::async_write( m_socket,
										  boost::asio::buffer( m_buffer.data(), l_length + c_headerLength ),
										  boost::bind( &TcpWriterBase::DoEndWrite,
													   this,
													   boost::asio::placeholders::error ) );
			}

			void DoAddMessage( std::string const & p_message )
			{
				bool l_noWrite = m_messages.empty();
				m_messages.push_back( p_message );

				if ( l_noWrite )
				{
					DoAsyncWrite();
				}
			}

			void DoAddMessageNoSend( std::string const & p_message )
			{
				m_messages.push_back( p_message );
			}

		public:
			boost::system::error_code BlockingSend( std::string const & p_message )
			{
				boost::system::error_code l_error;
				boost::asio::write( m_socket, boost::asio::buffer( p_message ), boost::asio::transfer_all(), l_error );
				return l_error;
			}

			void AsyncSend( std::string const & p_message )
			{
				if ( !m_noSend )
				{
					m_service.post( [this, p_message]()
					{
						DoAddMessage( p_message );
					} );
				}
			}

			void StoreMessage( std::string const & p_message )
			{
				if ( !m_noSend )
				{
					m_service.post( [this, p_message]()
					{
						DoAddMessageNoSend( p_message );
					} );
				}
			}

			void StartSending()
			{
				if ( !m_messages.empty() )
				{
					DoAsyncWrite();
				}
			}

		protected:
			boost::asio::ip::tcp::socket & m_socket;
			boost::asio::io_service & m_service;
			std::deque <std::string> m_messages;
			std::array < char, c_maxMessageLength + c_headerLength > m_buffer;
		};

		// Exemple de TcpWriter très simplifié
		class TcpWriter : public TcpWriterBase
		{
		public:
			TcpWriter( boost::asio::ip::tcp::socket & p_socket, boost::asio::io_service & p_service )
				: TcpWriterBase{ p_socket, p_service }
			{
			}
			virtual ~TcpWriter()
			{
			}

		protected:
			virtual bool CallbackWriterError( boost::system::error_code const & p_err )
			{
				GENLIB_EXCEPTION( "TcpWriter -> error : " + p_err.message() );
				return false;
			}
		};
	}
}

#endif
