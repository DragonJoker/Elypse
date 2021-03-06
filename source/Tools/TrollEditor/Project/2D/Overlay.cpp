#include "Overlay.h"

#include "Project/Object.h"
#include "Project/Media/VideoObject.h"
#include "GUI/Properties/Project/OverlayProperties.h"
#include "GUI/MainFrame.h"
#include "GUI/ProjectFrame.h"

namespace Troll
{
	namespace ProjectComponents
	{
		namespace Objects2D
		{
			TrollOverlay::TrollOverlay( wxString const & p_name, TrollOverlay * p_parent, wxString const & p_fileName )
				: Object( p_name, p_fileName, ttOverlay )
				, m_parent( p_parent )
				, m_verticalAlign( vNone )
				, m_horizontalAlign( hNone )
				, m_borderInfos( nullptr )
				, m_video( nullptr )
				, m_depth( 0 )
				, m_inFileName( p_name )
				, m_textInfos( nullptr )
				, m_museOverlay( nullptr )
			{
				if ( m_parent != nullptr )
				{
					m_depth = m_parent->m_depth + 1;
					m_parent->AddChild( this, false );
				}
			}

			TrollOverlay::~TrollOverlay()
			{
				if ( m_textInfos != nullptr )
				{
					delete m_textInfos;
					m_textInfos = nullptr;
				}

				if ( m_borderInfos != nullptr )
				{
					delete m_borderInfos;
					m_borderInfos = nullptr;
				}
			}

			void TrollOverlay::Reinitialise( wxString const & p_templateName )
			{
				m_museOverlay->Reinitialise( make_string( p_templateName ) );
				wxString l_templateName = p_templateName;
				l_templateName.MakeLower();

				if ( l_templateName.find( wxT( "border" ) ) == wxString::npos && m_borderInfos != nullptr )
				{
					delete m_borderInfos;
					m_borderInfos = nullptr;
				}

				if ( l_templateName.find( wxT( "Text" ) ) == wxString::npos && m_textInfos != nullptr )
				{
					delete m_textInfos;
					m_textInfos = nullptr;
				}
			}

			void TrollOverlay::AddChild( TrollOverlay * p_child, bool p_updateElypse )
			{
				if ( m_childs.find( p_child->GetObjectName() ) == m_childs.end() )
				{
					m_childs.insert( std::make_pair( p_child->GetObjectName(), p_child ) );
					p_child->m_parent = this;
				}
			}

			void TrollOverlay::RemoveChild( wxString const & p_name )
			{
				TrollOverlayMap::iterator l_it = m_childs.find( p_name );

				if ( l_it != m_childs.end() )
				{
					l_it->second->m_parent = nullptr;
					m_childs.erase( l_it );
				}
			}

			GUI::Properties::ObjectProperty * TrollOverlay::DoCreateProperties()
			{
				return new GUI::Properties::OverlayProperties( *this );
			}

			void TrollOverlay::SetMuseOverlay( Elypse::Gui::EMOverlay * p_museOverlay )
			{
				m_museOverlay = p_museOverlay;
				m_baseMaterial = make_wxString( m_museOverlay->GetBaseMaterial() );
				m_mouseOverMaterial = make_wxString( m_museOverlay->GetMouseOverMaterial() );
				m_clickedMaterial = make_wxString( m_museOverlay->GetClickedMaterial() );
				m_position = { m_museOverlay->GetLeft(), m_museOverlay->GetTop() };
				m_size = { m_museOverlay->GetWidth(), m_museOverlay->GetHeight() };
				m_horizontalAlign = static_cast < TEHorizAlign >( m_museOverlay->GetOgreOverlayElement()->getHorizontalAlignment() );
				m_verticalAlign = static_cast < TEVerticAlign >( m_museOverlay->GetOgreOverlayElement()->getVerticalAlignment() );

				if ( m_museOverlay->GetOgreOverlayElement()->getTypeName() == "TextArea" )
				{
					Ogre::TextAreaOverlayElement * l_overlayElement = static_cast < Ogre::TextAreaOverlayElement * >( m_museOverlay->GetOgreOverlayElement() );
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
					Ogre::BorderPanelOverlayElement * l_overlayElement = static_cast < Ogre::BorderPanelOverlayElement * >( m_museOverlay->GetOgreOverlayElement() );

					if ( m_borderInfos == nullptr )
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

			void TrollOverlay::SetOverlayClass( wxString const & p_className )
			{
				m_className = p_className;
			}

			void TrollOverlay::SetBaseMaterial( wxString const & p_matName, bool p_updateOgre )
			{
				m_baseMaterial = p_matName;

				if ( p_updateOgre )
				{
					m_museOverlay->SetBaseMaterial( make_string( p_matName ) );
				}
				else if ( m_properties )
				{
					m_properties->UpdateProperties();
				}
			}

			void TrollOverlay::SetMouseOverMaterial( wxString const & p_matName, bool p_updateOgre )
			{
				m_mouseOverMaterial = p_matName;

				if ( p_updateOgre )
				{
					m_museOverlay->SetMouseOverMaterial( make_string( p_matName ) );
				}
				else if ( m_properties )
				{
					m_properties->UpdateProperties();
				}
			}

			void TrollOverlay::SetClickedMaterial( wxString const & p_matName, bool p_updateOgre )
			{
				m_clickedMaterial = p_matName;

				if ( p_updateOgre )
				{
					m_museOverlay->SetClickedMaterial( make_string( p_matName ) );
				}
				else if ( m_properties )
				{
					m_properties->UpdateProperties();
				}
			}

			void TrollOverlay::SetPosition( Vector2 const & p_position, bool p_updateOgre )
			{
				m_position = p_position;

				if ( p_updateOgre )
				{
					m_museOverlay->SetLeft( p_position.x );
					m_museOverlay->SetTop( p_position.y );
				}
				else if ( m_properties )
				{
					m_properties->UpdateProperties();
				}
			}

			void TrollOverlay::SetLeft( float p_value, bool p_updateOgre )
			{
				m_position.x = p_value;

				if ( p_updateOgre )
				{
					m_museOverlay->SetLeft( p_value );
				}
				else if ( m_properties )
				{
					m_properties->UpdateProperties();
				}
			}

			void TrollOverlay::SetTop( float p_value, bool p_updateOgre )
			{
				m_position.y = p_value;

				if ( p_updateOgre )
				{
					m_museOverlay->SetTop( p_value );
				}
				else if ( m_properties )
				{
					m_properties->UpdateProperties();
				}
			}

			void TrollOverlay::SetSize( Vector2 const & p_size, bool p_updateOgre )
			{
				m_size = p_size;

				if ( p_updateOgre )
				{
					m_museOverlay->SetWidth( p_size.x );
					m_museOverlay->SetHeight( p_size.y );
				}
				else if ( m_properties )
				{
					m_properties->UpdateProperties();
				}
			}

			void TrollOverlay::SetWidth( float p_value, bool p_updateOgre )
			{
				m_size.x = p_value;

				if ( p_updateOgre )
				{
					m_museOverlay->SetWidth( p_value );
				}
				else if ( m_properties )
				{
					m_properties->UpdateProperties();
				}
			}

			void TrollOverlay::SetHeight( float p_value, bool p_updateOgre )
			{
				m_size.y = p_value;

				if ( p_updateOgre )
				{
					m_museOverlay->SetHeight( p_value );
				}
				else if ( m_properties )
				{
					m_properties->UpdateProperties();
				}
			}

			void TrollOverlay::SetVerticalAlign( TEVerticAlign p_align, bool p_updateOgre )
			{
				m_verticalAlign = p_align;

				if ( p_updateOgre )
				{
					m_museOverlay->GetOgreOverlayElement()->setVerticalAlignment( static_cast < Ogre::GuiVerticalAlignment >( p_align ) );
				}
				else if ( m_properties )
				{
					m_properties->UpdateProperties();
				}
			}

			void TrollOverlay::SetHorizontalAlign( TEHorizAlign p_align, bool p_updateOgre )
			{
				m_horizontalAlign = p_align;

				if ( p_updateOgre )
				{
					m_museOverlay->GetOgreOverlayElement()->setHorizontalAlignment( static_cast < Ogre::GuiHorizontalAlignment >( p_align ) );
				}
				else if ( m_properties )
				{
					m_properties->UpdateProperties();
				}
			}

			void TrollOverlay::SetCaption( wxString const & p_caption, bool p_updateOgre )
			{
				if ( m_textInfos == nullptr )
				{
					return;
				}

				m_textInfos->m_caption = p_caption;

				if ( p_updateOgre )
				{
					m_museOverlay->SetCaption( make_string( p_caption ) );
				}
				else if ( m_properties )
				{
					m_properties->UpdateProperties();
				}
			}

			void TrollOverlay::SetTextColour( const Ogre::ColourValue & p_colour, bool p_updateOgre )
			{
				if ( m_textInfos == nullptr )
				{
					return;
				}

				m_textInfos->m_textColour = p_colour;

				if ( p_updateOgre )
				{
					static_cast < Ogre::TextAreaOverlayElement * >( m_museOverlay->GetOgreOverlayElement() )->setColour( p_colour );
				}
				else if ( m_properties )
				{
					m_properties->UpdateProperties();
				}
			}

			void TrollOverlay::SetFontName( wxString const & p_fontName, bool p_updateOgre )
			{
				if ( m_textInfos == nullptr )
				{
					return;
				}

				m_textInfos->m_fontName = p_fontName;

				if ( p_updateOgre )
				{
					static_cast < Ogre::TextAreaOverlayElement * >( m_museOverlay->GetOgreOverlayElement() )->setFontName( make_string( p_fontName ) );
				}
				else if ( m_properties )
				{
					m_properties->UpdateProperties();
				}
			}

			void TrollOverlay::SetFontSize( float p_height, bool p_updateOgre )
			{
				if ( m_textInfos == nullptr )
				{
					return;
				}

				m_textInfos->m_fontSize = p_height;

				if ( p_updateOgre )
				{
					static_cast < Ogre::TextAreaOverlayElement * >( m_museOverlay->GetOgreOverlayElement() )->setCharHeight( p_height );
				}
				else if ( m_properties )
				{
					m_properties->UpdateProperties();
				}
			}

			void TrollOverlay::SetTextHorizontalAlign( TEHorizAlign p_align, bool p_updateOgre )
			{
				if ( m_textInfos == nullptr )
				{
					return;
				}

				m_textInfos->m_horizontalAlign = p_align;

				if ( p_updateOgre )
				{
					static_cast < Ogre::TextAreaOverlayElement * >( m_museOverlay->GetOgreOverlayElement() )->setHorizontalAlignment( static_cast < Ogre::GuiHorizontalAlignment >( p_align ) );
				}
				else if ( m_properties )
				{
					m_properties->UpdateProperties();
				}
			}

			void TrollOverlay::SetTextVerticalAlign( TEVerticAlign p_align, bool p_updateOgre )
			{
				if ( m_textInfos == nullptr )
				{
					return;
				}

				m_textInfos->m_verticalAlign = p_align;

				if ( p_updateOgre )
				{
					static_cast < Ogre::TextAreaOverlayElement * >( m_museOverlay->GetOgreOverlayElement() )->setVerticalAlignment( static_cast < Ogre::GuiVerticalAlignment >( p_align ) );
				}
				else if ( m_properties )
				{
					m_properties->UpdateProperties();
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

			void TrollOverlay::SetBorderMaterial( wxString const & p_name, bool p_updateOgre )
			{
				if ( m_borderInfos == nullptr )
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
					static_cast < Ogre::BorderPanelOverlayElement * >( m_museOverlay->GetOgreOverlayElement() )->setBorderMaterialName( make_string( p_name ) );
				}
				else if ( m_properties )
				{
					m_properties->UpdateProperties();
				}
			}

			void TrollOverlay::SetBorderSize( float p_top, float p_bottom, float p_left, float p_right, bool p_updateOgre )
			{
				if ( m_borderInfos == nullptr )
				{
					m_borderInfos = new BorderInfos;
				}

				m_borderInfos->m_top = p_top;
				m_borderInfos->m_bottom = p_bottom;
				m_borderInfos->m_left = p_left;
				m_borderInfos->m_right = p_right;

				if ( p_updateOgre )
				{
					static_cast < Ogre::BorderPanelOverlayElement * >( m_museOverlay->GetOgreOverlayElement() )->setBorderSize( p_left, p_right, p_top, p_bottom );
				}
				else if ( m_properties )
				{
					m_properties->UpdateProperties();
				}
			}

			void TrollOverlay::SetBorderTop( float p_top, bool p_updateOgre )
			{
				if ( m_borderInfos == nullptr )
				{
					m_borderInfos = new BorderInfos;
				}

				m_borderInfos->m_top = p_top;
				float l_bottom = m_borderInfos->m_bottom;
				float l_left = m_borderInfos->m_left;
				float l_right = m_borderInfos->m_right;

				if ( p_updateOgre )
				{
					static_cast < Ogre::BorderPanelOverlayElement * >( m_museOverlay->GetOgreOverlayElement() )->setBorderSize( l_left, l_right, p_top, l_bottom );
				}
				else if ( m_properties )
				{
					m_properties->UpdateProperties();
				}
			}

			void TrollOverlay::SetBorderBottom( float p_bottom, bool p_updateOgre )
			{
				if ( m_borderInfos == nullptr )
				{
					m_borderInfos = new BorderInfos;
				}

				m_borderInfos->m_bottom = p_bottom;
				float l_top = m_borderInfos->m_top;
				float l_left = m_borderInfos->m_left;
				float l_right = m_borderInfos->m_right;

				if ( p_updateOgre )
				{
					static_cast < Ogre::BorderPanelOverlayElement * >( m_museOverlay->GetOgreOverlayElement() )->setBorderSize( l_left, l_right, l_top, p_bottom );
				}
				else if ( m_properties )
				{
					m_properties->UpdateProperties();
				}
			}

			void TrollOverlay::SetBorderLeft( float p_left, bool p_updateOgre )
			{
				if ( m_borderInfos == nullptr )
				{
					m_borderInfos = new BorderInfos;
				}

				m_borderInfos->m_left = p_left;
				float l_top = m_borderInfos->m_top;
				float l_bottom = m_borderInfos->m_bottom;
				float l_right = m_borderInfos->m_right;

				if ( p_updateOgre )
				{
					static_cast < Ogre::BorderPanelOverlayElement * >( m_museOverlay->GetOgreOverlayElement() )->setBorderSize( p_left, l_right, l_top, l_bottom );
				}
				else if ( m_properties )
				{
					m_properties->UpdateProperties();
				}
			}

			void TrollOverlay::SetBorderRight( float p_right, bool p_updateOgre )
			{
				if ( m_borderInfos == nullptr )
				{
					m_borderInfos = new BorderInfos;
				}

				m_borderInfos->m_right = p_right;
				float l_top = m_borderInfos->m_top;
				float l_bottom = m_borderInfos->m_bottom;
				float l_left = m_borderInfos->m_left;

				if ( p_updateOgre )
				{
					static_cast < Ogre::BorderPanelOverlayElement * >( m_museOverlay->GetOgreOverlayElement() )->setBorderSize( l_left, p_right, l_top, l_bottom );
				}
				else if ( m_properties )
				{
					m_properties->UpdateProperties();
				}
			}

			void TrollOverlay::Write( wxTextOutputStream & p_stream )
			{
				wxString l_baseTabs;

				for ( int i = 0; i < m_depth; i++ )
				{
					l_baseTabs += wxT( "\t" );
				}

				if ( m_parent != nullptr )
				{
					p_stream << l_baseTabs << wxT( "element " ) << m_inFileName << wxT( "\n" ) << l_baseTabs << wxT( "{\n" );
				}
				else
				{
					p_stream << wxT( "overlay " ) << m_inFileName << wxT( "\n{\n" );
				}

				p_stream << l_baseTabs << wxT( "\tclass " ) << make_wxString( m_museOverlay->GetOgreOverlayElement()->getSourceTemplate()->getName() ) << wxT( "\n" );
				wxString l_tmp;
				float l_left = m_position.x;
				float l_top = m_position.y;
				float l_width = m_size.x;
				float l_height = m_size.y;

				//std::cout << "TrollOverlay::Write - " << ScriptEngine::GetContext()->mainViewport->getActualWidth() << "x" << ScriptEngine::GetContext()->mainViewport->getActualHeight() << " - " << wxGetApp().GetMainFrame()->m_testPanel->GetClientSize().x << "x" << wxGetApp().GetMainFrame()->m_testPanel->GetClientSize().y << "\n";
				if ( m_parent != nullptr )
				{
					l_top /= m_parent->GetSize().y;
					l_height /= m_parent->GetSize().y;
					l_left /= m_parent->GetSize().x;
					l_width /= m_parent->GetSize().x;
				}

				if ( m_position.x != 0 || m_position.y != 0 )
				{
					std::cout << "TrollOverlay::Write - " << m_name << " - " << l_left << "," << l_top << " - " << l_left * ScriptEngine::GetContext()->mainViewport->getActualWidth() << "," << l_top * ScriptEngine::GetContext()->mainViewport->getActualHeight() << "\n";
					l_tmp.Printf( wxT( "%.2f%% %.2f%%" ), 100 * l_left, 100 * l_top );
					l_tmp.Replace( wxT( "," ), wxT( "." ) );
					p_stream.WriteString( l_baseTabs + wxT( "\tposition " ) + l_tmp + wxT( "\n" ) );
					std::cout << "TrollOverlay::Write - " << m_name << " - " << l_tmp << "\n";
					l_tmp.clear();
				}

				l_tmp.Printf( wxT( "%.2f%% %.2f%%" ), 100 * l_width, 100 * l_height );
				l_tmp.Replace( wxT( "," ), wxT( "." ) );
				p_stream.WriteString( l_baseTabs + wxT( "\tsize " ) + l_tmp + wxT( "\n" ) );
				l_tmp.clear();

				if ( m_verticalAlign != vNone && m_horizontalAlign != hNone && ( m_verticalAlign != vTop || m_horizontalAlign != hLeft ) )
				{
					l_tmp = ( m_verticalAlign == vTop ? make_wxString( "top" ) : ( m_verticalAlign == vCenter ? make_wxString( "center" ) : make_wxString( "bottom" ) ) )
							+ make_wxString( " " )
							+ ( m_horizontalAlign == hLeft ? make_wxString( "left" ) : ( m_horizontalAlign == hCenter ? make_wxString( "center" ) : make_wxString( "right" ) ) );
					p_stream.WriteString( l_baseTabs + wxT( "\talign " ) + l_tmp + wxT( "\n" ) );
					l_tmp.clear();
				}

				if ( !m_baseMaterial.empty() )
				{
					p_stream.WriteString( l_baseTabs + wxT( "\tmaterial " ) + m_baseMaterial + wxT( "\n" ) );
				}

				if ( !m_mouseOverMaterial.empty() )
				{
					p_stream.WriteString( l_baseTabs + wxT( "\tmouse_over_material " ) + m_mouseOverMaterial + wxT( "\n" ) );
				}

				if ( !m_clickedMaterial.empty() )
				{
					p_stream.WriteString( l_baseTabs + wxT( "\tclicked_material " ) + m_clickedMaterial + wxT( "\n" ) );
				}

				if ( m_borderInfos != nullptr )
				{
					float l_screenWidth = float( ScriptEngine::GetContext()->mainViewport->getActualWidth() );
					float l_screenHeight = float( ScriptEngine::GetContext()->mainViewport->getActualHeight() );
					p_stream.WriteString( l_baseTabs + wxT( "\tborder_material " ) + m_borderInfos->m_material + wxT( "\n" ) );
					l_tmp.Printf( wxT( "%.2f %.2f %.2f %.2f" ), m_borderInfos->m_left * l_screenWidth, m_borderInfos->m_right * l_screenWidth,
								  m_borderInfos->m_top * l_screenHeight, m_borderInfos->m_bottom * l_screenHeight );
					l_tmp.Replace( wxT( "," ), wxT( "." ) );
					p_stream.WriteString( l_baseTabs + wxT( "\tborder_size " ) + l_tmp + wxT( "\n" ) );
				}

				if ( m_textInfos != nullptr )
				{
					p_stream.WriteString( l_baseTabs + wxT( "\tfont_name " ) + m_textInfos->m_fontName + wxT( "\n" ) );
					l_tmp.Printf( wxT( "%.2f" ), m_textInfos->m_fontSize * ScriptEngine::GetContext()->mainViewport->getActualHeight() );
					l_tmp.Replace( wxT( "," ), wxT( "." ) );
					p_stream.WriteString( l_baseTabs + wxT( "\tfont_size " ) + l_tmp + wxT( "\n" ) );
					l_tmp.clear();
					l_tmp.Printf( wxT( "%.2f %.2f %.2f %.2f" ), m_textInfos->m_textColour.r, m_textInfos->m_textColour.g, m_textInfos->m_textColour.b, m_textInfos->m_textColour.a );
					l_tmp.Replace( wxT( "," ), wxT( "." ) );
					p_stream.WriteString( l_baseTabs + wxT( "\ttext_colour " ) + l_tmp + wxT( "\n" ) );
					l_tmp.clear();

					if ( m_textInfos->m_verticalAlign != vNone && m_textInfos->m_horizontalAlign != hNone &&
							( m_textInfos->m_verticalAlign != vTop || m_textInfos->m_horizontalAlign != hLeft ) )
					{
						l_tmp = ( m_textInfos->m_verticalAlign == vTop ? make_wxString( "top" ) : ( m_textInfos->m_verticalAlign == vCenter ? make_wxString( "center" ) : make_wxString( "bottom" ) ) )
								+ make_wxString( " " )
								+ ( m_textInfos->m_horizontalAlign == hLeft ? make_wxString( "left" ) : ( m_textInfos->m_horizontalAlign == hCenter ? make_wxString( "center" ) : make_wxString( "right" ) ) );
						p_stream.WriteString( l_baseTabs + wxT( "\ttext_align " ) + l_tmp + wxT( "\n" ) );
						l_tmp.clear();
					}

					l_tmp = m_textInfos->m_caption;
					l_tmp.Replace( wxT( "\n" ), wxT( "" ) );
					p_stream.WriteString( l_baseTabs + wxT( "\ttext " ) + l_tmp + wxT( "\n" ) );
				}

				if ( m_video != nullptr )
				{
					m_video->Write( p_stream );
				}

				for ( auto && l_it : m_childs )
				{
					std::cout << "TrollOverlay::Write - " << l_it.first << " - " << l_it.second << "\n";
					l_it.second->Write( p_stream );
				}

				p_stream.WriteString( l_baseTabs + wxT( "}\n\n" ) );
			}
		}
	}
}
