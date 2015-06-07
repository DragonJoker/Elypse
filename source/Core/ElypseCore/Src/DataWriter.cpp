/*********************************************************************************************************************

	Author :	Sylvain DOREMUS

	Company :	ForDev Studio - Copyright 2006

	Files :		DataWriter.h - DataWriter.cpp

	Desc :		Class used to write the datas used in EMuse.

*********************************************************************************************************************/
#include "PrecompiledHeader.h"

#include "DataWriter.h"
#include <iostream>

#include <zlib.h>

#include <File.h>
#include <MD5.h>
#include <STLMacros.h>
#include <StringUtils.h>

#pragma warning( disable:4996 )

namespace EMuse
{
	namespace Download
	{
		namespace
		{
			template< typename T >
			void WriteValue( T const & p_value, FILE * p_file, size_t & p_nbBytes )
			{
				size_t l_written = fwrite( &p_value, 1, sizeof( T ), p_file );
				assert( l_written == sizeof( T ) );
				p_nbBytes += l_written;
			}

			template< typename T >
			void WriteValue( std::string const & p_value, T p_size, FILE * p_file, size_t & p_nbBytes )
			{
				size_t l_written = fwrite( p_value.c_str(), 1, p_size, p_file );
				assert( l_written == p_size );
				p_nbBytes += l_written;
			}

			void Log( std::string const & p_name )
			{
				std::cout << p_name << std::endl;
			}

			template< typename T >
			void Log( std::string const & p_name, T const & p_value )
			{
				std::cout << "\t" << p_name << ":" << p_value << std::endl;
			}

			bool DecompressFile( const std::string & p_zipFileName, const std::string & p_fileName, int p_size )
			{
				bool l_return = true;
				gzFile l_infile = gzopen( p_zipFileName.c_str(), "r" );

				if ( !l_infile )
				{
					l_return = false;
				}

				FILE * l_outfile = NULL;

				if ( l_return )
				{
					l_outfile = fopen( p_fileName.c_str(), "wb" );

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
							std::cerr << "Can't inflate file [" + p_fileName + "] due to error " + std::to_string( errno ) << std::endl;
							break;

						case Z_STREAM_END:
							std::cerr << "Can't inflate file [" + p_fileName + "] due to Z_STREAM_END error" << std::endl;
							break;

						case Z_DATA_ERROR:
							std::cerr << "Can't inflate file [" + p_fileName + "] due to Z_DATA_ERROR error" << std::endl;
							break;

						case Z_STREAM_ERROR:
							std::cerr << "Can't inflate file [" + p_fileName + "] due to Z_STREAM_ERROR error" << std::endl;
							break;

						case Z_NEED_DICT:
							std::cerr << "Can't inflate file [" + p_fileName + "] due to Z_NEED_DICT error" << std::endl;
							break;

						case Z_MEM_ERROR:
							std::cerr << "Can't inflate file [" + p_fileName + "] due to Z_MEM_ERROR error" << std::endl;
							break;
						}
					}

					fclose( l_outfile );
					gzclose( l_infile );
				}

				return l_return;
			}

			std::string GetName( const std::string & p_fileurl )
			{
				return Path( p_fileurl ).GetLeaf();
			}

			std::string GetParentFolder( const std::string & p_folder )
			{
				return Path( p_folder ).GetPath();
			}

			std::string GetNameWithoutExtension( const std::string & p_fileurl )
			{
				std::string l_res = GetName( p_fileurl );
				size_t l_index = l_res.find_last_of( "." );

				if ( l_index < p_fileurl.size() )
				{
					l_res = l_res.substr( 0, l_index );
				}

				return l_res;
			}

			std::string GetExtension( const std::string & p_fileurl )
			{
				size_t l_index = p_fileurl.find_last_of( "." );
				std::string l_res;

				if ( l_index < p_fileurl.size() )
				{
					l_res = p_fileurl.substr( l_index + 1 );
				}

				return l_res;
			}

			DataBlockType GetType( const std::string & p_filename )
			{
				DataBlockType l_return = EM_BLOCK_UNKNOWN;
				std::string l_extension = GetExtension( p_filename );

				if ( l_extension.empty() )
				{
					l_return = EM_BLOCK_ERROR;
				}
				else if ( l_extension == "emscene" )
				{
					l_return = EM_BLOCK_SCENE;
				}
				else if ( l_extension == "emscript" )
				{
					l_return = EM_BLOCK_INTERACTIONS;
				}
				else if ( l_extension == "zip" )
				{
					l_return = EM_BLOCK_ZIPDATA;
				}
				else if ( l_extension == "gzip" )
				{
					l_return = EM_BLOCK_GZIPDATA;
				}
				else if ( l_extension == "cfg" )
				{
					l_return = EM_BLOCK_CONFIG;
				}
				else if ( l_extension == "emcfg" )
				{
					l_return = EM_BLOCK_EMCONFIG;
				}
				else if ( l_extension == "wav" || l_extension == "mp3" || l_extension == "wma" )
				{
					l_return = EM_BLOCK_SNDDATA;
				}
				else if ( l_extension == "emsnd" )
				{
					l_return = EM_BLOCK_ZIPSNDDATA;
				}

				return l_return;
			}
		}

		DataWriter::DataWriter()
		{
		}

		DataWriter::~DataWriter()
		{
			m_header.clear();
			m_fileslist.clear();
		}

		bool DataWriter::AddFolder( const std::string & p_folder, Download::DataBlockType p_eType )
		{
			bool l_return = false;
			m_fileslist.clear();
			std::string l_folder = GetParentFolder( p_folder );

			if ( !l_folder.empty() )
			{
				std::string l_foldername = GetName( p_folder );

				if ( !l_foldername.empty() )
				{
					ListFiles( p_folder );
					std::string l_path = l_folder + d_path_slash + l_foldername + Download::DataTypeExtension[p_eType];

					if ( CompressFolder( l_folder, l_path, p_eType ) )
					{
						std::cout << "[" << l_path << "]" << std::endl;
						l_return = AddFile( l_path, p_eType );
					}
				}
			}

			return l_return;
		}

		void DataWriter::ListFiles( const std::string & p_folder )
		{
			ListDirectoryFiles( p_folder, [this]( std::string const & p_path )
			{
				m_fileslist.push_back( p_path );
			} );
		}

		int DataWriter::CompressFile( const std::string & p_infilename, const std::string & p_outfilename )
		{
			size_t l_totalWritten = -1;
			FILE * l_infile = fopen( p_infilename.c_str(), "rb" );

			if ( l_infile )
			{
				gzFile l_outfile = gzopen( p_outfilename.c_str(), "wb" );

				if ( !l_outfile )
				{
					fclose( l_infile );
				}
				else
				{
					uint8_t l_buffer[128];
					size_t l_nbBytesRead = 0;
					size_t l_totalRead = 0;
					l_totalWritten = 0;

					while ( ( l_nbBytesRead = fread( l_buffer, sizeof( uint8_t ), sizeof( l_buffer ), l_infile ) ) > 0 )
					{
						l_totalRead += l_nbBytesRead;
						l_totalWritten += gzwrite( l_outfile, l_buffer, l_nbBytesRead );
					}

					fclose( l_infile );
					gzclose( l_outfile );
					l_infile = fopen( p_outfilename.c_str(), "rb" );
					fseek( l_infile, 0, SEEK_END );
					l_totalWritten = ftell( l_infile );
					fclose( l_infile );
				}
			}

			return l_totalWritten;
		}

		bool DataWriter::CompressFolder( const Path & p_outFolder, const Path & p_outPath, DataBlockType p_eType )
		{
			ZipBlockList l_zipHeader;
			std::string l_fileName, l_extension, l_outFile, l_outpath;

			// We compress each file
			for ( auto && l_file : m_fileslist )
			{
				std::string l_fileName = GetName( l_file );
				Path l_outFile = p_outFolder + d_path_slash + l_fileName + ".zip";
				int l_size = CompressFile( l_file, l_outFile );
				EM_ZipBlock l_block = { l_size, static_cast< uint32_t >( l_fileName.size() ), l_fileName, l_outFile };
				l_zipHeader.push_back( l_block );
			}

			FILE * l_file = fopen( p_outPath.c_str(), "wb" );
			bool l_return = false;

			if ( l_file )
			{
				WriteZipHeader( l_zipHeader, l_file );
				WriteZipFile( l_zipHeader, l_file );
				fclose( l_file );
				l_return = true;
			}

			return l_return;
		}

		bool DataWriter::AddFile( const std::string & p_fileurl, DataBlockType p_eType )
		{
			bool l_return = false;
			EM_Block block;
			block.m_completeUrl = p_fileurl;
			FILE * l_file = fopen( block.m_completeUrl.c_str(), "rb" );

			if ( !l_file )
			{
				std::cerr << "Can't open " << block.m_completeUrl << std::endl;
			}
			else
			{
				struct stat tagStat;
				int ret = stat( block.m_completeUrl.c_str(), & tagStat );
				block.m_blocsize = uint32_t( tagStat.st_size );
				MD5 l_md5;
				block.m_hash = l_md5.HashFile( l_file );
				fclose( l_file );

				block.m_name = GetName( p_fileurl );

				if ( !block.m_name.empty() )
				{
					block.m_namesize = uint8_t( block.m_name.size() );
					block.m_blocktype = p_eType;

					if ( block.m_blocktype == EM_NB_BLOCK_TYPES )
					{
						block.m_blocktype = GetType( block.m_name );
					}

					if ( block.m_blocktype > EM_BLOCK_UNKNOWN && block.m_blocktype <= EM_NB_BLOCK_TYPES )
					{
						m_header.push_back( block );
						l_return = true;
					}
				}
			}

			return l_return;
		}

		bool DataWriter::DelFile( uint32_t p_index )
		{
			bool l_return = false;

			if ( p_index < m_header.size() )
			{
				m_header.erase( m_header.begin() + p_index );
				l_return = true;
			}

			return l_return;
		}

		bool DataWriter::DownFile( uint32_t p_index )
		{
			bool l_return = false;

			if ( p_index < m_header.size() - 1 )
			{
				std::swap( m_header[p_index], m_header[p_index + 1] );
				l_return = true;
			}

			return l_return;
		}

		bool DataWriter::UpFile( uint32_t p_index )
		{
			bool l_return = false;

			if ( p_index >= 1 && p_index < m_header.size() )
			{
				std::swap( m_header[p_index], m_header[p_index - 1] );
				l_return = true;
			}

			return l_return;
		}

		bool DataWriter::Write( const std::string & p_filename )
		{
			bool l_return = false;

			if ( !m_header.empty() )
			{
				int l_length = ( int )p_filename.size();
				std::string l_filename = p_filename;
				FILE * l_outfile = fopen( l_filename.c_str(), "wb" );

				if ( l_outfile )
				{
					char l_buffer[] = "MUSEFILE";
					size_t l_nbBytes = fwrite( l_buffer, 1, strlen( l_buffer ), l_outfile );
					std::cout << "Header written" ;
					std::cout << ", from byte: " << l_nbBytes;
					WriteHeader( l_nbBytes, l_outfile );
					std::cout << ", to byte: " << l_nbBytes << std::endl;

					for ( auto && l_block : m_header )
					{
						std::clog << l_block.m_completeUrl << "\n";
						FILE * l_infile = fopen( l_block.m_completeUrl.c_str(), "rb" );

						if ( l_infile )
						{
							std::cout << "Integrating file: " + l_block.m_name;
							std::cout << ", from byte: " << l_nbBytes;
							int l_size = l_block.m_blocsize;
							l_nbBytes += l_size;
							std::cout << ", to byte: " << l_nbBytes << std::endl;
							std::vector< uint8_t > l_readBuffer( l_size );
							fread( l_readBuffer.data(), 1, l_size, l_infile );
							fwrite( l_readBuffer.data(), 1, l_size, l_outfile );
							fclose( l_infile );
						}
					}

					std::clog << "File Length : " << l_nbBytes << std::endl;
					fclose( l_outfile );
				}

				l_return = true;
			}

			return l_return;
		}

		void DataWriter::WriteZipFile( ZipBlockList p_list, FILE * p_file )
		{
			for ( auto && l_block : p_list )
			{
				FILE * l_file = fopen( l_block.CompleteURL.c_str(), "rb" );

				if ( l_file )
				{
					uint32_t l_size = uint32_t( l_block.BlockSize );
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
					std::cerr << "DataWriter::WriteZipFile - Can't open file " << l_block.CompleteURL << "\n";
				}

				General::Files::FileDelete( l_block.CompleteURL.c_str() );
			}
		}

		void DataWriter::WriteHeader( size_t & l_nbBytes, FILE * p_file )
		{
			// Writes the number of blocks
			WriteValue( uint32_t( m_header.size() ), p_file, l_nbBytes );
			Log( "Blocks count", uint32_t( m_header.size() ) );

			// Writes each block in m_header
			for ( auto && l_block : m_header )
			{
				Log( "Block" );
				// Writes the block type
				Log( "Type", int( l_block.m_blocktype ) );
				WriteValue( l_block.m_blocktype, p_file, l_nbBytes );
				// Writes the block name's length
				Log( "Name size", int( l_block.m_namesize ) );
				WriteValue( l_block.m_namesize, p_file, l_nbBytes );
				// Writes the block's name
				Log( "Name", l_block.m_name );
				WriteValue( l_block.m_name, l_block.m_namesize, p_file, l_nbBytes );
				// Writes the block's size
				Log( "Size", l_block.m_blocsize );
				WriteValue( l_block.m_blocsize, p_file, l_nbBytes );
				// Writes the block's hash
				Log( "Hash", l_block.m_hash );
				WriteValue( l_block.m_hash, 32, p_file, l_nbBytes );
			}
		}

		void DataWriter::WriteZipHeader( ZipBlockList p_list, FILE * p_file )
		{
			size_t l_nbBytes = 0;
			//writing header size
			WriteValue( uint32_t( p_list.size() ), p_file, l_nbBytes );
			Log( "Blocks count", uint32_t( m_header.size() ) );

			// writing each block information (size, namesize, name)
			for ( auto && l_block : p_list )
			{
				Log( "Block" );
				// Writing size
				WriteValue( l_block.BlockSize, p_file, l_nbBytes );
				Log( "Size", int( l_block.BlockSize ) );
				// Writing name size
				WriteValue( l_block.NameSize, p_file, l_nbBytes );
				Log( "Name size", int( l_block.NameSize ) );
				// Writing file name (without extension!!)
				WriteValue( l_block.Name, l_block.NameSize, p_file, l_nbBytes );
				Log( "Name", l_block.NameSize );
			}
		}

		std::vector< std::string > DataWriter::GetFileList()
		{
			std::vector< std::string > l_return;

			for ( unsigned int i = 0; i < m_header.size(); i++ )
			{
				l_return.push_back( m_header[i].m_completeUrl );
			}

			return l_return;
		}
	}
}
