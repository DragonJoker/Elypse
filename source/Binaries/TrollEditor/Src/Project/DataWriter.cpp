/*********************************************************************************************************************

	Author :	Sylvain DOREMUS

	Company :	ForDev Studio - Copyright 2006

	Files :		DataWriter.h - DataWriter.cpp

	Desc :		Class used to write the datas used in EMuse.

*********************************************************************************************************************/
#include "PrecompiledHeader.h"

#include "Project/DataWriter.h"
#include "GUI/MainFrame.h"

#include <wx/dir.h>

using namespace Troll::GUI;
using namespace EMuse::Download;



DataWriter::DataWriter( wxTextCtrl * p_log )
	:	m_log( p_log ),
		m_file( NULL ),
		m_md5( new MD5() )
{
}



DataWriter::~DataWriter()
{
	if ( m_file != NULL )
	{
		fclose( m_file );
	}

	General::Utils::vector::deleteAll( m_header );
	m_fileslist.clear();
	delete m_md5;
}



bool DataWriter::AddFolder( const wxString & p_folder, DataBlockType p_eType )
{
	m_fileslist.clear();
	wxString l_folder = GetParentFolder( p_folder );

	if ( l_folder.empty() )
	{
		return false;
	}

	wxString l_foldername = GetName( p_folder );

	if ( l_foldername.empty() )
	{
		return false;
	}

	ListFiles( p_folder );
	wxString l_path = l_folder + wxFileName::GetPathSeparator() + l_foldername + DataTypeExtension[p_eType];

	if ( CompressFolder( l_folder, l_path, p_eType ) )
	{
		std::cout << "[" << l_path << "]\n";
		return AddFile( l_path, p_eType );
	}
	else
	{
		return false;
	}
}



void DataWriter::ListFiles( const wxString & p_folder )
{
	wxDir l_dir( p_folder );
	wxString l_strFileName;
	wxString l_path;

	if ( l_dir.IsOpened() )
	{
		bool l_bRes = l_dir.GetFirst( & l_strFileName, wxString( wxT( "*.*" ) ), wxDIR_DEFAULT );

		while ( l_bRes )
		{
			if ( l_strFileName != wxT( "." ) && l_strFileName != wxT( ".." ) )
			{
				l_path = p_folder + wxFileName::GetPathSeparator() + l_strFileName;
				wxFileName l_fileName( l_path );

				if ( l_fileName.IsDir() )
				{
					ListFiles( l_path );
				}
				else
				{
					m_fileslist.push_back( l_path );
				}
			}

			l_bRes = l_dir.GetNext( & l_strFileName );
		}
	}
}



bool DecompressFile( const wxString & p_zipFileName, const wxString & p_fileName, int p_size )
{
	bool l_return = true;
	gzFile l_infile = gzopen( p_zipFileName.char_str().data(), "r" );

	if ( !l_infile )
	{
		l_return = false;
	}

	FILE * l_outfile = NULL;

	if ( l_return )
	{
		l_outfile = fopen( p_fileName.char_str().data(), "wb" );

		if ( !l_outfile )
		{
			gzclose( l_infile );
			l_return = false;
		}
	}

	if ( l_return )
	{
		std::vector< uint8_t > l_buffer( p_size );
		int l_nbBytesRead = 0;
		int l_totalRead = 0;

		while ( ( l_nbBytesRead = gzread( l_infile, l_buffer.data(), uint32_t( p_size ) ) ) > 0 )
		{
			l_totalRead += l_nbBytesRead;
			fwrite( l_buffer.data(), 1, uint32_t( l_nbBytesRead ), l_outfile );
		}

		if ( l_nbBytesRead < 0 )
		{
			l_return = false;

			switch ( l_nbBytesRead )
			{
			case Z_ERRNO:
				std::cout << "Can't inflate file [" + p_fileName + "] due to error " + std::to_string( errno ) << std::endl;
				break;

			case Z_STREAM_END:
				std::cout << "Can't inflate file [" + p_fileName + "] due to Z_STREAM_END error" << std::endl;
				break;

			case Z_DATA_ERROR:
				std::cout << "Can't inflate file [" + p_fileName + "] due to Z_DATA_ERROR error" << std::endl;
				break;

			case Z_STREAM_ERROR:
				std::cout << "Can't inflate file [" + p_fileName + "] due to Z_STREAM_ERROR error" << std::endl;
				break;

			case Z_NEED_DICT:
				std::cout << "Can't inflate file [" + p_fileName + "] due to Z_NEED_DICT error" << std::endl;
				break;

			case Z_MEM_ERROR:
				std::cout << "Can't inflate file [" + p_fileName + "] due to Z_MEM_ERROR error" << std::endl;
				break;
			}
		}

		fclose( l_outfile );
		gzclose( l_infile );
	}

	return l_return;
}



int DataWriter::CompressFile( const wxString & p_infilename, const wxString & p_outfilename )
{
	FILE * l_infile = fopen( p_infilename.char_str().data(), "rb" );
	gzFile l_outfile = gzopen( p_outfilename.char_str().data(), "wb" );

	if ( l_infile == NULL )
	{
		if ( l_outfile != NULL )
		{
			gzclose( l_outfile );
		}

		return -1;
	}

	if ( l_outfile == NULL )
	{
		if ( l_infile != NULL )
		{
			fclose( l_infile );
		}

		return -1;
	}

	char l_buffer[128];
	int l_nbBytesRead = 0, l_totalRead = 0, l_totalWrote = 0, l_totalWritten = 0;

	while ( ( l_nbBytesRead = static_cast<int>( fread( l_buffer, sizeof( char ), 128, l_infile ) ) ) > 0 )
	{
		l_totalRead += l_nbBytesRead;
		l_totalWritten += gzwrite( l_outfile, l_buffer, l_nbBytesRead );
	}

	fclose( l_infile );
	gzclose( l_outfile );
	l_infile = fopen( p_outfilename.char_str().data(), "rb" );
	fseek( l_infile, 0, SEEK_END );
	l_totalWritten = ftell( l_infile );
	fclose( l_infile );
//	DecompressFile( p_outfilename, p_infilename + wxT( "unc" ), l_totalWritten );
	return l_totalWritten;
}



bool DataWriter::CompressFolder( const wxString & p_outFolder, const wxString & p_outPath, DataBlockType p_eType )
{
	ZipBlockList l_zipHeader;
	wxString l_fileName, l_extension, l_outFile, l_outpath;
	int l_size;
	EM_ZipBlock * l_block;

	// We compress each file
	for ( unsigned int i = 0 ; i < m_fileslist.size() ; i++ )
	{
		l_fileName = GetNameWithoutExtension( m_fileslist[i] );
		l_extension = GetExtension( m_fileslist[i] );
		l_outFile = p_outFolder + wxFileName::GetPathSeparator() + GetName( m_fileslist[i] ) + wxT( ".zip" );
		l_outpath = l_fileName + wxT( "." ) + l_extension;
		l_size = CompressFile( m_fileslist[i], l_outFile );
		l_block = new EM_ZipBlock();
		l_block->BlockSize = l_size;
		l_block->Name = l_outpath;
		l_block->NameSize = static_cast <int>( l_outpath.size() );
		l_block->CompleteURL = l_outFile;
		l_zipHeader.push_back( l_block );
	}

	FILE * l_file = fopen( p_outPath.char_str().data(), "wb" );
	WriteZipHeader( l_zipHeader, l_file );
	WriteZipFile( l_zipHeader, l_file );
	General::Utils::vector::deleteAll( l_zipHeader );
	fclose( l_file );
	return true;
}



bool DataWriter::AddFile( const wxString & p_fileurl, DataBlockType p_eType )
{
	g_mainFrame->LogDebugMessage( p_fileurl );
	wxString l_fileurl = p_fileurl;
	FILE * l_file = fopen( l_fileurl.char_str().data(), "rb" );

	if ( l_file == NULL )
	{
		m_log->WriteText( wxT( "Can't open " ) + l_fileurl + wxT( "\n" ) );
		return false;
	}

	struct stat tagStat;

	int ret = stat( l_fileurl.char_str().data(), & tagStat );

	unsigned long long l_size = tagStat.st_size;

	//unsigned __int64 l_size = l_file.GetLength();
	String l_md5 = m_md5->HashFile( l_file );

	fclose( l_file );

	wxString l_name = GetName( p_fileurl );

	if ( l_name.empty() )
	{
		return false;
	}

	int l_namesize = static_cast <int>( l_name.size() );
	int l_type = p_eType;

	if ( l_type == EM_NB_BLOCK_TYPES )
	{
		l_type = GetType( l_name );
	}

	if ( l_type <= 0 || l_type > EM_NB_BLOCK_TYPES )
	{
		return false;
	}

	EM_Block * block = new EM_Block();
	block->m_blocktype = l_type;
	block->m_blocsize = static_cast <int>( l_size );
	block->m_name = l_name;
	block->m_namesize = l_namesize;
	block->m_completeUrl = p_fileurl;
	char * l_hash = new char[33];

	for ( int i = 0 ; i < 32 ; i++ )
	{
		l_hash[i] = l_md5[i];
	}

	l_hash[32] = '\0';
	block->m_hash = l_hash;
	m_header.push_back( block );
	return true;
}



bool DataWriter::DelFile( const wxString & p_file )
{
	BlockList::iterator l_it = m_header.begin();

	for ( unsigned int i = 0 ; i < m_header.size() ; i++ )
	{
		if ( m_header[i]->m_completeUrl == p_file )
		{
			m_header.erase( l_it );
			m_log->WriteText( wxT( "Entry for " ) + p_file + wxT( " deleted" ) );
			return true;
		}

		l_it++;
	}

	m_log->WriteText( wxT( "Entry for " ) + p_file + wxT( " not deleted" ) );
	return false;
}



bool DataWriter::DownFile( const wxString & p_file )
{
	BlockList::iterator l_it = m_header.begin();

	for ( unsigned int i = 0 ; i < m_header.size() ; i++ )
	{
		if ( m_header[i]->m_completeUrl ==  p_file && i < m_header.size() - 1 )
		{
			EM_Block * l_tmp = m_header[i + 1];
			m_header[i + 1] = m_header[i];
			m_header[i] = l_tmp;
			return true;
		}

		l_it++;
	}

	return false;
}



bool DataWriter::UpFile( const wxString & p_file )
{
	BlockList::iterator l_it = m_header.begin();

	for ( unsigned int i = 0 ; i < m_header.size() ; i++ )
	{
		if ( m_header[i]->m_completeUrl == p_file && i > 0 )
		{
			EM_Block * l_tmp = m_header[i - 1];
			m_header[i - 1] = m_header[i];
			m_header[i] = l_tmp;
			return true;
		}

		l_it++;
	}

	return false;
}



bool DataWriter::Write( const wxString & p_filename )
{
	if ( m_header.size() == 0 )
	{
		return false;
	}

	int l_length = ( int )p_filename.size();
	wxString l_filename = p_filename;
	m_file = fopen( l_filename.char_str().data(), "wb" );
	char * l_buffer = new char[8];
	l_buffer[0] = 'M';
	l_buffer[1] = 'U';
	l_buffer[2] = 'S';
	l_buffer[3] = 'E';
	l_buffer[4] = 'F';
	l_buffer[5] = 'I';
	l_buffer[6] = 'L';
	l_buffer[7] = 'E';
	fwrite( l_buffer, 1, 8, m_file );
	l_buffer[7] = 0;
	delete [] l_buffer;
	unsigned long long l_nbBytes = 8;
	WriteHeader( l_nbBytes );
	wxString l_log;
	l_log << wxT( "Header written from byte 8 to byte : " ) << l_nbBytes << wxT( "\n" );
	m_log->WriteText( l_log );
	l_log.clear();

	for ( unsigned int i = 0 ; i < m_header.size() ; i++ )
	{
//		std::cout << m_header[i]->m_completeUrl << "\n";
		FILE * l_file = fopen( m_header[i]->m_completeUrl.char_str().data(), "rb" );
		m_log->WriteText( wxT( "Integrating file : " ) + m_header[i]->m_name + wxT( "\n" ) );
		l_log << wxT( "From byte : " ) << l_nbBytes;
		int l_size = m_header[i]->m_blocsize;
		l_nbBytes += l_size;
		l_log << wxT( "To byte : " ) << l_nbBytes << wxT( "\n" );
		m_log->WriteText( l_log );
		l_log.clear();
		std::vector< uint8_t > l_readBuffer( l_size );
		fread( l_readBuffer.data(), 1, l_size, l_file );
		fwrite( l_readBuffer.data(), 1, l_size, m_file );
		fclose( l_file );
	}

//	sprintf_s(l_log, 256, "File Length : %d\0", m_file->GetLength());
//	m_log->WriteText(l_log);
	fclose( m_file );
	m_file = NULL;
	return true;
}



void DataWriter::WriteZipFile( ZipBlockList p_list, FILE * p_file )
{
	for ( ZipBlockList::const_iterator l_it = p_list.begin(); l_it != p_list.end(); ++l_it )
	{
		EM_ZipBlock * p_block = *l_it;
		FILE * l_file = fopen( p_block->CompleteURL.char_str().data(), "rb" );

		if ( l_file != NULL )
		{
			uint32_t l_size = uint32_t( p_block->BlockSize );
			fseek( l_file, 0, SEEK_END );
			uint32_t l_detected = ftell( l_file );
			fseek( l_file, 0, SEEK_SET );
			std::vector< uint8_t > l_readBuffer( l_size );
			uint32_t l_uiRead = 0;
			uint32_t l_uiTotal = 0;

			while ( l_uiTotal < l_size && ( l_uiRead = fread( &l_readBuffer[l_uiTotal], 1, l_size - l_uiTotal, l_file ) ) > 0 )
			{
				fwrite( &l_readBuffer[l_uiTotal], 1, l_uiRead, p_file );
				l_uiTotal += l_uiRead;
			}

			fclose( l_file );
		}
		else
		{
			std::cout << "DataWriter::WriteZipFile - Can't open file " << p_block->CompleteURL << "\n";
		}

		FileDelete( p_block->CompleteURL.char_str().data() );
	}
}



void DataWriter::WriteHeader( unsigned long long & l_nbBytes )
{
	char * l_buffer = new char[sizeof( int )];
	// Writes the number of blocks
	int l_nbBlocks = ( int )( m_header.size() );
	fwrite( &l_nbBlocks, 1, sizeof( int ), m_file );
	l_nbBytes += sizeof( int );
	wxString l_log;
	l_log << wxT( "WriteHeader - nbBlocks :\t\t" ) << l_nbBlocks << wxT( "\n" );
	m_log->WriteText( l_log );
	l_log.clear();

	// Writes each block in m_header
	for ( int i = 0 ; i < l_nbBlocks ; i++ )
	{
		l_log << wxT( "WriteHeader - block number :\t\t" ) << i << wxT( "\n" );
		m_log->WriteText( l_log );
		l_log.clear();
		EM_Block * block = m_header[i];
		// Writes the block type
		l_buffer = new char[1];
		l_log << wxT( "WriteHeader - block type :\t\t" ) << block->m_blocktype << wxT( "\n" );
		m_log->WriteText( l_log );
		l_log.clear();
		char l_buffer = ( char )( block->m_blocktype );
		fwrite( &l_buffer, 1, sizeof( char ), m_file );
		l_nbBytes += sizeof( char );
		// Writes the block name's length
		l_log << wxT( "WriteHeader - block name size :\t" ) << block->m_namesize << wxT( "\n" );
		m_log->WriteText( l_log );
		l_log.clear();
		l_buffer = ( char )( block->m_namesize );
		fwrite( &l_buffer, 1, sizeof( char ), m_file );
		l_nbBytes += sizeof( char );
		// Writes the block's name
		m_log->WriteText( wxT( "WriteHeader - block name :\t\t" ) + block->m_name + wxT( "\n" ) );
		fwrite( block->m_name, 1, block->m_namesize, m_file );
		l_nbBytes += block->m_namesize;
		// Writes the block's size
		l_log << wxT( "WriteHeader - block size :\t\t" ) << block->m_blocsize << wxT( "\n" );
		m_log->WriteText( l_log );
		l_log.clear();
		fwrite( &block->m_blocsize, 1, sizeof( int ), m_file );
		l_nbBytes += sizeof( int );
		// Writes the block's hash
		m_log->WriteText( wxString( wxT( "WriteHeader - block hash :\t\t" ) ) + make_wxString( block->m_hash ) + wxString( wxT( "\n" ) ) );
		fwrite( block->m_hash, 1, 32, m_file );
		l_nbBytes += 32;
	}
}



void DataWriter::WriteZipHeader( ZipBlockList p_list, FILE * p_file )
{
	//writing header size
	int l_nbBlocks = ( int )( p_list.size() );
	fwrite( &l_nbBlocks, 1, sizeof( int ), p_file );
	wxString l_log;
	l_log << wxT( "WriteZipHeader - nbBlocks :\t\t" ) << l_nbBlocks << wxT( "\n" );
	m_log->WriteText( l_log );
	l_log.clear();

	// writing each block information (size, namesize, name)
	for ( unsigned int i = 0 ; i < p_list.size() ; i++ )
	{
		l_log << wxT( "WriteZipHeader - block number :\t" ) << i << wxT( "\n" );
		m_log->WriteText( l_log );
		l_log.clear();
		EM_ZipBlock * l_block = p_list[i];
		// Writing size
		fwrite( &l_block->BlockSize, 1, sizeof( int ), p_file );
		l_log << wxT( "WriteZipHeader - block size :\t\t" ) << l_block->BlockSize << wxT( "\n" );
		m_log->WriteText( l_log );
		l_log.clear();
		// Writing name size
		fwrite( &l_block->NameSize, 1, sizeof( short ), p_file );
		l_log << wxT( "WriteZipHeader - block name's size :\t" ) << l_block->NameSize << wxT( "\n" );
		m_log->WriteText( l_log );
		l_log.clear();
		// Writing file name (without extension!!)
		fwrite( l_block->Name, 1, l_block->NameSize, p_file );
		m_log->WriteText( wxT( "WriteZipHeader - block's name :\t" ) + l_block->Name + wxT( "\n" ) );
	}
}



int DataWriter::GetType( const wxString & p_filename )
{
	wxString l_extension = GetExtension( p_filename );

	if ( l_extension.empty() )
	{
		return -1;
	}

	if ( l_extension == wxT( "emscene" ) )
	{
		return EM_BLOCK_SCENE;
	}
	else if ( l_extension == wxT( "emscript" ) )
	{
		return EM_BLOCK_INTERACTIONS;
	}
	else if ( l_extension == wxT( "zip" ) )
	{
		return EM_BLOCK_ZIPDATA;
	}
	else if ( l_extension == wxT( "gzip" ) )
	{
		return EM_BLOCK_GZIPDATA;
	}
	else if ( l_extension == wxT( "cfg" ) )
	{
		return EM_BLOCK_CONFIG;
	}
	else if ( l_extension == wxT( "emcfg" ) )
	{
		return EM_BLOCK_EMCONFIG;
	}
	else if ( l_extension == wxT( "wav" ) || l_extension == wxT( "mp3" ) || l_extension == wxT( "wma" ) )
	{
		return EM_BLOCK_SNDDATA;
	}
	else if ( l_extension == wxT( "emsnd" ) )
	{
		return EM_BLOCK_ZIPSNDDATA;
	}
	else
	{
		return 0;
	}
}



wxString DataWriter::GetName( const wxString & p_fileurl )
{
	wxString l_fileUrl = p_fileurl;
	l_fileUrl.Replace( wxT( '/' ), wxFileName::GetPathSeparator() );
	l_fileUrl.Replace( wxT( '\\' ), wxFileName::GetPathSeparator() );
	size_t l_index = l_fileUrl.find_last_of( wxFileName::GetPathSeparator() );
	wxString l_fileName = l_fileUrl.Mid( l_index + 1 );
	return l_fileName;
}



wxString DataWriter::GetNameWithoutExtension( const wxString & p_fileurl )
{
	wxString l_res = GetName( p_fileurl );
	size_t l_index = l_res.find_last_of( wxT( "." ) );

	if ( l_index >= p_fileurl.size() )
	{
		return l_res;
	}

	l_res = l_res.Mid( 0, l_index );
	return l_res;
}



wxString DataWriter::GetExtension( const wxString & p_fileurl )
{
	size_t l_index = p_fileurl.find_last_of( wxT( "." ) );

	if ( l_index >= p_fileurl.size() )
	{
		return wxEmptyString;
	}

	wxString l_res = p_fileurl.Mid( l_index + 1 );
	return l_res;
}



wxString DataWriter::GetParentFolder( const wxString & p_folder )
{
	wxString l_folder = p_folder;
	l_folder.Replace( wxT( '\\' ), wxFileName::GetPathSeparator() );
	l_folder.Replace( wxT( '/' ), wxFileName::GetPathSeparator() );
	int l_length = static_cast <int>( l_folder.size() );
	int i = l_length - 1;

	while ( i >= 0 && p_folder.at( i ) != wxFileName::GetPathSeparator() )
	{
		i--;
	}

	if ( l_folder.at( i ) == wxFileName::GetPathSeparator() )
	{
		wxString l_parentfolder;
		int j = 0;

		for ( int j = 0 ; j < i ; j++ )
		{
			l_parentfolder += l_folder[j];
		}

		return l_parentfolder;
	}

	return wxEmptyString;
}



void DataWriter::UpdateFileList( wxTextCtrl * p_listbox )
{
	for ( unsigned int i = 0 ; i < m_header.size() ; i++ )
	{
		p_listbox->WriteText( m_header[i]->m_completeUrl );
	}
}


