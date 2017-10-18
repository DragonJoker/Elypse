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

#include "DownloadManager.h"

#include "MuseFile.h"
#include "MuseDownloader.h"
#include "ElypseLoadingBar.h"

DownloadManager::DownloadManager( const Path & p_installPath, ElypseLoadingBar * p_bar, bool p_download )
	: m_loadingBar( p_bar )
	, m_installPath( p_installPath )
	, m_downloadFiles( p_download )
{
}

DownloadManager::~DownloadManager()
{
	General::Utils::map::deleteAll( m_museFiles );
}

MuseFile * DownloadManager::StartDownloadFile( Url const & p_url, bool p_download )
{
	genlib_assert( ! p_url.empty() );
	Url l_url = p_url;
	l_url.GetProtocol();
	String const & l_filename = l_url.GetFilename();
	genlib_assert( ! l_filename.empty() );
	const MuseFileMap::const_iterator & l_find = m_museFiles.find( l_filename );

	if ( l_find == m_museFiles.end() )
	{
		MuseFile * l_file = new MuseFile( l_filename );
		l_file->GetDownloader().SetInstallPath( m_installPath );
		l_file->GetDownloader().SetDownloadFiles( m_downloadFiles && p_download );
		l_file->GetDownloader().SetURL( l_url );

		if ( m_downloadFiles && p_download )
		{
			l_file->GetDownloader().StartDownload();
		}

		m_museFiles.insert( std::make_pair( l_filename, l_file ) );
		return l_file;
	}

	return l_find->second;
}
