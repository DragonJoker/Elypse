/*
See LICENSE file in root folder
*/
#include "PrecompiledHeader.h"

#include "EMOverlayGroup.h"
#include "EMOverlay.h"
#include "EMGui.h"

#include <OgreOverlay.h>
#include <OgreOverlayManager.h>
#include <OgreOverlayElement.h>

using namespace Ogre;

EMOverlayGroup::EMOverlayGroup( String const & p_name, EMGui & p_owner )
	: named( p_name )
	, owned_by< EMGui >( p_owner )
	, m_overlay( NULL )
	, m_visible( true )
{
	m_overlay = OverlayManager::getSingletonPtr()->create( m_name );
	m_first = CreateElement( m_name );
}

EMOverlayGroup::~EMOverlayGroup()
{
	m_overlay->clear();

	if ( m_first != NULL )
	{
		delete m_first;
	}

	OverlayManager::getSingletonPtr()->destroy( m_overlay );
}

void EMOverlayGroup::ShowImmediate( bool p_visible )
{
	if ( p_visible && m_visible )
	{
		m_overlay->show();
	}
	else
	{
		m_overlay->hide();
	}
}

bool EMOverlayGroup::Contains( Real p_x, Real p_y )const
{
	if ( m_first != NULL )
	{
		return m_first->GetOgreOverlayElement()->contains( p_x, p_y );
	}

	return false;
}

void EMOverlayGroup::_finaliseAddElement( EMOverlay * p_element )
{
	m_overlay->add2D( static_cast <OverlayContainer *>( p_element->GetOgreOverlayElement() ) );

	if ( p_element->GetParent() != NULL && p_element->GetParent()->GetOgreOverlayElement() != NULL )
	{
		p_element->GetOgreOverlayElement()->_setParent( static_cast <OverlayContainer *>( p_element->GetParent()->GetOgreOverlayElement() ) );
	}
}

EMOverlay * EMOverlayGroup::CreateElement( String const & p_name )
{
	EMOverlay * l_overlay = General::Utils::map::insert( m_elements, p_name, p_name, *this );
	GetOwner()->AddElement( l_overlay );
	return l_overlay;
}

void EMOverlayGroup::_removeElement( String const & p_name )
{
	const EMOverlayMap::iterator & l_it = m_elements.find( p_name );

	if ( l_it == m_elements.end() )
	{
		return;
	}

	if ( m_overlay != NULL && l_it->second->GetOgreOverlayElement() != NULL )
	{
		m_overlay->remove2D( reinterpret_cast <OverlayContainer *>( l_it->second->GetOgreOverlayElement() ) );
	}

	m_elements.erase( l_it );
}

void EMOverlayGroup::DestroyElement( String const & p_name )
{
	const EMOverlayMap::iterator & l_it = m_elements.find( p_name );

	if ( l_it == m_elements.end() )
	{
		return;
	}

	if ( m_overlay != NULL && l_it->second->GetOgreOverlayElement() != NULL )
	{
		m_overlay->remove2D( reinterpret_cast <OverlayContainer *>( l_it->second->GetOgreOverlayElement() ) );
	}

	delete l_it->second;
	m_elements.erase( l_it );
}
