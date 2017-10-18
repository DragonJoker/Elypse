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

#include "PubManager.h"
#include "PubObject.h"
#include "PubInstance.h"

#include "WebImageManager.h"
#include "WebImage.h"

#include "ElypsePlugin.h"

#include <OgreEntity.h>
#include <OgreStringConverter.h>
#include <NeoCurl.h>
#include <StringUtils.h>

PubObject::PubObject( String const & p_name, ElypsePlugin * p_plugin )
	: named( p_name )
	, m_plugin( p_plugin )
	, m_image( NULL )
{
	genlib_assert( m_plugin != NULL );
	time_t abstime;
	time( & abstime );
	m_timestamp = StringConverter::toString( static_cast <uint32_t>( abstime ) );
}

PubObject::~PubObject()
{
	if ( m_image != NULL )
	{
		WebImageManager::GetSingletonPtr()->DestroyImage( m_image->GetName() );
	}

	General::Utils::map::deleteAll( m_instances );
}

void PubObject::SetLinkUrl( String const & p_url )
{
	m_linkURL = string::replace( p_url, " ", m_timestamp.c_str() );
}

void PubObject::SetUrl( String const & p_url )
{
	m_imageURL = string::replace( p_url, " ", m_timestamp.c_str() );
	_retrieveUrl();
}

void PubObject::SetImageUrl( String const & p_url )
{
	m_imageURL = string::replace( p_url, " ", m_timestamp.c_str() );
}

void PubObject::RemoveInstance( PubInstance * p_instance )
{
	General::Utils::map::deleteValue( m_instances, p_instance->GetEntity()->getName() );
}

void PubObject::_retrieveUrl()
{
	CURLManager * l_curl = WebImageManager::GetSingletonPtr()->GetCurl();
	String l_contents;
	l_curl->GetStringFromUrl( m_imageURL, l_contents );
	size_t l_index = l_contents.find( "href = \"" );

	if ( l_index != String::npos )
	{
		String l_temp = l_contents.substr( l_index );
		size_t l_index2 = l_temp.find( "\"" );
		m_linkURL = l_temp.substr( 0, l_index2 );
	}

	l_index = l_contents.find( "src = \"" );

	if ( l_index != String::npos )
	{
		String l_temp = l_contents.substr( l_index );
		size_t l_index2 = l_temp.find( "\"" );
		m_imageURL = l_temp.substr( 0, l_index2 );
	}
}

void PubObject::Finalise()
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

PubInstance * PubObject::CreateInstance( Entity * p_ent )
{
	genlib_assert( p_ent != NULL );
	PubInstance * l_instance = General::Utils::map::insert( m_instances, p_ent->getName(), *this, p_ent );
	return l_instance;
}

void PubObject::OnClick()
{
	if ( !m_clicked )
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

void PubObject::Reload()
{
	m_image->Reload( /*& PubObject::_reloadCallback, this*/ );
}
