#include "PrecompiledHeader.h"

#include "WebImage.h"
#include "WebImageManager.h"
#include "MaterialTarget.h"

#include <OgreImage.h>
#include <OgreImageCodec.h>
#include <OgreMaterialManager.h>

#include <NeoCurl.h>
#include <FastMath.h>
#include <StringConverter.h>

#include "EMuseLogs.h"

using namespace Ogre;

WebImage :: WebImage( const String & p_name )
	:	named( p_name ),
		m_material( NULL ),
		m_texture( NULL ),
		m_ready( false ),
		m_loading( false )
{
	m_material = static_cast <Material *>( MaterialManager::getSingleton().create( m_name, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME ).getPointer() );
}

WebImage :: ~WebImage()
{
	while ( m_loading )
	{
		WebImageManager::GetSingletonPtr()->Unlock();
		WebImageManager::GetSingletonPtr()->Lock();
	}

	_delete();
	GENLIB_AUTO_SCOPED_LOCK();
	MaterialManager::getSingletonPtr()->remove( m_material->getName() );
	m_material = NULL;
	General::Utils::vector::deleteAll( m_targets );
}

void WebImage :: _delete()
{
	GENLIB_AUTO_SCOPED_LOCK();

	if ( m_texture != NULL )
	{
		TextureManager::getSingletonPtr()->remove( m_texture->getHandle() );
		m_texture = NULL;
	}
}

void WebImage :: Reload()
{
	GENLIB_AUTO_SCOPED_LOCK();

	if ( m_ready || m_texture == NULL )
	{
		m_ready = false;
		m_loading = true;
		WebImageManager::GetSingletonPtr()->ThreadedLoad( this );
	}
}

void WebImage :: Load()
{
	_setupImage();
}

void WebImage :: _setupImage()
{
	Image * l_image = new Image;
	CURLManager l_curl;
	{
		String l_contents;
		String l_ext;
		const String & l_type = l_curl.GetContentType( m_url );

		if ( l_type == "image/gif" )
		{
			l_ext = "gif";
		}
		else if ( l_type == "image/jpeg" )
		{
			l_ext = "jpg";
		}
		else if ( l_type == "image/png" )
		{
			l_ext = "png";
		}
		else
		{
			EMUSE_LOG_MESSAGE_RELEASE( "File type is not valid for WebImage named " + m_name + " : type : " + l_type );
			GENLIB_AUTO_SCOPED_LOCK();
			m_loading = false;
			delete l_image;
			return;
		}

		l_curl.GetStringFromUrl( m_url, l_contents );
		_loadFromMemory( l_ext, l_contents, l_image );
	}
	Pass * l_pass = m_material->getTechnique( 0 )->getPass( 0 );
	TextureUnitState * l_tex;

	if ( l_pass->getNumTextureUnitStates() == 0 )
	{
		l_tex = l_pass->createTextureUnitState( m_name );
		l_pass->setSceneBlending( Ogre::SBT_TRANSPARENT_ALPHA );
	}
	else
	{
		l_tex = l_pass->getTextureUnitState( 0 );
	}

	l_pass->setSceneBlending( Ogre::SBT_TRANSPARENT_ALPHA );
	WebImageManager::GetSingletonPtr()->Lock();
	_delete();
	m_texture = TextureManager::getSingleton().createManual(	m_name,
				ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
				Ogre::TEX_TYPE_2D,
				static_cast <unsigned int>( l_image->getWidth() ),
				static_cast <unsigned int>( l_image->getHeight() ),
				0,
				Ogre::PF_X8R8G8B8,
				Ogre::TU_DEFAULT ).getPointer();
	m_texture->loadImage( * l_image );
	l_tex->setTextureName( m_name );
	delete l_image;
	GENLIB_AUTO_SCOPED_LOCK();
	m_ready = true;
	m_loading = false;
	General::Utils::vector::cycle( m_targets, & MaterialTarget::Apply, m_name );
	General::Utils::vector::deleteAll( m_targets );
	WebImageManager::GetSingletonPtr()->Unlock();
}

void WebImage :: _loadFromMemory( const String & p_ext, const String & p_buffer, Image * p_image )
{
	Codec * l_codec = Codec::getCodec( p_ext );
	genlib_assert( l_codec != NULL );
	unsigned char * l_buffer = reinterpret_cast <unsigned char *>( CStrCopy( p_buffer ) );
	Ogre::DataStreamPtr l_stream( new MemoryDataStream( l_buffer, p_buffer.size() - 1, false ) );
	Codec::DecodeResult l_result = l_codec->decode( l_stream );
	ImageCodec::ImageData * l_data = static_cast <ImageCodec::ImageData *>( l_result.second.getPointer() );
	p_image->loadRawData( ( DataStreamPtr & )( l_result.first ), l_data->width, l_data->height, l_data->depth, l_data->format, 1, l_data->num_mipmaps );
	delete [] l_buffer;
}

void WebImage :: AddTarget( MaterialTarget * p_target )
{
	GENLIB_AUTO_SCOPED_LOCK();
	m_targets.push_back( p_target );
}