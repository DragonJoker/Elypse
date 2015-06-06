#ifndef ___MUSE_PACK_H___
#define ___MUSE_PACK_H___

#include "Module_Download.h"

#include <PreciseTimer.h>

namespace EMuse
{
	namespace Download
	{
		class d_dll_export MusePack : public owned_by<MuseDownloader>, d_noncopyable
		{
		private:
			DataArray m_waitingDatas;
			MuseFileArray m_files;
			PreciseTimer * m_timer;
			Path m_installPath;
			Path m_basePath;
			Path m_packPath;
			bool m_museOK;
			bool m_checkHeader;
			unsigned int m_nbHeader;
			unsigned int m_museFileSize;
			size_t m_totalByteDownloaded;
			long long m_beginTime;

		public:
			MusePack( MuseDownloader * p_owner, const Path & p_installPath, const Path & p_basePath, bool p_verifyHeader );
			~MusePack();

		private:
			bool _verifyFiles();
			bool _downloadFinished();
			void _finishMuseFile();
			bool _treatGZipFile( const Path & p_fullFilePath, const String & p_fileName, const String & p_folderName );

			bool _getHeaders( size_t & p_indexBuffer );

			bool _finishHeaders();
			bool _getNumFiles( size_t & p_indexBuffer );

		public:
			inline bool IsOK()const
			{
				return m_museOK;
			}
			inline Real GetPercent()const
			{
				return ( static_cast <Real>( m_totalByteDownloaded ) / static_cast <Real>( m_museFileSize ) );
			}
			inline unsigned int GetNbBlocks()const
			{
				return m_nbHeader;
			}
			inline Real GetDownloadSpeed()const
			{
				m_timer->Time();
				Real m_timeElapsed = Real( 1024.0 ) * static_cast <Real>( m_timer->TimeDiff( m_beginTime ) );
				return ( static_cast <Real>( m_totalByteDownloaded ) / ( m_timeElapsed ) );
			}

			inline void AddFileSize( unsigned int p_size )
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

		public:
			inline static size_t StCurlCallback( void * p_buffer, size_t p_size, size_t p_nmemb, void * p_userp )
			{
				return reinterpret_cast <MusePack *>( p_userp )->CurlCallback( p_buffer, p_size * p_nmemb );
			}
			static std::string DataType2String( DataBlockType p_dataType );

		public:
			size_t CurlCallback( void * p_buffer, size_t p_size );
			const String & GetFileName( size_t p_index )const;
			size_t GetFileIndex( const String & p_name )const;
			bool IsDownloadFinished( unsigned int p_index )const;
			friend std::ostream & operator <<( std::ostream & p_out, const MusePack & p_file );

			bool DecompressFile( const String & p_zipFileName, const String & p_mp3FileName, int p_size );
		};
	}
}

#endif
