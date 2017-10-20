/*
See LICENSE file in root folder
*/
#include "PrecompiledHeader.h"

#include "EMOverlay.h"
#include "EMOverlayGroup.h"
#include "EMGui.h"
#include "ElypseLogs.h"

#include "ClickableShape.h"

#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OgreMaterialManager.h>
#include <OgreTextAreaOverlayElement.h>
#include <OgreFontManager.h>

#include "ScriptNode.h"

using namespace Ogre;

EMOverlay::EMOverlay( String const & p_name, EMOverlayGroup & p_owner )
	: named( p_name )
	, owned_by< EMOverlayGroup >( p_owner )
	, m_visible( true )
	, m_clickedScript( NULL )
	, m_overlay( NULL )
	, m_parent( NULL )
	, m_baseMaterialCopy( NULL )
	, m_textAlign( TextAreaOverlayElement::Left )
	, m_wordWrapping( false )
{
}

EMOverlay::~EMOverlay()
{
	GetOwner()->_removeElement( m_name );
	GetOwner()->GetOwner()->RemoveOverlay( m_name );

	if ( m_clickedScript != NULL )
	{
		m_clickedScript->Delete();
	}

	General::Utils::vector::deleteAll( m_clickables );

	if ( m_parent != NULL && m_overlay != NULL && m_parent->GetOgreOverlayElement() != NULL )
	{
		static_cast <OverlayContainer *>( m_parent->GetOgreOverlayElement() )->removeChild( m_overlay->getName() );
	}

	if ( m_baseMaterialCopy != NULL )
	{
		MaterialManager::getSingleton().remove( m_baseMaterialCopy->getName() );
	}

	General::Utils::vector::deleteAll( m_childs );

	if ( m_overlay )
	{
		OverlayManager::getSingletonPtr()->destroyOverlayElement( m_overlay, false );
	}
}

bool EMOverlay::Initialise( String const & p_className )
{
	if ( m_overlay != NULL )
	{
		return true;
	}

	try
	{
		if ( OverlayManager::getSingleton().getOverlayElement( p_className, true ) == NULL )
		{
			return false;
		}
	}
	catch ( Ogre::ItemIdentityException & p_exc )
	{
		EMUSE_MESSAGE_NORMAL( String( "Exception : " ) + p_exc.what() );
		return false;
	}

	m_overlay = OverlayManager::getSingletonPtr()->createOverlayElementFromTemplate( p_className, "", m_name /* + GetOwner()->GetOwner()->GetInstanceKey() */, false );
	/*
		if (m_overlay->getTypeName() == "TextArea")
		{
			m_overlay->setMetricsMode( GMM_PIXELS);
		}
		else
		{
	*/
	m_overlay->setMetricsMode( GMM_RELATIVE );
//	}
	m_baseMaterialName = m_overlay->getMaterialName();
	GetOwner()->_finaliseAddElement( this );

	if ( m_parent != NULL && m_parent->m_overlay != NULL )
	{
		static_cast <OverlayContainer *>( m_parent->m_overlay )->addChild( m_overlay );
	}

	return true;
}

void EMOverlay::Reinitialise( String const & p_className )
{
	if ( m_parent != NULL )
	{
		m_parent->RemoveChild( this );
		m_overlay->getParent()->removeChild( m_overlay->getName() );
	}

	OverlayManager::getSingletonPtr()->destroyOverlayElement( m_overlay, false );
	m_overlay = NULL;
	Initialise( p_className );

	for ( uint32_t i = 0 ; i < m_childs.size() ; i++ )
	{
		m_childs[i]->SetParent( this );
		reinterpret_cast <OverlayContainer *>( m_overlay )->addChild( m_childs[i]->GetOgreOverlayElement() );
	}
}

EMOverlay * EMOverlay::CreateChild( String const & p_name )
{
	EMOverlay * l_element = GetOwner()->CreateElement( p_name );
	l_element->SetParent( this );
	m_childs.push_back( l_element );
	return l_element;
}

void EMOverlay::DestroyChild( EMOverlay * p_child )
{
	if ( General::Utils::vector::has( m_childs, p_child ) )
	{
		General::Utils::vector::eraseValue( m_childs, p_child );
		delete p_child;
	}
}

void EMOverlay::SetBaseMaterial( String const & p_materialName )
{
	if ( m_overlay )
	{
		m_overlay->setMaterialName( p_materialName );
		m_baseMaterialName = p_materialName;
	}
}

Material * EMOverlay::GetOrCopyMaterial()
{
	if ( m_baseMaterialCopy == NULL )
	{
		MaterialPtr l_mat = MaterialManager::getSingletonPtr()->getByName( m_overlay->getMaterialName() );
		l_mat = l_mat->clone( m_overlay->getMaterialName() + "_copyfor_" + m_name );
		m_baseMaterialCopy = l_mat.getPointer();
		m_overlay->setMaterialName( m_baseMaterialCopy->getName() );
	}

	return m_baseMaterialCopy;
}

void EMOverlay::SetClickedScript( ScriptNode * p_script )
{
	if ( m_clickedScript != NULL )
	{
		m_clickedScript->Delete();
	}

	m_clickedScript = p_script;

	if ( m_clickedScript != NULL )
	{
		m_clickedScript->Use();
	}
}

/*
void EMOverlay::AddChild( EMOverlay * p_child)
{
	genlib_assert( p_child != NULL);

	m_childs.push_back( p_child);
}
*/

void EMOverlay::Reset()
{
	if ( m_overlay )
	{
		m_overlay->setMaterialName( m_baseMaterialName );
	}
}

void EMOverlay::MouseOver()
{
	if ( m_overlay )
	{
		m_overlay->setMaterialName( m_mouseOverMaterialName );
	}
}

void EMOverlay::Click()
{
	if ( m_overlay )
	{
		m_overlay->setMaterialName( m_clickedMaterialName );
	}
}

void EMOverlay::AddClickableShape( ClickableShape * p_shape )
{
	genlib_assert( p_shape != NULL );
	m_clickables.push_back( p_shape );
}

bool EMOverlay::TestMouseCoords( Real p_mouseX, Real p_mouseY )const
{
	if ( ! m_overlay->isVisible() )
	{
		return false;
	}

	if ( ! m_overlay->contains( p_mouseX, p_mouseY ) )
	{
		return false;
	}

	if ( m_clickables.empty() )
	{
		return true;
	}

	size_t imax = m_clickables.size();

	for ( size_t i = 0 ; i < imax ; i ++ )
	{
		if ( m_clickables[i]->TestClick( p_mouseX - m_overlay->_getDerivedLeft(), p_mouseY - m_overlay->_getDerivedTop() ) )
		{
			return true;
		}
	}

	return false;
}

EMOverlay * EMOverlay::GetAt( Real p_x, Real p_y )
{
	if ( m_overlay == NULL )
	{
		return NULL;
	}

	if ( ! m_overlay->isVisible() )
	{
		return NULL;
	}

	EMOverlay * l_overlay = NULL;
	EMOverlay * l_retOverlay = NULL;
	uint16_t l_zmax = 0;
	size_t imax = m_childs.size();

	for ( size_t i = 0 ; i < imax; ++ i )
	{
		l_overlay = m_childs[i]->GetAt( p_x, p_y );

		if ( l_overlay != NULL )
		{
			if ( l_overlay->GetOgreOverlayElement()->getZOrder() > l_zmax )
			{
				l_zmax = l_overlay->GetOgreOverlayElement()->getZOrder();
				l_retOverlay = l_overlay;
			}
		}
	}

	if ( l_retOverlay == NULL )
	{
		if ( TestMouseCoords( p_x, p_y ) )
		{
			l_retOverlay = this;
		}
	}

	return l_retOverlay;
}

EMOverlay * EMOverlay::RecursiveGetScript( Real p_x, Real p_y )
{
	if ( m_clickedScript != NULL )
	{
		return this;
	}

	if ( m_parent != NULL && m_parent->TestMouseCoords( p_x, p_y ) )
	{
		return m_parent->RecursiveGetScript( p_x, p_y );
	}

	return NULL;
}

EMOverlay * EMOverlay::RecursiveGetMouseOver( Real p_x, Real p_y )
{
	if ( ! m_mouseOverMaterialName.empty() )
	{
		return this;
	}

	if ( m_parent != NULL && m_parent->TestMouseCoords( p_x, p_y ) )
	{
		return m_parent->RecursiveGetMouseOver( p_x, p_y );
	}

	return NULL;
}

EMOverlay * EMOverlay::RecursiveGetClick( Real p_x, Real p_y )
{
	if ( ! m_clickedMaterialName.empty() )
	{
		return this;
	}

	if ( m_parent != NULL && m_parent->TestMouseCoords( p_x, p_y ) )
	{
		return m_parent->RecursiveGetClick( p_x, p_y );
	}

	return NULL;
}

Real EMOverlay::GetTop()const
{
	if ( !m_overlay )
	{
		return 0;
	}

	return m_overlay->getTop();
}

Real EMOverlay::GetLeft()const
{
	if ( !m_overlay )
	{
		return 0;
	}

	return m_overlay->getLeft();
}

Real EMOverlay::GetWidth()const
{
	if ( !m_overlay )
	{
		return 0;
	}

	return m_overlay->getWidth();
}

Real EMOverlay::GetHeight()const
{
	if ( !m_overlay )
	{
		return 0;
	}

	return m_overlay->getHeight();
}

void EMOverlay::SetTop( Real p_top )
{
	if ( m_overlay )
	{
		m_overlay->setTop( p_top );
	}
}

void EMOverlay::SetLeft( Real p_left )
{
	if ( m_overlay )
	{
		m_overlay->setLeft( p_left );
	}
}

void EMOverlay::SetWidth( Real p_width )
{
	if ( m_overlay )
	{
		m_overlay->setWidth( p_width );
	}
}

void EMOverlay::SetHeight( Real p_height )
{
	if ( m_overlay )
	{
		m_overlay->setHeight( p_height );
	}
}

void EMOverlay::SetCaption( String const & p_caption )
{
	if ( m_overlay )
	{
		static_cast <TextAreaOverlayElement *>( m_overlay )->setCaption( ToWString( p_caption ) );
	}

	_wrapWords();
}

void EMOverlay::SetAlignment( TextAreaOverlayElement::Alignment p_alignement )
{
	Real l_slide = 0.0;

	if ( m_textAlign == TextAreaOverlayElement::Right )
	{
		l_slide = -1.0;
	}
	else if ( m_textAlign == TextAreaOverlayElement::Center )
	{
		l_slide = -0.5;
	}

	if ( p_alignement == TextAreaOverlayElement::Right )
	{
		l_slide += 1.0;
	}
	else if ( p_alignement == TextAreaOverlayElement::Center )
	{
		l_slide += 0.5;
	}

	( static_cast <TextAreaOverlayElement *>( m_overlay ) )->setAlignment( p_alignement );
	m_overlay->setLeft( m_overlay->getLeft() + l_slide * m_overlay->getWidth() );
}

void EMOverlay::_wrapWords()
{
	genlib_assert( m_overlay != NULL );
	Real l_fontSize = ( static_cast <TextAreaOverlayElement *>( m_overlay ) )->getCharHeight();

	if ( m_overlay->getWidth() < 2 * l_fontSize )
	{
		return;
	}

	String l_caption = m_overlay->getCaption().asUTF8();
	size_t i = 0;
	FontPtr l_font = FontManager::getSingleton().getByName( ( static_cast <TextAreaOverlayElement *>( m_overlay ) )->getFontName() );
	genlib_assert( ! l_font.isNull() );
	Real l_total = 0.0;
	size_t l_lastPossibleIndex = l_caption.size();

	while ( i < l_caption.size() )
	{
		char l_char = l_caption[i];

		if ( l_char == ' ' || l_char == '\t' )
		{
			l_lastPossibleIndex = i;
		}

		l_total += GetOwner()->GetOwner()->GetWidth( l_caption[i], l_font, l_fontSize );

		if ( l_total >= m_overlay->getWidth() )
		{
			if ( l_lastPossibleIndex == l_caption.size() )
			{
				l_caption.insert( l_caption.begin() + i, '\n' );
			}
			else
			{
				l_caption[l_lastPossibleIndex] = '\n';
				i = l_lastPossibleIndex;
			}

			l_char = '\n';
		}

		if ( l_char == '|' )
		{
			l_char = '\n';
		}

		if ( l_char == '\n' )
		{
			l_total = 0.0;
			l_lastPossibleIndex = l_caption.size();
		}

		i++;
	}

	m_overlay->setCaption( l_caption );
}

void EMOverlay::SetVisible( bool p_visible )
{
	if ( m_overlay == NULL )
	{
		return;
	}

	if ( p_visible )
	{
		m_overlay->show();
	}
	else
	{
		m_overlay->hide();
	}

	General::Utils::vector::cycle( m_childs, &EMOverlay::SetVisible, p_visible );
}

void EMOverlay::_addChild( EMOverlay * p_child )
{
	p_child->SetParent( this );
	m_childs.push_back( p_child );
	GetOwner()->_finaliseAddElement( p_child );

	if ( m_overlay != NULL )
	{
		static_cast <OverlayContainer *>( m_overlay )->addChild( p_child->GetOgreOverlayElement() );
	}
}

void EMOverlay::ChangeParent( EMOverlay * p_parent )
{
	genlib_assert( p_parent != NULL );

	if ( GetOwner() != p_parent->GetOwner() || m_parent == nullptr )
	{
		GetOwner()->_removeElement( m_name );
	}

	if ( m_parent == NULL )
	{
		GetOwner()->GetOwner()->DestroyOverlayGroup( GetOwner()->GetName() );
	}

	p_parent->_addChild( this );
	ChangeOwner( *p_parent->GetOwner() );
	m_parent = p_parent;
}
