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
#ifndef ___MUSE_PACK_H___
#define ___MUSE_PACK_H___

#include "Module_Download.h"

#include <PreciseTimer.h>

namespace Elypse
{
	namespace Download
	{
		class d_dll_export MusePack
			: public owned_by< MuseDownloader >
			, d_noncopyable
		{
		public:
			MusePack( MuseDownloader & p_owner, const Path & p_installPath, const Path & p_basePath, bool p_verifyHeader );
			~MusePack();

			inline bool IsOK()const
			{
				return m_museOK;
			}
			inline Real GetPercent()const
			{
				return ( Real( m_totalByteDownloaded ) / Real( m_museFileSize ) );
			}
			inline uint32_t GetNbBlocks()const
			{
				return m_nbHeader;
			}
			inline Real GetDownloadSpeed()const
			{
				m_timer->Time();
				Real m_timeElapsed = Real( 1024.0 ) * Real( m_timer->TimeDiff( m_beginTime ) );
				return ( Real( m_totalByteDownloaded ) / ( m_timeElapsed ) );
			}
			inline void AddFileSize( uint32_t p_size )
			{
				m_museFileSize += p_size;
			}
			inline const Path & GetPackPath()const
			{
				return m_packPath;
			}
			inline void JustDownloaded( size_t p_num )
			{
				m_totalByteDownloaded += p_num;
			}

			inline static size_t StCurlCallback( void * p_buffer, size_t p_size, size_t p_nmemb, void * p_userp )
			{
				return reinterpret_cast <MusePack *>( p_userp )->CurlCallback( p_buffer, p_size * p_nmemb );
			}

			static std::string DataType2String( DataBlockType p_dataType );

			size_t CurlCallback( void * p_buffer, size_t p_size );
			String const & GetFileName( size_t p_index )const;
			size_t GetFileIndex( String const & p_name )const;
			bool IsDownloadFinished( uint32_t p_index )const;
			friend std::ostream & operator<<( std::ostream & p_out, const MusePack & p_file );

			bool DecompressFile( String const & p_zipFileName, String const & p_mp3FileName, int p_size );

		private:
			bool _verifyFiles();
			bool _downloadFinished();
			void _finishMuseFile();
			bool _treatGZipFile( const Path & p_fullFilePath, String const & p_fileName, String const & p_folderName );

			bool _getHeaders( size_t & p_indexBuffer );

			bool _finishHeaders();
			bool _getNumFiles( size_t & p_indexBuffer );

		private:
			DataArray m_waitingDatas;
			MuseFileArray m_files;
			PreciseTimer * m_timer;
			Path m_installPath;
			Path m_basePath;
			Path m_packPath;
			bool m_museOK;
			bool m_checkHeader;
			uint32_t m_nbHeader;
			uint32_t m_museFileSize;
			size_t m_totalByteDownloaded;
			std::chrono::high_resolution_clock::time_point m_beginTime;
		};
	}
}

#endif
