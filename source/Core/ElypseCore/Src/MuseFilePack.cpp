#include "PrecompiledHeader.h"

#include "MuseFilePack.h"
#include "MuseDownloader.h"
#include "MusePack.h"
#include "DownloadManager.h"

#include "MuseFile.h"

#include "EMuseController.h"
#include "EMuseLogs.h"

#pragma warning( disable:4996 )

namespace
{
	const unsigned short MAX_FILENAMESIZE	= 255;
	const unsigned short MAX_MD5SIZE		= 32;
}

bool MuseFilePack :: ReadHeader( size_t & p_indexBuffer, const DataArray & p_waitingDatas )
{
	while ( p_indexBuffer < p_waitingDatas.size()
			&& !IsHeaderCompleted() )
	{
		switch ( m_status )
		{
		case HEADEREMPTY:
		{
			if ( p_waitingDatas.size() - p_indexBuffer < sizeof( char ) )
			{
				return true;
			}

			m_type = static_cast <DataBlockType>( p_waitingDatas[p_indexBuffer ++] );

			if ( m_type <= 0 || m_type > EM_NB_BLOCK_TYPES )
			{
				m_status = HEADERERROR;
				return false;
			}

			m_status = TYPEISOK;
//				break;
		}

		case TYPEISOK:
		{
			if ( p_waitingDatas.size() - p_indexBuffer < sizeof( char ) )
			{
				return true;
			}

			size_t l_size = static_cast <size_t>( p_waitingDatas[p_indexBuffer ++] );

			if ( l_size == 0 || l_size > MAX_FILENAMESIZE )
			{
				m_status = HEADERERROR;
				return false;
			}

			SetFilenameSize( l_size );
			m_status = FILENAMESIZEISOK;
//				break;
		}

		case FILENAMESIZEISOK:
		{
			if ( m_filenameSize == 0 || m_filenameSize > MAX_FILENAMESIZE )
			{
				m_status = HEADERERROR;
				return false;
			}

			if ( p_waitingDatas.size() - p_indexBuffer < m_filenameSize )
			{
				return true;
			}

			SetFilename( & p_waitingDatas[p_indexBuffer] );
			m_status = FILENAMEISOK;
			p_indexBuffer += m_filenameSize;

			if ( m_type == EM_BLOCK_ZIPDATA )
			{
				m_owner->GetOwner()->GetOwner()->GetDataFile( m_filename );
			}
			else if ( m_type == EM_BLOCK_INTERACTIONS || m_type == EM_BLOCK_SCENE || m_type == EM_BLOCK_EMCONFIG )
			{
				m_owner->GetOwner()->GetOwner()->GetConfigFile( m_filename );
			}

//				break;
		}

		case FILENAMEISOK:
		{
			if ( p_waitingDatas.size() - p_indexBuffer < sizeof( int ) )
			{
				return true;
			}

			memcpy( & m_fileSize, & p_waitingDatas[p_indexBuffer], sizeof( int ) );
			p_indexBuffer += sizeof( int );
			m_owner->AddFileSize( static_cast<unsigned int>( m_fileSize ) );
			m_status = FILESIZEISOK;
//				break;
		}

		case FILESIZEISOK:
		{
			if ( p_waitingDatas.size() - p_indexBuffer < MAX_MD5SIZE )
			{
				return true;
			}

			m_md5.assign( & p_waitingDatas[p_indexBuffer], MAX_MD5SIZE );
			p_indexBuffer += MAX_MD5SIZE;
			m_status = HEADERCOMPLETE;
//				break;
		}

		default:
		{
			return true;
		}
		}
	}

	return true;
}

bool MuseFilePack :: ReadContents( size_t & p_indexBuffer, const DataArray & p_waitingDatas )
{
	if ( m_fileSize <= m_downloadedBytes )
	{
		if ( m_fileSize == 0 )
		{
			fclose( fopen( ( m_owner->GetPackPath() / m_filename ).c_str(), "wb+" ) );
		}

		return true;
	}

	if ( m_fileDescriptor == NULL )
	{
		m_fileDescriptor = fopen( ( m_owner->GetPackPath() / m_filename ).c_str(), "wb+" );

		if ( m_fileDescriptor == NULL )
		{
			EMUSE_MESSAGE_RELEASE( "MuseFilePack :: ReadContents - Error in opening file [" + m_owner->GetPackPath() + "][" + m_filename + "] : [" + String( strerror( errno ) ) + "]" );
			return false;
		}
	}

	size_t l_nbAvailable = p_waitingDatas.size() - p_indexBuffer;
	size_t l_nbMissing = m_fileSize - m_downloadedBytes;
	size_t l_min = min( l_nbAvailable, l_nbMissing );
	fwrite( & p_waitingDatas[p_indexBuffer], sizeof( char ), l_min, m_fileDescriptor );
	p_indexBuffer += l_min;

	if ( ( m_downloadedBytes + l_min ) == m_fileSize && m_fileDescriptor != NULL )
	{
		fflush( m_fileDescriptor );
		fclose( m_fileDescriptor );
		m_fileDescriptor = NULL;
	}

	m_downloadedBytes += l_min;
	m_owner->JustDownloaded( l_min );
	return true;
}

void MuseFilePack :: SetFinished()
{
	m_finished = true;

	if ( m_fileDescriptor != NULL )
	{
		fflush( m_fileDescriptor );
		fclose( m_fileDescriptor );
		m_fileDescriptor = NULL;
	}
}
