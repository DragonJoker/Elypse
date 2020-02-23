/*
See LICENSE file in root folder
*/
#include "PrecompiledHeader.h"

#include "WebImageManager.h"
#include "WebImage.h"

#include <NeoCurl.h>

#include <OgreStringConverter.h>

#include "ElypseLogs.h"

GENLIB_INIT_SINGLETON( WebImageManager );

WebImageManager::WebImageManager()
	: m_threadPool( 2, 500 )
{
	m_curl = new CURLManager;
	Lock();
}

WebImageManager::~WebImageManager()
{
	Unlock();
	delete m_curl;
	General::Utils::map::deleteAll( m_images );
}

WebImage * WebImageManager::Create( String const & p_name )
{
	return General::Utils::map::insert( m_images, p_name, p_name );
}

bool WebImageManager::DestroyImage( String const & p_name )
{
	return General::Utils::map::deleteValue( m_images, p_name );
}

void WebImageManager::Unlock()
{
	m_mutex.unlock();
}

void WebImageManager::Lock()
{
	m_mutex.lock();
}

void WebImageManager::ThreadedLoad( WebImage * p_image )
{
	m_threadPool.AddJob( [p_image]()
	{
		p_image->Load();
	} );
}
