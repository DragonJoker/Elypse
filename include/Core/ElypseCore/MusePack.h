/*
See LICENSE file in root folder
*/
#ifndef ___MUSE_PACK_H___
#define ___MUSE_PACK_H___

#include "Module_Download.h"

#include <PreciseTimer.h>

namespace Elypse::Download
{
	class d_dll_export MusePack
		: public owned_by< MuseDownloader >
		, d_noncopyable
	{
	public:
		MusePack( MuseDownloader & p_owner, const Path & p_installPath, const Path & p_basePath, bool p_verifyHeader );

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
			return reinterpret_cast< MusePack * >( p_userp )->CurlCallback( p_buffer, p_size * p_nmemb );
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
		std::unique_ptr< PreciseTimer > m_timer;
		Path m_installPath;
		Path m_basePath;
		Path m_packPath;
		bool m_museOK{ false };
		bool m_checkHeader{ true };
		uint32_t m_nbHeader{ 0u };
		uint32_t m_museFileSize{ 0u };
		size_t m_headerSize{ 0u };
		size_t m_totalByteDownloaded{ 0u };
		std::chrono::high_resolution_clock::time_point m_beginTime;
	};
}

#endif
