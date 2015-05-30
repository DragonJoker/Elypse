#include "PrecompiledHeader.h"

#include "DownloadManager.h"

#include "MuseFile.h"
#include "MuseDownloader.h"
#include "EMuseLoadingBar.h"

DownloadManager::DownloadManager( const Path & p_installPath, EMuseLoadingBar * p_bar, bool p_download )
	:	m_loadingBar( p_bar ),
		m_installPath( p_installPath ),
		m_downloadFiles( p_download )
{
}

DownloadManager::~DownloadManager()
{
	General::Utils::map::deleteAll( m_museFiles );
}

MuseFile * DownloadManager::StartDownloadFile( const Url & p_url, bool p_download )
{
	genlib_assert( ! p_url.empty() );
	Url l_url = p_url;
	l_url.GetProtocol();
	const String & l_filename = l_url.GetFilename();
	genlib_assert( ! l_filename.empty() );
	const MuseFileMap::const_iterator & l_find = m_museFiles.find( l_filename );

	if ( l_find == m_museFiles.end() )
	{
		MuseFile * l_file = new MuseFile( l_filename );
		l_file->GetDownloader()->SetInstallPath( m_installPath );
		l_file->GetDownloader()->SetDownloadFiles( m_downloadFiles && p_download );
		l_file->GetDownloader()->SetURL( l_url );

		if ( m_downloadFiles && p_download )
		{
			l_file->GetDownloader()->StartDownload();
		}

		m_museFiles.insert( MuseFileMap::value_type( l_filename, l_file ) );
		return l_file;
	}

	return l_find->second;
}
