/*********************************************************************************************************************

	Author :	Sylvain DOREMUS

	Company :	ForDev Studio - Copyright 2006

	Files :		DataStreamer.h - DataStreamer.cpp

	Desc :		Class used to stream the datas used in EMuse.

*********************************************************************************************************************/
#include "stdafx.h"
#include "DataStreamer.h"

#include <wx/filename.h>
#include <wx/protocol/ftp.h>
#include <wx/protocol/http.h>
#include <wx/wfstream.h>

CDataStreamer::CDataStreamer( wxListBox * p_log )
	: m_stream( NULL )
	, m_protocol( NULL )
	, m_type( 0 )
	, m_currentBlockIndex( 0 )
	, m_nbBlocks( 0 )
	, m_log( p_log )
{
}

CDataStreamer::~CDataStreamer ()
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
			m_log->AppendString( _( "Url invalide" ) );
		}
		else if ( !m_type )
		{
			m_log->AppendString( _( "Type d'URL inconnu" ) );
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
	m_log->AppendString( _( "Lecture de l'en-tête" ) );
	
	// retrieve number of bloc definitions contained in the header
	try
	{
		std::vector< char > l_magic( 9 );
		Read( reinterpret_cast< uint8_t * >( l_magic.data() ), 8 );
		m_log->AppendString( wxString( _( "  Magic : " ) ) << l_magic.data() );

		Read( m_nbBlocks );
		m_log->AppendString( wxString( _( "Nombre de blocs : " ) ) << m_nbBlocks );
		int l_index = 0;

		// retrieve all the blocks informations
		while ( l_index < m_nbBlocks )
		{
			m_log->AppendString( wxString( _( "Bloc index : " ) ) << l_index );
			// retrieve block type
			int8_t l_blockType = 0;
			Read( l_blockType );
			m_log->AppendString( wxString( _( "  Type : " ) ) << l_blockType );

			if ( l_blockType <= 0 || l_blockType > EM_BLOCK_INTERACTIONS )
			{
				throw std::runtime_error( "Invalid block type" );
			}

			// retrieve name size
			int8_t l_namesize = 0;
			Read( l_namesize );
			m_log->AppendString( wxString( _( "  Taille du nom : " ) ) << l_namesize );

			if ( l_namesize <= 0  )
			{
				throw std::runtime_error( "Invalid block name size" );
			}

			// retrieve name
			std::vector< char > l_name( l_namesize + 1, 0 );
			Read( reinterpret_cast< uint8_t * >( l_name.data() ), l_namesize );
			m_log->AppendString( wxString( _( "  Nom : " ) ) << l_name.data() );

			// retrieve block's size
			int l_blocksize = 0;
			Read( l_blocksize );
			m_log->AppendString( wxString( _( "  Taille du bloc : " ) ) << l_blocksize << _( " octets" ) );

			if ( l_blocksize <= 0  )
			{
				throw std::runtime_error( "Invalid block size" );
			}

			// retrieve block's hash
			std::vector< char > l_hash( 33, 0 );
			Read( reinterpret_cast< uint8_t * >( l_hash.data() ), 32 );
			m_log->AppendString( wxString( _( "  Hash du bloc : " ) ) << l_hash.data() );

			// write these infos into the struct deserved to it
			EM_Block block = { l_blockType, l_namesize, wxString( l_name.data() ), l_blocksize, wxString( l_hash.data() ) };

			// put this block in the list of blocks
			m_header.push_back( block );
			l_index++;
		}

		l_result = true;
	}
	catch ( std::runtime_error & p_exc )
	{
		m_log->AppendString( _( "Erreur de lecture: " ) + wxString( p_exc.what() ) );
	}

	return l_result;
}

bool CDataStreamer::GetNextBlock ()
{
	bool l_result = false;

	if ( m_currentBlockIndex < int( m_header.size() ) )
	{
		EM_Block & l_block = m_header[m_currentBlockIndex];
		wxString l_path =  m_outFolder + wxFileName::GetPathSeparator() + l_block.m_name;

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
			m_log->AppendString( _( "Erreur de lecture: " ) + wxString( p_exc.what() ) );
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

		switch( m_stream->GetLastError() )
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

int CDataStreamer::GetType( wxString const & p_url)
{
	int l_return = -1;

	if ( p_url.size() > 6 )
	{
		wxString l_buf = p_url.substr( 0, 3 );

		if ( l_buf == wxT( "fil" )
			 || l_buf == wxT( "c:\\") || l_buf == wxT( "d:\\") || l_buf == wxT( "e:\\" ) 
			 || l_buf == wxT( "f:\\") || l_buf == wxT( "g:\\") || l_buf == wxT( "h:\\" )
			 || l_buf == wxT( "i:\\") || l_buf == wxT( "j:\\") || l_buf == wxT( "k:\\" )
			 || l_buf == wxT( "l:\\") || l_buf == wxT( "m:\\") || l_buf == wxT( "n:\\" )
			 || l_buf == wxT( "o:\\") || l_buf == wxT( "p:\\") || l_buf == wxT( "q:\\" )
			 || l_buf == wxT( "r:\\") || l_buf == wxT( "s:\\") || l_buf == wxT( "t:\\" )
			 || l_buf == wxT( "u:\\") || l_buf == wxT( "v:\\") || l_buf == wxT( "w:\\" )
			 || l_buf == wxT( "x:\\") || l_buf == wxT( "y:\\") || l_buf == wxT( "z:\\" )
			 || l_buf == wxT( "C:\\") || l_buf == wxT( "D:\\") || l_buf == wxT( "E:\\" ) 
			 || l_buf == wxT( "F:\\") || l_buf == wxT( "G:\\") || l_buf == wxT( "H:\\" )
			 || l_buf == wxT( "I:\\") || l_buf == wxT( "J:\\") || l_buf == wxT( "K:\\" )
			 || l_buf == wxT( "L:\\") || l_buf == wxT( "M:\\") || l_buf == wxT( "N:\\" )
			 || l_buf == wxT( "O:\\") || l_buf == wxT( "P:\\") || l_buf == wxT( "Q:\\" )
			 || l_buf == wxT( "R:\\") || l_buf == wxT( "S:\\") || l_buf == wxT( "T:\\" )
			 || l_buf == wxT( "U:\\") || l_buf == wxT( "V:\\") || l_buf == wxT( "W:\\" )
			 || l_buf == wxT( "X:\\") || l_buf == wxT( "Y:\\") || l_buf == wxT( "Z:\\" ) )
		{
			l_return = EM_TYPE_LOCAL;
		}
		else if ( l_buf == wxT( "htt" ) )
		{
			l_return = EM_TYPE_HTTP;
		}
		else if ( l_buf == wxT( "ftp" ) )
		{
			l_return = EM_TYPE_FTP;
		}
		else
		{
			l_return = 0;
		}
	}

	return l_return;
}

void CDataStreamer::Cleanup ()
{
	CleanupStream();
	m_header.clear();
}

void CDataStreamer::CleanupStream()
{
	delete m_stream;
	m_stream = NULL;
	delete m_protocol;
	m_protocol = NULL;
}

wxString CDataStreamer::GetFileName( int p_index )
{
	return m_header[p_index].m_name;
}

//********************************************************************************************************************