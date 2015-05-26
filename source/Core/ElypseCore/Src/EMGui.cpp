#include "PrecompiledHeader.h"

#include "EMGui.h"
#include "EMOverlay.h"
#include "EMOverlayGroup.h"

#include "ScriptNode.h"

#include <OgreOverlay.h>
#include <OgreOverlayElement.h>
#include <OgreFontManager.h>

EMGui :: EMGui( const String & p_instanceKey, Real p_aspect )
	:	m_currentClickedOverlay( NULL ),
		m_currentMouseOverOverlay( NULL ),
		m_topmostOverlay( NULL ),
		m_instanceKey( p_instanceKey ),
		m_aspectRatio( p_aspect )
{
}

EMGui :: ~EMGui()
{
	ClearAll();
}

void EMGui :: ClearAll() d_no_throw
{
	m_currentMouseOverOverlay = NULL;
	m_currentClickedOverlay = NULL;
	m_topmostOverlay = NULL;

	General::Utils::map::deleteAll( m_groups );

	m_overlays.clear();
}
/*
EMOverlayGroup * EMGui :: AddOverlay( Overlay * p_overlay)
{
	genlib_assert( p_overlay != NULL);

	p_overlay->setZOrder( static_cast <Ogre::ushort> (99 + m_groups.size() ));

	return General::Utils::map::insert( m_groups, p_overlay->getName(), p_overlay, true);
}
*/

EMOverlayGroup * EMGui :: CreateOverlayGroup( const String & p_name )
{
	return General::Utils::map::insert( m_groups, p_name, p_name, this );
}


ScriptNode * EMGui :: Click( Real p_x, Real p_y )
{
	if ( m_topmostOverlay != NULL )
	{
		EMOverlay * l_script = m_topmostOverlay->RecursiveGetScript( p_x, p_y );

		if ( l_script != NULL )
		{
			return l_script->GetScript();
		}
	}

	return NULL;
}

EMOverlay * EMGui :: GetTopmostAt( Real p_x, Real p_y )const
{
	EMOverlay * l_topmost = NULL;
	unsigned short l_minZorder = 0;
	OverlayGroupMap::const_iterator iter = m_groups.begin();
	const OverlayGroupMap::const_iterator & iend = m_groups.end();

	for ( ; iter != iend ; ++ iter )
	{
		EMOverlayGroup * l_group = iter->second;

		if ( l_group->IsVisible() )
		{
			EMOverlay * l_tempOverlay = l_group->GetFirstElement()->GetAt( p_x, p_y );

			if ( l_tempOverlay != NULL )
			{
				if ( l_tempOverlay->GetOgreOverlayElement()->getZOrder() > l_minZorder )
				{
					l_minZorder = l_tempOverlay->GetOgreOverlayElement()->getZOrder();
					l_topmost = l_tempOverlay;
				}
			}
		}
	}

	return l_topmost;
}

void EMGui :: MouseOver( Real p_x, Real p_y )
{
	if ( m_currentMouseOverOverlay != NULL )
	{
		m_currentMouseOverOverlay->Reset();
		m_currentMouseOverOverlay = NULL;
	}

	EMOverlay * l_topmost = GetTopmostAt( p_x, p_y );

	if ( l_topmost != NULL )
	{
		m_topmostOverlay = l_topmost;
		m_currentClickedOverlay = l_topmost->RecursiveGetClick( p_x, p_y );
		m_currentMouseOverOverlay = l_topmost->RecursiveGetMouseOver( p_x, p_y );

		if ( m_currentMouseOverOverlay != NULL )
		{
			m_currentMouseOverOverlay->MouseOver();
		}
	}
	else
	{
		m_topmostOverlay = NULL;
		m_currentClickedOverlay = NULL;
		m_currentMouseOverOverlay = NULL;
	}
}

void EMGui :: SetAllOverlaysVisibility( bool p_show )const
{
	General::Utils::map::cycle( m_groups, & EMOverlayGroup::ShowImmediate, p_show );
}

void EMGui :: AddElement( EMOverlay * p_element )
{
	genlib_assert( p_element != NULL );
	m_overlays.insert( EMOverlayMap :: value_type( p_element->GetName(), p_element ) );
}

void EMGui :: RemoveOverlay( const String & p_overlayName )
{
	if ( p_overlayName.empty() )
	{
		return;
	}

	EMOverlayMap::iterator l_it = m_overlays.find( p_overlayName );

	if ( l_it != m_overlays.end() )
	{
		m_overlays.erase( l_it );
	}
}

void EMGui :: DestroyEMOverlay( const String & p_name )
{
	EMOverlayMap::iterator l_it = m_overlays.find( p_name );

	if ( l_it != m_overlays.end() )
	{
		if ( l_it->second->GetOwner() != NULL )
		{
			l_it->second->GetOwner()->DestroyElement( p_name );
		}

		m_overlays.erase( l_it );
	}
}

void EMGui :: DestroyOverlayGroup( const String & p_name )
{
	General::Utils::map::deleteValue( m_groups, p_name );
}


Real EMGui :: GetWidth( char p_char, const FontPtr & p_font, Real p_fontSize )const
{
	if ( p_font.isNull() )
	{
		return 0.0;
	}

	if ( p_char == ' ' )
	{
		p_char = 'A';
	}

	return p_font->getGlyphAspectRatio( p_char ) * p_fontSize / m_aspectRatio;
}

Real EMGui :: GetWidth( const String & p_string, const FontPtr & p_font, Real p_fontSize )const
{
	if ( p_font.isNull() )
	{
		return 0.0;
	}

	Real l_width = 0.0;

	for ( size_t i = 0 ; i < p_string.size() ; i ++ )
	{
		l_width += GetWidth( p_string[i], p_font, p_fontSize );
	}

	return l_width;
}
