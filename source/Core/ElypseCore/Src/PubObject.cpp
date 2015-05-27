#include "PrecompiledHeader.h"

#include "PubManager.h"
#include "PubObject.h"
#include "PubInstance.h"

#include "WebImageManager.h"
#include "WebImage.h"

#include "EMusePlugin.h"

#include <OgreEntity.h>
#include <OgreStringConverter.h>
#include <NeoCurl.h>
#include <StringUtils.h>

PubObject :: PubObject(	const String & p_name, EMusePlugin * p_plugin )
	:	named( p_name ),
		m_plugin( p_plugin ),
		m_image( NULL )
{
	genlib_assert( m_plugin != NULL );
	time_t abstime;
	time( & abstime );
	m_timestamp = StringConverter::toString( static_cast <unsigned int>( abstime ) );
}

PubObject :: ~PubObject()
{
	if ( m_image != NULL )
	{
		WebImageManager::GetSingletonPtr()->DestroyImage( m_image->GetName() );
	}

	General::Utils::map::deleteAll( m_instances );
}

void PubObject :: SetLinkUrl( const String & p_url )
{
	m_linkURL = string::replace( p_url, " ", m_timestamp.c_str() );
}

void PubObject :: SetUrl( const String & p_url )
{
	m_imageURL = string::replace( p_url, " ", m_timestamp.c_str() );
	_retrieveUrl();
}

void PubObject :: SetImageUrl( const String & p_url )
{
	m_imageURL = string::replace( p_url, " ", m_timestamp.c_str() );
}

void PubObject :: RemoveInstance( PubInstance * p_instance )
{
	General::Utils::map::deleteValue( m_instances, p_instance->GetEntity()->getName() );
}

void PubObject :: _retrieveUrl()
{
	CURLManager * l_curl = WebImageManager::GetSingletonPtr()->GetCurl();
	String l_contents;
	l_curl->GetStringFromUrl( m_imageURL, l_contents );
	size_t l_index = l_contents.find( "href=\"" );

	if ( l_index != String::npos )
	{
		String l_temp = l_contents.substr( l_index );
		size_t l_index2 = l_temp.find( "\"" );
		m_linkURL = l_temp.substr( 0, l_index2 );
	}

	l_index = l_contents.find( "src=\"" );

	if ( l_index != String::npos )
	{
		String l_temp = l_contents.substr( l_index );
		size_t l_index2 = l_temp.find( "\"" );
		m_imageURL = l_temp.substr( 0, l_index2 );
	}
}

void PubObject :: Finalise()
{
	if ( m_linkURL.empty() )
	{
		_retrieveUrl();
	}

	if ( m_image == NULL )
	{
		m_image = WebImageManager::GetSingletonPtr()->Create( m_name );
		m_image->SetUrl( m_imageURL );
		m_image->Load();
	}

	General::Utils::map::cycle( m_instances, & PubInstance::Apply );
}

PubInstance * PubObject :: CreateInstance( Entity * p_ent )
{
	genlib_assert( p_ent != NULL );
	PubInstance * l_instance = General::Utils::map::insert( m_instances, p_ent->getName(), this, p_ent );
	return l_instance;
}

void PubObject :: OnClick()
{
	if ( ! m_clicked )
	{
		m_clicked = true;
		m_timer.Time();
		return;
	}

	if ( m_timer.Time() > 0.3 )
	{
		return;
	}

	m_clicked = false;
	m_plugin->OpenURL( m_linkURL, true, false );
}

void PubObject :: Reload()
{
	m_image->Reload( /*& PubObject::_reloadCallback, this*/ );
}
