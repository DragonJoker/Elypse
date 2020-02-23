/*
See LICENSE file in root folder
*/
#include "stdafx.h"
#include "DataStreamer.h"

#include <wx/filename.h>
#include <wx/protocol/ftp.h>
#include <wx/protocol/http.h>
#include <wx/wfstream.h>

#include <regex>

using namespace Elypse::Download;

CDataStreamer::CDataStreamer( wxListBox * p_log )
	: m_stream( nullptr )
	, m_protocol( nullptr )
	, m_type( 0 )
	, m_currentBlockIndex( 0 )
	, m_nbBlocks( 0 )
	, m_log( p_log )
{
}

CDataStreamer::~CDataStreamer()
{
	Cleanup();
}

bool CDataStreamer::SetFile( wxString const & p_url )
{
	bool l_result = false;

	if ( !p_url .empty() )
	{
		m_type = GetType( p_url );

		if ( m_type == -1 )
		{
			m_log->AppendString( _( "Invalid URL" ) );
		}
		else if ( m_type == EM_TYPE_UNKNOWN )
		{
			m_log->AppendString( _( "Unknown URL format" ) );
		}
		else
		{
			CleanupStream();

			// connect to the file
			switch ( m_type )
			{
			case EM_TYPE_LOCAL:
				m_stream = new wxFileInputStream( p_url );
				l_result = true;
				break;

			case EM_TYPE_HTTP:
				m_protocol = new wxHTTP;
				m_stream = m_protocol->GetInputStream( p_url );
				l_result = true;
				break;

			case EM_TYPE_FTP:
				m_protocol = new wxFTP;
				m_stream = m_protocol->GetInputStream( p_url );
				l_result = true;
				break;

			default:
				break;
			}
		}
	}

	return l_result;
}

bool CDataStreamer::ReadHeader()
{
	bool l_result = false;
	m_log->AppendString( _( "Reading header" ) );

	// retrieve number of bloc definitions contained in the header
	try
	{
		std::vector< char > l_magic( 9 );
		Read( reinterpret_cast< uint8_t * >( l_magic.data() ), 8 );
		m_log->AppendString( _( "  Magic : " ) + make_wxString( l_magic.data() ) );

		int l_version = 1;

		if ( std::string( l_magic.data() ) != "MUSEFILE" )
		{
			l_version = l_magic[7] - '0';
		}

		Read( m_nbBlocks );
		m_log->AppendString( wxString( _( "Blocks count: " ) ) << m_nbBlocks );
		int l_index = 0;

		// retrieve all the blocks informations
		while ( l_index < m_nbBlocks )
		{
			m_log->AppendString( wxString( _( "Block index : " ) ) << l_index );
			// retrieve block type
			int8_t l_blockType = 0;
			Read( l_blockType );
			m_log->AppendString( wxString( _( "  Type : " ) ) << l_blockType );

			if ( l_blockType <= 0 || l_blockType >= EM_BLOCK_ERROR )
			{
				throw std::runtime_error( "Invalid block type" );
			}

			// retrieve name size
			int8_t l_namesize = 0;
			Read( l_namesize );
			m_log->AppendString( wxString( _( "  Name size: " ) ) << l_namesize );

			if ( l_namesize <= 0 )
			{
				throw std::runtime_error( "Invalid block name size" );
			}

			// retrieve name
			std::vector< char > l_name( l_namesize + 1, 0 );
			Read( reinterpret_cast< uint8_t * >( l_name.data() ), l_namesize );
			m_log->AppendString( wxString( _( "  Name : " ) ) << make_wxString( l_name.data() ) );

			// retrieve block's size
			int l_blocksize = 0;
			Read( l_blocksize );
			m_log->AppendString( wxString( _( "  Block size: " ) ) << l_blocksize << _( " octets" ) );

			if ( l_blocksize <= 0 )
			{
				throw std::runtime_error( "Invalid block size" );
			}

			std::vector< char > l_hash( 33, 0 );

			if ( l_version >= 1 )
			{
				// retrieve block's hash
				Read( reinterpret_cast< uint8_t * >( l_hash.data() ), 32 );
				m_log->AppendString( wxString( _( "  Block hash: " ) ) << make_wxString( l_hash.data() ) );
			}

			// write these infos into the struct deserved to it
			EM_Block block = { uint8_t( l_blockType ), uint8_t( l_namesize ), std::string( l_name.data() ), uint32_t( l_blocksize ), std::string( l_hash.data() ) };

			// put this block in the list of blocks
			m_header.push_back( block );
			l_index++;
		}

		l_result = true;
	}
	catch ( std::runtime_error & p_exc )
	{
		m_log->AppendString( _( "Read error: " ) + make_wxString( p_exc.what() ) );
	}

	return l_result;
}

bool CDataStreamer::GetNextBlock()
{
	bool l_result = false;

	if ( m_currentBlockIndex < int( m_header.size() ) )
	{
		EM_Block & l_block = m_header[m_currentBlockIndex];
		wxString l_path = m_outFolder + wxFileName::GetPathSeparator() + make_wxString( l_block.m_name );

		try
		{
			std::vector< uint8_t > l_buffer( l_block.m_blocsize );
			Read( l_buffer.data(), l_buffer.size() );
			wxFile l_file( l_path, wxFile::write );
			l_file.Write( reinterpret_cast< void * >( l_buffer.data() ), l_buffer.size() );
			l_file.Close();
			l_result = true;
		}
		catch ( std::runtime_error & p_exc )
		{
			m_log->AppendString( _( "Read error: " ) + make_wxString( p_exc.what() ) );
		}

		m_currentBlockIndex++;
	}

	return l_result;
}

void CDataStreamer::Read( uint8_t * p_value, size_t p_size )
{
	m_stream->Read( reinterpret_cast< void * >( p_value ), p_size );

	if ( m_stream->LastRead() < p_size )
	{
		std::string l_error;

		switch ( m_stream->GetLastError() )
		{
		case wxStreamError::wxSTREAM_EOF:
			l_error = "End of stream";
			break;

		case wxStreamError::wxSTREAM_READ_ERROR:
			l_error = "Read error";
			break;

		default:
			l_error = "Unknown error";
			break;
		}

		throw std::runtime_error( l_error );
	}
}

int CDataStreamer::GetType( wxString const & p_url )
{
	int l_return = -1;

	if ( p_url.size() > 6 )
	{
		std::string l_buf = make_string( p_url.substr( 0, 3 ) );
		const std::regex l_regexDisk( "[a-zA-Z]:\\\\" );

		if ( l_buf == "fil" || l_buf[0] == '/' || std::regex_search( l_buf, l_regexDisk ) )
		{
			l_return = EM_TYPE_LOCAL;
		}
		else if ( l_buf == "htt" )
		{
			l_return = EM_TYPE_HTTP;
		}
		else if ( l_buf == "ftp" )
		{
			l_return = EM_TYPE_FTP;
		}
		else
		{
			l_return = EM_TYPE_UNKNOWN;
		}
	}

	return l_return;
}

void CDataStreamer::Cleanup()
{
	CleanupStream();
	m_header.clear();
}

void CDataStreamer::CleanupStream()
{
	delete m_stream;
	m_stream = nullptr;
	delete m_protocol;
	m_protocol = nullptr;
}

wxString CDataStreamer::GetFileName( int p_index )
{
	return make_wxString( m_header[p_index].m_name );
}

//********************************************************************************************************************
