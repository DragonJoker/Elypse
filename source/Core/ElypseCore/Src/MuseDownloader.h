/*
See LICENSE file in root folder
*/
#ifndef ___CMuseDownloader_H___
#define ___CMuseDownloader_H___

#include "Module_Download.h"
#include "Module_Data.h"

#include <thread>

#include <NeoCurl.h>

namespace Elypse
{
	namespace Download
	{
		class d_dll_export MuseDownloader
			: public named
			, public owned_by< MuseFile >
			, d_noncopyable
		{
		public:
			MuseDownloader( MuseFile & p_owner, const Path & p_basePath );
			~MuseDownloader();

			void StartDownload();
			void Execute();
			Real GetPercent()const;
			Real GetDownloadSpeed()const;
			uint32_t GetNbBlocks()const;
			bool IsDownloadFinished( uint32_t p_index )const;
			String const & GetFileName( uint32_t p_index )const;
			String GetExtension( String const & p_filename );
			String GetFolderName( String const & p_fileName );
			DataBlockType GetFileType( String const & p_filename );
			void SetURL( String const & p_url );
			size_t GetFileIndex( String const & p_name )const;

			inline bool CantDownload()const
			{
				return m_cantDownload;
			}
			inline bool IsInitialised()const
			{
				return m_initialised || m_cantDownload;
			}
			inline bool DownloadFiles()const
			{
				return m_downloadFiles;
			}
			inline const Path & GetPath()const
			{
				return m_basePath;
			}
			inline const Path & GetInstallPath()const
			{
				return m_installPath;
			}
			inline void SetCantDownload( bool p_cantDownload )
			{
				m_cantDownload = p_cantDownload;
			}
			inline void SetInitialised( bool p_initialised )
			{
				m_initialised = p_initialised;
			}
			inline void SetInstallPath( const General::Utils::Path & p_path )
			{
				m_installPath = p_path;
			}
			inline void SetDownloadFiles( bool p_download )
			{
				m_downloadFiles = p_download;
			}

		private:
			bool m_downloadFiles;
			bool m_cantDownload;
			bool m_initialised;
			bool m_stopThread;
			bool m_verifyHeader;

			std::thread * m_thread;
			MusePack * m_musePack;
			General::Files::CURLManager m_curlManager;

			String m_url;
			General::Utils::Path m_basePath;
			General::Utils::Path m_installPath;
		};
	}
}

#endif
