
#include "PrecompiledHeader.h"

#include "Overlay.h"
#include "Project/Object.h"
#include "Project/Media/VideoObject.h"
#include "GUI/ObjectProperties/OverlayProperties.h"
#include "GUI/MainFrame.h"

using namespace Troll;
using namespace Troll::Objects2D;
using namespace Troll::GUI;


TrollOverlay::TrollOverlay( const wxString & p_name, TrollOverlay * p_parent, const wxString & p_fileName )
	:	TrollObject( p_name, p_fileName, ttOverlay ),
		m_parent( p_parent ),
		m_verticalAlign( vNone ),
		m_horizontalAlign( hNone ),
		m_top( 0.0 ),
		m_left( 0.0 ),
		m_width( 0.0 ),
		m_height( 0.0 ),
		m_borderInfos( NULL ),
		m_video( NULL ),
		m_depth( 0 ),
		m_inFileName( p_name ),
		m_textInfos( NULL ),
		m_museOverlay( NULL )
{
	if ( m_parent != NULL )
	{
		m_depth = m_parent->m_depth + 1;
		m_parent->AddChild( this, false );
	}
}



TrollOverlay::~TrollOverlay()
{
	m_propertiesPanel = NULL;

	if ( m_textInfos != NULL )
	{
		delete m_textInfos;
		m_textInfos = NULL;
	}

	if ( m_borderInfos != NULL )
	{
		delete m_borderInfos;
		m_borderInfos = NULL;
	}
}



void TrollOverlay::Reinitialise( const wxString & p_templateName )
{
	m_museOverlay->Reinitialise( p_templateName.char_str().data() );
	wxString l_templateName = p_templateName;
	l_templateName.MakeLower();

	if ( l_templateName.find( wxT( "border" ) ) == wxString::npos && m_borderInfos != NULL )
	{
		delete m_borderInfos;
		m_borderInfos = NULL;
	}

	if ( l_templateName.find( wxT( "Text" ) ) == wxString::npos && m_textInfos != NULL )
	{
		delete m_textInfos;
		m_textInfos = NULL;
	}
}



void TrollOverlay::AddChild( TrollOverlay * p_child, bool p_updateElypse )
{
	if ( m_childs.find( p_child->GetName() ) == m_childs.end() )
	{
		m_childs.insert( TrollOverlayMap::value_type( p_child->GetName(), p_child ) );
		p_child->m_parent = this;
	}
}



void TrollOverlay::RemoveChild( const wxString & p_name )
{
	TrollOverlayMap::iterator l_it = m_childs.find( p_name );

	if ( l_it != m_childs.end() )
	{
		l_it->second->m_parent = NULL;
		m_childs.erase( l_it );
	}
}



int TrollOverlay::BuildPanel( wxWindow * p_parent, int p_width )
{
	int l_height = 240 + ( m_textInfos != NULL ? 180 : 0 ) + ( m_borderInfos != NULL ? 100 : 0 );
	wxSize l_returnSize( p_width, l_height );

	if ( m_propertiesPanel != NULL )
	{
		m_propertiesPanel->GetParent()->RemoveChild( m_propertiesPanel );
		delete m_propertiesPanel;
		m_propertiesPanel = NULL;
	}

	m_propertiesPanel = new OverlayProperties( this, p_parent, wxPoint( 0, 0 ), l_returnSize );
	return l_height;
}



void TrollOverlay::SetMuseOverlay( EMuse::Gui::EMOverlay * p_museOverlay )
{
	m_museOverlay = p_museOverlay;
	m_baseMaterial = make_wxString( m_museOverlay->GetBaseMaterial() );
	m_mouseOverMaterial = make_wxString( m_museOverlay->GetMouseOverMaterial() );
	m_clickedMaterial = make_wxString( m_museOverlay->GetClickedMaterial() );
	m_top = m_museOverlay->GetTop();
	m_left = m_museOverlay->GetLeft();
	m_width = m_museOverlay->GetWidth();
	m_height = m_museOverlay->GetHeight();
	m_horizontalAlign = static_cast <TEHorizAlign>( m_museOverlay->GetOgreOverlayElement()->getHorizontalAlignment() );
	m_verticalAlign = static_cast <TEVerticAlign>( m_museOverlay->GetOgreOverlayElement()->getVerticalAlignment() );

	if ( m_museOverlay->GetOgreOverlayElement()->getTypeName() == "TextArea" )
	{
		Ogre::TextAreaOverlayElement * l_overlayElement = static_cast <Ogre::TextAreaOverlayElement *>( m_museOverlay->GetOgreOverlayElement() );
		m_textInfos = new TextInfos;
		wxString l_caption = make_wxString( l_overlayElement->getCaption().asUTF8() );
		l_caption.Replace( wxT( "\n" ), wxT( " " ) );
		m_textInfos->m_caption = l_caption;
		m_textInfos->m_fontName = make_wxString( l_overlayElement->getFontName() );
		m_textInfos->m_fontSize = l_overlayElement->getCharHeight();
		m_textInfos->m_textColour = l_overlayElement->getColour();
		m_textInfos->m_horizontalAlign = hNone;
		m_textInfos->m_verticalAlign = vNone;
	}
	else if ( m_museOverlay->GetOgreOverlayElement()->getTypeName() == "BorderPanel" )
	{
		Ogre::BorderPanelOverlayElement * l_overlayElement = static_cast <Ogre::BorderPanelOverlayElement *>( m_museOverlay->GetOgreOverlayElement() );

		if ( m_borderInfos == NULL )
		{
			m_borderInfos = new BorderInfos;
		}

		m_borderInfos->m_top = l_overlayElement->getTopBorderSize();
		m_borderInfos->m_bottom = l_overlayElement->getBottomBorderSize();
		m_borderInfos->m_left = l_overlayElement->getLeftBorderSize();
		m_borderInfos->m_right = l_overlayElement->getRightBorderSize();
		m_borderInfos->m_material = make_wxString( l_overlayElement->getBorderMaterialName() );
	}
}



void TrollOverlay::SetOverlayClass( const wxString & p_className )
{
	m_className = p_className;
}



void TrollOverlay::SetBaseMaterial( const wxString & p_matName, bool p_updateOgre )
{
	m_baseMaterial = p_matName;

	if ( p_updateOgre )
	{
		m_museOverlay->SetBaseMaterial( p_matName.char_str().data() );
	}
	else if ( m_propertiesPanel != NULL )
	{
		m_propertiesPanel->UpdateProperties();
	}
}



void TrollOverlay::SetMouseOverMaterial( const wxString & p_matName, bool p_updateOgre )
{
	m_mouseOverMaterial = p_matName;

	if ( p_updateOgre )
	{
		m_museOverlay->SetMouseOverMaterial( p_matName.char_str().data() );
	}
	else if ( m_propertiesPanel != NULL )
	{
		m_propertiesPanel->UpdateProperties();
	}
}



void TrollOverlay::SetClickedMaterial( const wxString & p_matName, bool p_updateOgre )
{
	m_clickedMaterial = p_matName;

	if ( p_updateOgre )
	{
		m_museOverlay->SetClickedMaterial( p_matName.char_str().data() );
	}
	else if ( m_propertiesPanel != NULL )
	{
		m_propertiesPanel->UpdateProperties();
	}
}



void TrollOverlay::SetTop( float p_top, bool p_updateOgre )
{
	m_top = p_top;

	if ( p_updateOgre )
	{
		m_museOverlay->SetTop( p_top );
	}
	else if ( m_propertiesPanel != NULL )
	{
		m_propertiesPanel->UpdateProperties();
	}
}



void TrollOverlay::SetLeft( float p_left, bool p_updateOgre )
{
	m_left = p_left;

	if ( p_updateOgre )
	{
		m_museOverlay->SetLeft( p_left );
	}
	else if ( m_propertiesPanel != NULL )
	{
		m_propertiesPanel->UpdateProperties();
	}
}



void TrollOverlay::SetWidth( float p_width, bool p_updateOgre )
{
	m_width = p_width;

	if ( p_updateOgre )
	{
		m_museOverlay->SetWidth( p_width );
	}
	else if ( m_propertiesPanel != NULL )
	{
		m_propertiesPanel->UpdateProperties();
	}
}



void TrollOverlay::SetHeight( float p_height, bool p_updateOgre )
{
	m_height = p_height;

	if ( p_updateOgre )
	{
		m_museOverlay->SetHeight( p_height );
	}
	else if ( m_propertiesPanel != NULL )
	{
		m_propertiesPanel->UpdateProperties();
	}
}



void TrollOverlay::SetVerticalAlign( TEVerticAlign p_align, bool p_updateOgre )
{
	m_verticalAlign = p_align;

	if ( p_updateOgre )
	{
		m_museOverlay->GetOgreOverlayElement()->setVerticalAlignment( static_cast <Ogre::GuiVerticalAlignment>( p_align ) );
	}
	else if ( m_propertiesPanel != NULL )
	{
		m_propertiesPanel->UpdateProperties();
	}
}



void TrollOverlay::SetHorizontalAlign( TEHorizAlign p_align, bool p_updateOgre )
{
	m_horizontalAlign = p_align;

	if ( p_updateOgre )
	{
		m_museOverlay->GetOgreOverlayElement()->setHorizontalAlignment( static_cast <Ogre::GuiHorizontalAlignment>( p_align ) );
	}
	else if ( m_propertiesPanel != NULL )
	{
		m_propertiesPanel->UpdateProperties();
	}
}



void TrollOverlay::SetCaption( const wxString & p_caption, bool p_updateOgre )
{
	if ( m_textInfos == NULL )
	{
		return;
	}

	m_textInfos->m_caption = p_caption;

	if ( p_updateOgre )
	{
		m_museOverlay->SetCaption( p_caption.char_str().data() );
	}
	else if ( m_propertiesPanel != NULL )
	{
		m_propertiesPanel->UpdateProperties();
	}
}



void TrollOverlay::SetTextColour( const Ogre::ColourValue & p_colour, bool p_updateOgre )
{
	if ( m_textInfos == NULL )
	{
		return;
	}

	m_textInfos->m_textColour = p_colour;

	if ( p_updateOgre )
	{
		static_cast <Ogre::TextAreaOverlayElement *>( m_museOverlay->GetOgreOverlayElement() )->setColour( p_colour );
	}
	else if ( m_propertiesPanel != NULL )
	{
		m_propertiesPanel->UpdateProperties();
	}
}



void TrollOverlay::SetFontName( const wxString & p_fontName, bool p_updateOgre )
{
	if ( m_textInfos == NULL )
	{
		return;
	}

	m_textInfos->m_fontName = p_fontName;

	if ( p_updateOgre )
	{
		static_cast <Ogre::TextAreaOverlayElement *>( m_museOverlay->GetOgreOverlayElement() )->setFontName( p_fontName.char_str().data() );
	}
	else if ( m_propertiesPanel != NULL )
	{
		m_propertiesPanel->UpdateProperties();
	}
}



void TrollOverlay::SetFontSize( float p_height, bool p_updateOgre )
{
	if ( m_textInfos == NULL )
	{
		return;
	}

	m_textInfos->m_fontSize = p_height;

	if ( p_updateOgre )
	{
		static_cast <Ogre::TextAreaOverlayElement *>( m_museOverlay->GetOgreOverlayElement() )->setCharHeight( p_height );
	}
	else if ( m_propertiesPanel != NULL )
	{
		m_propertiesPanel->UpdateProperties();
	}
}



void TrollOverlay::SetTextHorizontalAlign( TEHorizAlign p_align, bool p_updateOgre )
{
	if ( m_textInfos == NULL )
	{
		return;
	}

	m_textInfos->m_horizontalAlign = p_align;

	if ( p_updateOgre )
	{
		static_cast <Ogre::TextAreaOverlayElement *>( m_museOverlay->GetOgreOverlayElement() )->setHorizontalAlignment( static_cast <Ogre::GuiHorizontalAlignment>( p_align ) );
	}
	else if ( m_propertiesPanel != NULL )
	{
		m_propertiesPanel->UpdateProperties();
	}
}



void TrollOverlay::SetTextVerticalAlign( TEVerticAlign p_align, bool p_updateOgre )
{
	if ( m_textInfos == NULL )
	{
		return;
	}

	m_textInfos->m_verticalAlign = p_align;

	if ( p_updateOgre )
	{
		static_cast <Ogre::TextAreaOverlayElement *>( m_museOverlay->GetOgreOverlayElement() )->setVerticalAlignment( static_cast <Ogre::GuiVerticalAlignment>( p_align ) );
	}
	else if ( m_propertiesPanel != NULL )
	{
		m_propertiesPanel->UpdateProperties();
	}
}



void TrollOverlay::SetVisible( bool p_visible )
{
	if ( p_visible )
	{
		m_museOverlay->GetOgreOverlayElement()->show();
	}
	else
	{
		m_museOverlay->GetOgreOverlayElement()->hide();
	}
}



void TrollOverlay::SetBorderMaterial( const wxString & p_name, bool p_updateOgre )
{
	if ( m_borderInfos == NULL )
	{
		m_borderInfos = new BorderInfos;
		m_borderInfos->m_bottom = 0.0;
		m_borderInfos->m_left = 0.0;
		m_borderInfos->m_right = 0.0;
		m_borderInfos->m_top = 0.0;
	}

	m_borderInfos->m_material = p_name;

	if ( p_updateOgre )
	{
		static_cast <Ogre::BorderPanelOverlayElement *>( m_museOverlay->GetOgreOverlayElement() )->setBorderMaterialName( p_name.char_str().data() );
	}
	else if ( m_propertiesPanel != NULL )
	{
		m_propertiesPanel->UpdateProperties();
	}
}



void TrollOverlay::SetBorderSize( float p_top, float p_bottom, float p_left, float p_right, bool p_updateOgre )
{
	if ( m_borderInfos == NULL )
	{
		m_borderInfos = new BorderInfos;
	}

	m_borderInfos->m_top = p_top;
	m_borderInfos->m_bottom = p_bottom;
	m_borderInfos->m_left = p_left;
	m_borderInfos->m_right = p_right;

	if ( p_updateOgre )
	{
		static_cast <Ogre::BorderPanelOverlayElement *>( m_museOverlay->GetOgreOverlayElement() )->setBorderSize( p_left, p_right, p_top, p_bottom );
	}
	else if ( m_propertiesPanel != NULL )
	{
		m_propertiesPanel->UpdateProperties();
	}
}



void TrollOverlay::SetBorderTop( float p_top, bool p_updateOgre )
{
	if ( m_borderInfos == NULL )
	{
		m_borderInfos = new BorderInfos;
	}

	m_borderInfos->m_top = p_top;
	float l_bottom = m_borderInfos->m_bottom;
	float l_left = m_borderInfos->m_left;
	float l_right = m_borderInfos->m_right;

	if ( p_updateOgre )
	{
		static_cast <Ogre::BorderPanelOverlayElement *>( m_museOverlay->GetOgreOverlayElement() )->setBorderSize( l_left, l_right, p_top, l_bottom );
	}
	else if ( m_propertiesPanel != NULL )
	{
		m_propertiesPanel->UpdateProperties();
	}
}



void TrollOverlay::SetBorderBottom( float p_bottom, bool p_updateOgre )
{
	if ( m_borderInfos == NULL )
	{
		m_borderInfos = new BorderInfos;
	}

	m_borderInfos->m_bottom = p_bottom;
	float l_top = m_borderInfos->m_top;
	float l_left = m_borderInfos->m_left;
	float l_right = m_borderInfos->m_right;

	if ( p_updateOgre )
	{
		static_cast <Ogre::BorderPanelOverlayElement *>( m_museOverlay->GetOgreOverlayElement() )->setBorderSize( l_left, l_right, l_top, p_bottom );
	}
	else if ( m_propertiesPanel != NULL )
	{
		m_propertiesPanel->UpdateProperties();
	}
}



void TrollOverlay::SetBorderLeft( float p_left, bool p_updateOgre )
{
	if ( m_borderInfos == NULL )
	{
		m_borderInfos = new BorderInfos;
	}

	m_borderInfos->m_left = p_left;
	float l_top = m_borderInfos->m_top;
	float l_bottom = m_borderInfos->m_bottom;
	float l_right = m_borderInfos->m_right;

	if ( p_updateOgre )
	{
		static_cast <Ogre::BorderPanelOverlayElement *>( m_museOverlay->GetOgreOverlayElement() )->setBorderSize( p_left, l_right, l_top, l_bottom );
	}
	else if ( m_propertiesPanel != NULL )
	{
		m_propertiesPanel->UpdateProperties();
	}
}



void TrollOverlay::SetBorderRight( float p_right, bool p_updateOgre )
{
	if ( m_borderInfos == NULL )
	{
		m_borderInfos = new BorderInfos;
	}

	m_borderInfos->m_right = p_right;
	float l_top = m_borderInfos->m_top;
	float l_bottom = m_borderInfos->m_bottom;
	float l_left = m_borderInfos->m_left;

	if ( p_updateOgre )
	{
		static_cast <Ogre::BorderPanelOverlayElement *>( m_museOverlay->GetOgreOverlayElement() )->setBorderSize( l_left, p_right, l_top, l_bottom );
	}
	else if ( m_propertiesPanel != NULL )
	{
		m_propertiesPanel->UpdateProperties();
	}
}



void TrollOverlay::Write( wxTextOutputStream * p_stream )
{
	wxString l_baseTabs;

	for ( int i = 0 ; i < m_depth ; i++ )
	{
		l_baseTabs += wxT( "\t" );
	}

	if ( m_parent != NULL )
	{
		p_stream->WriteString( l_baseTabs + wxT( "element " ) + m_inFileName + wxT( "\n" ) + l_baseTabs + wxT( "{\n" ) );
	}
	else
	{
		p_stream->WriteString( wxT( "overlay " ) + m_inFileName + wxT( "\n{\n" ) );
	}

	p_stream->WriteString( l_baseTabs + wxT( "\tclass " ) + make_wxString( m_museOverlay->GetOgreOverlayElement()->getSourceTemplate()->getName() ) + wxT( "\n" ) );
	wxString l_tmp;
	float l_top = m_top;
	float l_left = m_left;
	float l_height = m_height;
	float l_width = m_width;

//	std::cout << "TrollOverlay::Write - " << ScriptEngine::GetContext()->mainViewport->getActualWidth() << "x" << ScriptEngine::GetContext()->mainViewport->getActualHeight() << " - " << g_mainFrame->m_testPanel->GetClientSize().x << "x" << g_mainFrame->m_testPanel->GetClientSize().y << "\n";
	if ( m_parent != NULL )
	{
		l_top /= m_parent->GetHeight();
		l_height /= m_parent->GetHeight();
		l_left /= m_parent->GetWidth();
		l_width /= m_parent->GetWidth();
	}

	if ( m_top != 0 || m_left != 0 )
	{
		std::cout << "TrollOverlay::Write - " << m_name << " - " << l_left << "," << l_top << " - " << l_left * ScriptEngine::GetContext()->mainViewport->getActualWidth() << "," << l_top * ScriptEngine::GetContext()->mainViewport->getActualHeight() << "\n";
		l_tmp.Printf( wxT( "%.2f%% %.2f%%" ), 100 * l_left, 100 * l_top );
		l_tmp.Replace( wxT( "," ), wxT( "." ) );
		p_stream->WriteString( l_baseTabs + wxT( "\tposition " ) + l_tmp + wxT( "\n" ) );
		std::cout << "TrollOverlay::Write - " << m_name << " - " << l_tmp << "\n";
		l_tmp.clear();
	}

	l_tmp.Printf( wxT( "%.2f%% %.2f%%" ), 100 * l_width, 100 * l_height );
	l_tmp.Replace( wxT( "," ), wxT( "." ) );
	p_stream->WriteString( l_baseTabs + wxT( "\tsize " ) + l_tmp + wxT( "\n" ) );
	l_tmp.clear();

	if ( m_verticalAlign != vNone && m_horizontalAlign != hNone && ( m_verticalAlign != vTop || m_horizontalAlign != hLeft ) )
	{
		l_tmp = ( m_verticalAlign == vTop ? make_wxString( "top" ) : ( m_verticalAlign == vCenter ? make_wxString( "center" ) : make_wxString( "bottom" ) ) )
				+ make_wxString( " " )
				+ ( m_horizontalAlign == hLeft ? make_wxString( "left" ) : ( m_horizontalAlign == hCenter ? make_wxString( "center" ) : make_wxString( "right" ) ) );
		p_stream->WriteString( l_baseTabs + wxT( "\talign " ) + l_tmp + wxT( "\n" ) );
		l_tmp.clear();
	}

	if ( ! m_baseMaterial.empty() )
	{
		p_stream->WriteString( l_baseTabs + wxT( "\tmaterial " ) + m_baseMaterial + wxT( "\n" ) );
	}

	if ( ! m_mouseOverMaterial.empty() )
	{
		p_stream->WriteString( l_baseTabs + wxT( "\tmouse_over_material " ) + m_mouseOverMaterial + wxT( "\n" ) );
	}

	if ( ! m_clickedMaterial.empty() )
	{
		p_stream->WriteString( l_baseTabs + wxT( "\tclicked_material " ) + m_clickedMaterial + wxT( "\n" ) );
	}

	if ( m_borderInfos != NULL )
	{
		float l_screenWidth = float( ScriptEngine::GetContext()->mainViewport->getActualWidth() );
		float l_screenHeight = float( ScriptEngine::GetContext()->mainViewport->getActualHeight() );
		p_stream->WriteString( l_baseTabs + wxT( "\tborder_material " ) + m_borderInfos->m_material + wxT( "\n" ) );
		l_tmp.Printf( wxT( "%.2f %.2f %.2f %.2f" ), m_borderInfos->m_left * l_screenWidth, m_borderInfos->m_right * l_screenWidth,
					  m_borderInfos->m_top * l_screenHeight, m_borderInfos->m_bottom * l_screenHeight );
		l_tmp.Replace( wxT( "," ), wxT( "." ) );
		p_stream->WriteString( l_baseTabs + wxT( "\tborder_size " ) + l_tmp + wxT( "\n" ) );
	}

	if ( m_textInfos != NULL )
	{
		p_stream->WriteString( l_baseTabs + wxT( "\tfont_name " ) + m_textInfos->m_fontName + wxT( "\n" ) );
		l_tmp.Printf( wxT( "%.2f" ), m_textInfos->m_fontSize * ScriptEngine::GetContext()->mainViewport->getActualHeight() );
		l_tmp.Replace( wxT( "," ), wxT( "." ) );
		p_stream->WriteString( l_baseTabs + wxT( "\tfont_size " ) + l_tmp + wxT( "\n" ) );
		l_tmp.clear();
		l_tmp.Printf( wxT( "%.2f %.2f %.2f %.2f" ), m_textInfos->m_textColour.r, m_textInfos->m_textColour.g, m_textInfos->m_textColour.b, m_textInfos->m_textColour.a );
		l_tmp.Replace( wxT( "," ), wxT( "." ) );
		p_stream->WriteString( l_baseTabs + wxT( "\ttext_colour " ) + l_tmp + wxT( "\n" ) );
		l_tmp.clear();

		if ( m_textInfos->m_verticalAlign != vNone && m_textInfos->m_horizontalAlign != hNone &&
				( m_textInfos->m_verticalAlign != vTop || m_textInfos->m_horizontalAlign != hLeft ) )
		{
			l_tmp = ( m_textInfos->m_verticalAlign == vTop ? make_wxString( "top" ) : ( m_textInfos->m_verticalAlign == vCenter ? make_wxString( "center" ) : make_wxString( "bottom" ) ) )
					+ make_wxString( " " )
					+ ( m_textInfos->m_horizontalAlign == hLeft ? make_wxString( "left" ) : ( m_textInfos->m_horizontalAlign == hCenter ? make_wxString( "center" ) : make_wxString( "right" ) ) );
			p_stream->WriteString( l_baseTabs + wxT( "\ttext_align " ) + l_tmp + wxT( "\n" ) );
			l_tmp.clear();
		}

		l_tmp = m_textInfos->m_caption;
		l_tmp.Replace( wxT( "\n" ), wxT( "" ) );
		p_stream->WriteString( l_baseTabs + wxT( "\ttext " ) + l_tmp + wxT( "\n" ) );
	}

	if ( m_video != NULL )
	{
		m_video->Write( p_stream );
	}

	TrollOverlayMap::iterator l_it = m_childs.begin();

	for ( ; l_it != m_childs.end() ; ++l_it )
	{
		std::cout << "TrollOverlay::Write - " << l_it->first << " - " << l_it->second << "\n";
		l_it->second->Write( p_stream );
	}

	p_stream->WriteString( l_baseTabs + wxT( "}\n\n" ) );
}


