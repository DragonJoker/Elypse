#include "PrecompiledHeader.h"

#include "WebImageManager.h"
#include "WebImage.h"

#include <NeoCurl.h>
#include <ThreadId.h>

#include <OgreStringConverter.h>

#include "EMuseLogs.h"

GENLIB_INIT_SINGLETON( WebImageManager );

WebImageManager :: WebImageManager()
	:	m_threadPool( 2, 500 )
{
	m_curl = new CURLManager;
	Lock();
}

WebImageManager :: ~WebImageManager()
{
	Unlock();
	delete m_curl;
	General::Utils::map::deleteAll( m_images );
}

WebImage * WebImageManager :: Create( const String & p_name )
{
	return General::Utils::map::insert( m_images, p_name, p_name );
}

bool WebImageManager :: DestroyImage( const String & p_name )
{
	return General::Utils::map::deleteValue( m_images, p_name );
}

void WebImageManager :: Unlock()
{
	GENLIB_UNLOCK_MUTEX( m_mutex );
}

void WebImageManager :: Lock()
{
	GENLIB_LOCK_MUTEX( m_mutex );
}

void WebImageManager :: ThreadedLoad( WebImage * p_image )
{
	m_threadPool.AddJob( GENLIB_THREAD_CLASS_FUNCTOR( p_image, WebImage, Load ) );
}
