#ifndef ___GENERAL_TCP_WRITER_H___
#define ___GENERAL_TCP_WRITER_H___

#include <string>
#include <deque>

#include "BoostHeaders.h"

#include "Macros.h"
#include "Exception.h"

namespace Elypse
{
	namespace Network
	{
		class TcpWriterBase
		{
		protected:
			boost::asio::ip::tcp::socket & m_socket;
			boost::asio::io_service & m_service;
			std::deque <std::string> m_messages;
			bool m_noSend;
			enum
			{
				c_headerLength = sizeof( short ),
				c_maxMessageLength = 8190
			};
			boost::array < char, c_maxMessageLength + c_headerLength > m_buffer;

		public:
			TcpWriterBase( boost::asio::ip::tcp::socket & p_socket, boost::asio::io_service & p_service )
				:	m_socket( p_socket ),
					m_service( p_service ),
					m_noSend( false )
			{
			}
			virtual ~TcpWriterBase()
			{
			}
		protected:
			virtual bool CallbackWriterError( const boost::system::error_code & p_err ) = 0;

			void _endWrite( const boost::system::error_code & p_err )
			{
				if ( p_err )
				{
					return;
				}

				/*
							if (p_err && _writerErrorCB( p_err))
							{
								return;
							}
				*/
				m_messages.pop_front();

				if ( ! m_messages.empty() )
				{
					_asyncWrite();
				}
			}

			void _asyncWrite()
			{
				unsigned short l_length = static_cast <unsigned short>( m_messages.front().size() );
				m_buffer.data()[0] = static_cast< unsigned char >( l_length & 0xFF );
				m_buffer.data()[1] = static_cast< unsigned char >( ( l_length >> 8 ) & 0xFF );
//			memcpy( m_buffer.data(), &l_length, c_headerLength);
				memcpy( m_buffer.data() + c_headerLength, m_messages.front().c_str(), m_messages.front().size() );

				boost::asio::async_write(	m_socket,
											boost::asio::buffer( m_buffer.data(), l_length + c_headerLength ),
											boost::bind(	& TcpWriterBase::_endWrite,
													this,
													boost::asio::placeholders::error ) );
			}

			void _addMessage( const std::string & p_message )
			{
				bool l_noWrite = m_messages.empty();

				m_messages.push_back( p_message );

				if ( l_noWrite )
				{
					_asyncWrite();
				}
			}

			void _addMessageNoSend( const std::string & p_message )
			{
				m_messages.push_back( p_message );
			}

		public:
			boost::system::error_code BlockingSend( const std::string & p_message )
			{
				boost::system::error_code l_error;
				boost::asio::write( m_socket, boost::asio::buffer( p_message ), boost::asio::transfer_all(), l_error );
				return l_error;
			}

			void AsyncSend( const std::string & p_message )
			{
				if ( m_noSend )
				{
					return;
				}

				m_service.post( boost::bind( & TcpWriterBase::_addMessage, this, p_message ) );
			}

			void StoreMessage( const std::string & p_message )
			{
				if ( m_noSend )
				{
					return;
				}

				m_service.post( boost::bind( & TcpWriterBase::_addMessageNoSend, this, p_message ) );
			}

			void StartSending()
			{
				if ( ! m_messages.empty() )
				{
					_asyncWrite();
				}
			}
		};


//	Exemple de TcpWriter très simplifié
		class TcpWriter : public TcpWriterBase
		{
		public:
			TcpWriter( boost::asio::ip::tcp::socket & p_socket, boost::asio::io_service & p_service )
				:	TcpWriterBase( p_socket, p_service )
			{
			}
			virtual ~TcpWriter()
			{
			}

		protected:
			virtual bool _errorCallback( const boost::system::error_code & p_err )
			{
//			std::cout << "TcpWriter -> error : " + p_err.message() << std::endl;
				GENLIB_EXCEPTION( "TcpWriter -> error : " + p_err.message() );
				return false;
			}
		};
	}
}

#endif
