/*
This source file is part of ElypsePlayer (https://sourceforge.net/projects/elypse/)

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
the program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
*/
#include "PrecompiledHeader.h"

#include "MusePack.h"
#include "MuseFilePack.h"
#include "MuseDownloader.h"
#include "DownloadManager.h"

#include "DataFile.h"
#include "MuseFile.h"
#include "ConfigFile.h"
#include "ElypseController.h"
#include "ElypseLogs.h"

#include <STLMacros.h>
#include <MinMax.h>
#include <File.h>
#include <MD5.h>

#include <fstream>
#include <iostream>
#include <errno.h>
#include <zlib.h>

#pragma warning( disable:4996 )

MusePack::MusePack( MuseDownloader & p_owner,
					const Path & p_installPath,
					const Path & p_basePath,
					bool p_checkHeader )
	: owned_by< MuseDownloader >( p_owner )
	, m_timer( NULL )
	, m_installPath( p_installPath )
	, m_basePath( p_basePath )
	, m_museOK( false )
	, m_checkHeader( p_checkHeader )
	, m_nbHeader( 0 )
	, m_museFileSize( 0 )
	, m_totalByteDownloaded( 0 )
	, m_beginTime{}
{
	m_packPath = m_installPath / "rsc" / m_basePath.GetLeaf();
//	m_packPath.erase( m_packPath.size() - 5, String::npos);
	DirectoryCreate( m_packPath );
	EMUSE_MESSAGE_DEBUG( "MusePack::MusePack -> pack path : " + m_packPath );
	EMUSE_MESSAGE_DEBUG( "MusePack::MusePack -> install path : " + p_installPath );
	EMUSE_MESSAGE_DEBUG( "MusePack::MusePack -> base path : " + p_basePath );
	EMUSE_MESSAGE_DEBUG( "MusePack::MusePack -> checkheader : " + p_checkHeader );
}

MusePack::~MusePack()
{
	General::Utils::vector::deleteAll( m_files );
	m_waitingDatas.clear();

	if ( m_timer != NULL )
	{
		delete m_timer;
		m_timer = NULL;
	}
}

size_t MusePack::GetFileIndex( String const & p_name )const
{
	for ( size_t i = 0 ; i < m_files.size() ; i ++ )
	{
		if ( m_files[i]->GetFilename() == p_name )
		{
			return i;
		}
	}

	return String::npos;
}

String const & MusePack::GetFileName( size_t p_index )const
{
	if ( p_index >= m_files.size() )
	{
		return EMPTY_STRING;
	}

	return m_files[p_index]->GetFilename();
}

bool MusePack::IsDownloadFinished( uint32_t p_index )const
{
	if ( p_index >= m_files.size() || ! GetOwner()->IsInitialised() )
	{
		return false;
	}

	if ( GetOwner()->DownloadFiles() )
	{
		return m_files[p_index]->IsFinished();
	}

	return true;
}

bool MusePack::_getHeaders( size_t & p_indexBuffer )
{
	if ( m_files.size() == m_nbHeader )
	{
		return true;
	}

	MuseFilePack * l_file = NULL;

	while ( p_indexBuffer < m_waitingDatas.size() && m_files.size() < m_nbHeader )
	{
		if ( l_file == NULL )
		{
			l_file = new MuseFilePack( *this );
			m_files.push_back( l_file );
		}
		else if ( l_file->IsHeaderCompleted() )
		{
//			m_museFileSize += l_file->GetFileSize();
			l_file = new MuseFilePack( *this );
			m_files.push_back( l_file );
		}

		//bool l_canCompleteData = true;

		if ( ! l_file->ReadHeader( p_indexBuffer, m_waitingDatas ) )
		{
			return false;
		}
	}

	GetOwner()->SetInitialised( true );

	if ( _finishHeaders() )
	{
		_finishMuseFile();
		return false;
	}

	DirectoryDelete( m_packPath );
	DirectoryCreate( m_packPath );
	return true;
}

bool MusePack::_finishHeaders()
{
	if ( _verifyFiles() )
	{
		// Tous les fichiers sont bons, pas besoin d'aller plus loin
		return true;
	}

	if ( ! GetOwner()->DownloadFiles() )
	{
		EMUSE_MESSAGE_DEBUG( "MusePack::_finishHeaders -> not downloading" );
		return true;
	}

	return false;
}

bool MusePack::_getNumFiles( size_t & p_indexBuffer )
{
	int l_nbHeader;

	if ( m_checkHeader )
	{
		String l_string( & m_waitingDatas[0], 8 );

		if ( l_string != "MUSEFILE" )
		{
			EMUSE_MESSAGE_DEBUG( "MusePack::_getNumFiles -> Error : musefile header not present" );
//			return false;
		}
		else
		{
			p_indexBuffer += 8;
		}
	}

	memcpy( & l_nbHeader, & m_waitingDatas[p_indexBuffer], sizeof( int ) );
	p_indexBuffer += sizeof( int );

	if ( l_nbHeader <= 0 )
	{
		/* Erreur */
		EMUSE_MESSAGE_RELEASE( "MusePack::_getNumFiles -> Error in getting file - [" + m_packPath + "], l_nbHeader : [" + StringConverter::toString( l_nbHeader ) + "]" );
		GetOwner()->SetCantDownload( true );
		return false;
	}

	m_nbHeader = unsigned( l_nbHeader );
	return true;
}

size_t MusePack::CurlCallback( void * p_buffer, size_t p_size )
{
	if ( m_timer == NULL )
	{
		m_timer = new PreciseTimer();
		m_beginTime = m_timer->SaveTime();
	}

	/* Unification des buffers */
	size_t l_oldSize = m_waitingDatas.size();
	m_waitingDatas.resize( l_oldSize + p_size );
	memcpy( & m_waitingDatas[l_oldSize], p_buffer, p_size );
	size_t l_indexBuffer = 0;

	if ( m_nbHeader == 0 )
	{
		/*
		if (m_checkHeader)
		{
			l_indexBuffer = sizeof( "MUSEFILE") - 1;
		}
		else
		{
			l_indexBuffer = 0;
		}
		*/
		if ( m_waitingDatas.size() < sizeof( int ) + l_indexBuffer )
		{
			return p_size;
		}

		if ( ! _getNumFiles( l_indexBuffer ) )
		{
			return p_size + 1;
		}
	}

	if ( m_nbHeader < m_files.size() )
	{
		EMUSE_MESSAGE_RELEASE( "MusePack::CurlCallback -> Error in getting file - [" + m_packPath + "] (m_nbHeader < m_files.size())" );
		GetOwner()->SetCantDownload( true );
		return p_size + 1;
	}

	if ( l_indexBuffer >= m_waitingDatas.size() )
	{
		m_waitingDatas.clear();
		return p_size;
	}

	if ( ! _getHeaders( l_indexBuffer ) )
	{
		return p_size + 1;
	}

	while ( l_indexBuffer < m_waitingDatas.size() )
	{
		size_t l_nbHeader = m_files.size();

		for ( size_t i = 0 ; l_indexBuffer < m_waitingDatas.size() && i < l_nbHeader ; i ++ )
		{
			if ( ! m_files[i]->ReadContents( l_indexBuffer, m_waitingDatas ) )
			{
				EMUSE_MESSAGE_RELEASE( "MusePack::CurlCallback -> Error in reading contents - [" + m_packPath + "]" );
				return p_size + 1;
			}
		}
	}

	if ( _downloadFinished() )
	{
		_finishMuseFile();
	}

	l_oldSize = m_waitingDatas.size() - l_indexBuffer;

	if ( l_oldSize <= 0 )
	{
		m_waitingDatas.clear();
		return p_size;
	}

	DataArray l_endDatas;
	l_endDatas.resize( l_oldSize );
	memcpy( & l_endDatas[0], & m_waitingDatas[l_indexBuffer], l_oldSize );
	m_waitingDatas.resize( l_oldSize );
	memcpy( & m_waitingDatas[0], & l_endDatas[0], l_oldSize );
	return p_size;
}

bool MusePack::_verifyFiles()
{
	size_t l_nbHeaders = m_files.size();

	for ( size_t i = 0 ; i < l_nbHeaders ; i ++ )
	{
		if ( ! m_files[i]->IsHeaderCompleted() )
		{
			return false;
		}

		if ( ! FileExists( m_packPath / m_files[i]->GetFilename() ) )
		{
			return false;
		}

		String l_md5 = MD5::HashFile( m_packPath / m_files[i]->GetFilename() );

		if ( m_files[i]->GetMD5() != l_md5 )
		{
			return false;
		}
	}

	return true;
}

bool MusePack::_downloadFinished()
{
	size_t l_nbHeaders = m_files.size();

	for ( size_t i = 0 ; i < l_nbHeaders ; i ++ )
	{
		if ( ! m_files[i]->IsDownloadFinished() )
		{
			return false;
		}
	}

	return true;
}

void MusePack::_finishMuseFile()
{
	EMUSE_LOG_MESSAGE_RELEASE( "_finish muse file : " + m_packPath );
	size_t l_nbHeaders = m_files.size();
	MuseFile * l_museFile = GetOwner()->GetOwner();

	for ( size_t i = 0 ; i < l_nbHeaders ; i ++ )
	{
		MuseFilePack * l_file = m_files[i];
		l_file->SetFinished();

		if ( l_file->GetType() == EM_BLOCK_ZIPSNDDATA )
		{
			String l_directoryName = l_file->GetFilename().substr( 0, l_file->GetFilename().find_last_of( "." ) );
			_treatGZipFile( m_packPath, l_file->GetFilename(), l_directoryName );
		}
		else if ( l_file->GetType() == EM_BLOCK_GZIPDATA )
		{
			String l_directoryName = l_file->GetFilename().substr( 0, l_file->GetFilename().find_last_of( "." ) + 1 ) + "zip_";
			_treatGZipFile( m_packPath, l_file->GetFilename(), l_directoryName );
		}
		else if ( l_file->GetType() == EM_BLOCK_ZIPDATA )
		{
			DataFile * l_dataFile = l_museFile->GetDataFile( l_file->GetFilename() );

			if ( l_dataFile == NULL )
			{
				EMUSE_MESSAGE_RELEASE( "MusePack::_finishMuseFile -> Major error in getting DataFile from DownloadManager" );
				continue;
			}

			l_dataFile->DownloadFinished();
		}
		else if ( l_file->GetType() == EM_BLOCK_INTERACTIONS || l_file->GetType() == EM_BLOCK_SCENE || l_file->GetType() == EM_BLOCK_EMCONFIG )
		{
			ConfigFile * l_configFile = l_museFile->GetConfigFile( l_file->GetFilename() );

			if ( l_configFile == NULL )
			{
				EMUSE_MESSAGE_RELEASE( "MusePack::_finishMuseFile -> Major error in getting ConfigFile from DownloadManager" );
				continue;
			}

			l_configFile->DownloadFinished();
		}
	}

	m_museOK = true;
}

bool MusePack::_treatGZipFile( const Path & p_basePath, String const & p_fileName, String const & p_folderName )
{
	if ( ! GetOwner()->DownloadFiles() )
	{
		return false;
	}

	if ( ! FileExists( p_basePath / p_fileName ) )
	{
		EMUSE_MESSAGE_RELEASE( "MusePack::_treatGZipFile : File[" + p_basePath / p_fileName + "] doesn't exists..." );
		return false;
	}

	if ( ! DirectoryExists( p_basePath / p_folderName ) )
	{
		EMUSE_MESSAGE_RELEASE( "MusePack::_treatGZipFile : Directory[" + p_basePath / p_folderName + "] doesn't exists..." );
		DirectoryCreate( p_basePath / p_folderName );
	}

	FILE * l_file = fopen( ( p_basePath / p_fileName ).c_str(), "rb" );

	if ( l_file == NULL )
	{
		EMUSE_MESSAGE_RELEASE( "MusePack::_treatGZipFile : Error while opening sound file : [" + p_basePath + "/" + p_fileName + "] : " + String( strerror( errno ) ) );
		return false;
	}

	int l_nbHeader = 0;

	if ( fread( & l_nbHeader, sizeof( int ), 1, l_file ) != 1 )
	{
		if ( ferror( l_file ) != 0 )
		{
			EMUSE_MESSAGE_RELEASE( "MusePack::_treatGZipFile : Error while reading file : [" + p_basePath + "/" + p_fileName + "]" );
			fclose( l_file );
			return false;
		}

		EMUSE_MESSAGE_RELEASE( "MusePack::_treatGZipFile : End of file [" + p_basePath + "/" + p_fileName + "] reached but unexpected..." );
		fclose( l_file );
		return false;
	}

	if ( l_nbHeader <= 0 )
	{
		EMUSE_MESSAGE_RELEASE( "MusePack::_treatGZipFile : Error with file [" + p_basePath + "/" + p_fileName + "] l_nbHeader <= 0" );
		fclose( l_file );
		return false;
	}

	SndHeaderArray l_sndList;

	for ( uint32_t i = 0 ; i < unsigned( l_nbHeader ); i ++ )
	{
		EM_SndHeader * l_sndHeader = new EM_SndHeader;
		l_sndList.push_back( l_sndHeader );

		if ( fread( & l_sndHeader->fileSize, sizeof( int ), 1, l_file ) != 1 )
		{
			if ( ferror( l_file ) != 0 )
			{
				EMUSE_MESSAGE_RELEASE( "MusePack::_treatGZipFile : Error while reading file : [" + p_basePath + "/" + p_fileName + "]" );
				General::Utils::vector::deleteAll( l_sndList );
				fclose( l_file );
				return false;
			}

			EMUSE_MESSAGE_RELEASE( "MusePack::_treatGZipFile : End of file [" + p_basePath + "/" + p_fileName + "] reached but unexpected..." );
			General::Utils::vector::deleteAll( l_sndList );
			fclose( l_file );
			return false;
		}

		if ( l_sndHeader->fileSize <= 0 )
		{
			EMUSE_MESSAGE_RELEASE( "MusePack::_treatGZipFile : File size is invalid : [" + StringConverter::toString( l_sndHeader->fileSize ) + "]" );
			General::Utils::vector::deleteAll( l_sndList );
			fclose( l_file );
			return false;
		}

		if ( fread( & l_sndHeader->fileNameSize, sizeof( short ), 1, l_file ) != 1 )
		{
			if ( ferror( l_file ) != 0 )
			{
				EMUSE_MESSAGE_RELEASE( "MusePack::_treatGZipFile : Error while reading file : [" + p_basePath + "/" + p_fileName + "]" );
				General::Utils::vector::deleteAll( l_sndList );
				fclose( l_file );
				return false;
			}

			EMUSE_MESSAGE_RELEASE( "MusePack::_treatGZipFile : End of file [" + p_basePath + "/" + p_fileName + "] reached but unexpected..." );
			General::Utils::vector::deleteAll( l_sndList );
			fclose( l_file );
			return false;
		}

		if ( l_sndHeader->fileNameSize <= 0 )
		{
			EMUSE_MESSAGE_RELEASE( "MusePack::_treatGZipFile : File size is invalid : [" + StringConverter::toString( l_sndHeader->fileSize ) + "]" );
			General::Utils::vector::deleteAll( l_sndList );
			fclose( l_file );
			return false;
		}

		l_sndHeader->fileName.resize( l_sndHeader->fileNameSize + 1 );

		if ( fread( l_sndHeader->fileName.data(), sizeof( char ), l_sndHeader->fileNameSize, l_file ) != l_sndHeader->fileNameSize )
		{
			if ( ferror( l_file ) != 0 )
			{
				EMUSE_MESSAGE_RELEASE( "MusePack::_treatGZipFile : Error while reading file : [" + p_basePath + "/" + p_fileName + "]" );
				General::Utils::vector::deleteAll( l_sndList );
				fclose( l_file );
				return false;
			}

			EMUSE_MESSAGE_RELEASE( "MusePack::_treatGZipFile : End of file [" + p_basePath + "/" + p_fileName + "] reached but unexpected..." );
			General::Utils::vector::deleteAll( l_sndList );
			fclose( l_file );
			return false;
		}

		l_sndHeader->fileName[l_sndHeader->fileNameSize] = '\0';
	}

	for ( uint32_t i = 0 ; i < unsigned( l_nbHeader ); i ++ )
	{
		EM_SndHeader * l_sndHeader = l_sndList[i];
		FILE * l_zipFile = fopen( ( p_basePath / p_folderName / l_sndHeader->fileName.data() + ".gz" ).c_str(), "wb" );

		if ( l_zipFile == NULL )
		{
			EMUSE_MESSAGE_RELEASE( "MusePack::_treatGZipFile : Error while opening zip file : [" + p_basePath / p_fileName + "] : " + String( strerror( errno ) ) );
			General::Utils::vector::deleteAll( l_sndList );
			fclose( l_file );
			fclose( l_zipFile );
			return false;
		}

		if ( l_sndHeader->fileSize == 0 )
		{
			fclose( l_zipFile );
			continue;
		}

		uint32_t l_nbWritten = 0;
		uint32_t l_nbDataToWrite = 8192;
		char * l_buffer = new char[8192];

		while ( l_sndHeader->fileSize > l_nbWritten )
		{
			l_nbDataToWrite = min( 8192u, l_sndHeader->fileSize - l_nbWritten );

			if ( fread( l_buffer, sizeof( char ), l_nbDataToWrite, l_file ) != l_nbDataToWrite )
			{
				if ( ferror( l_file ) != 0 )
				{
					EMUSE_MESSAGE_RELEASE( "MusePack::_treatGZipFile : Error while reading file [" + p_basePath / p_fileName + "]" );
					delete [] l_buffer;
					General::Utils::vector::deleteAll( l_sndList );
					fclose( l_file );
					fclose( l_zipFile );
					return false;
				}

				EMUSE_MESSAGE_RELEASE( "MusePack::_treatGZipFile : End of file [" + p_basePath + "/" + p_fileName + "] reached but unexpected..." );
				delete [] l_buffer;
				General::Utils::vector::deleteAll( l_sndList );
				fclose( l_file );
				fclose( l_zipFile );
				return false;
			}

			if ( fwrite( l_buffer, sizeof( char ), l_nbDataToWrite, l_zipFile ) != l_nbDataToWrite )
			{
				if ( ferror( l_file ) != 0 )
				{
					EMUSE_MESSAGE_RELEASE( "MusePack::_treatGZipFile : Error while writing into file [" + p_basePath + "/" + p_folderName + "/" + l_sndHeader->fileName.data() + "]" );
					delete [] l_buffer;
					General::Utils::vector::deleteAll( l_sndList );
					fclose( l_file );
					fclose( l_zipFile );
					return false;
				}

				EMUSE_MESSAGE_RELEASE( "MusePack::_treatGZipFile : End of file [" + p_basePath + "/" + p_fileName + "] reached but unexpected..." );
				delete [] l_buffer;
				General::Utils::vector::deleteAll( l_sndList );
				fclose( l_file );
				fclose( l_zipFile );
				return false;
			}

			l_nbWritten += l_nbDataToWrite;
		}

		delete [] l_buffer;
		fclose( l_zipFile );
		DecompressFile( p_basePath / p_folderName / l_sndHeader->fileName.data() + ".gz",
						p_basePath / p_folderName / l_sndHeader->fileName.data(), 8192 );
		FileDelete( p_basePath / p_folderName / l_sndHeader->fileName.data() + ".gz" );
	}

	General::Utils::vector::deleteAll( l_sndList );
	fclose( l_file );
	/* Soit on détruit le fichier ".emsnd" et l'on n'effectue pas la vérif sur les fichiers ".emsnd", ou alors on laisse le fichier en place. */
	//DeleteFile( p_basePath + "/" + p_sndFileName);
	return true;
}

bool MusePack::DecompressFile( String const & p_zipFileName, String const & p_fileName, int p_size )
{
	bool l_return = true;
	gzFile l_infile = gzopen( p_zipFileName.c_str(), "r" );

	if ( l_infile == Z_NULL )
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
				EMUSE_LOG_MESSAGE_NORMAL( "Can't deflate file [" + p_fileName + "] due to error " + std::to_string( errno ) );
				break;

			case Z_STREAM_END:
				EMUSE_LOG_MESSAGE_NORMAL( "Can't deflate file [" + p_fileName + "] due to Z_STREAM_END error" );
				break;

			case Z_DATA_ERROR:
				EMUSE_LOG_MESSAGE_NORMAL( "Can't deflate file [" + p_fileName + "] due to Z_DATA_ERROR error" );
				break;

			case Z_STREAM_ERROR:
				EMUSE_LOG_MESSAGE_NORMAL( "Can't deflate file [" + p_fileName + "] due to Z_STREAM_ERROR error" );
				break;

			case Z_NEED_DICT:
				EMUSE_LOG_MESSAGE_NORMAL( "Can't deflate file [" + p_fileName + "] due to Z_NEED_DICT error" );
				break;

			case Z_MEM_ERROR:
				EMUSE_LOG_MESSAGE_NORMAL( "Can't deflate file [" + p_fileName + "] due to Z_MEM_ERROR error" );
				break;
			}
		}

		fclose( l_outfile );
		gzclose( l_infile );
	}

	return l_return;
}

std::string MusePack::DataType2String( DataBlockType p_dataType )
{
	switch ( p_dataType )
	{
	case EM_BLOCK_ZIPDATA:
		return String( "EM_BLOCK_ZIPDATA" );

	case EM_BLOCK_GZIPDATA:
		return String( "EM_BLOCK_GZIPDATA" );

	case EM_BLOCK_SCENE:
		return String( "EM_BLOCK_SCENE" );

	case EM_BLOCK_INTERACTIONS:
		return String( "EM_BLOCK_INTERACTIONS" );

	case EM_BLOCK_SNDDATA:
		return String( "EM_BLOCK_SNDDATA" );

	case EM_BLOCK_CONFIG:
		return String( "EM_BLOCK_CONFIG" );

	case EM_BLOCK_ZIPSNDDATA:
		return String( "EM_BLOCK_ZIPSNDDATA" );

	case EM_BLOCK_EMCONFIG:
		return String( "EM_BLOCK_EMCONFIG" );

	case EM_BLOCK_ERROR:
		return String( "EM_BLOCK_ERROR" );

	default:
		return String( "Not a valid data type" );
	}
}

