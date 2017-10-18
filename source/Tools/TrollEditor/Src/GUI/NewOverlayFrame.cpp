#include "GUI/NewOverlayFrame.h"

#include "TrollEditorApplication.h"

#include "GUI/ElypseCtrl.h"
#include "GUI/ElypseFrame.h"
#include "GUI/ElypsePanel.h"
#include "GUI/ProjectFrame.h"
#include "GUI/ObjectsTree.h"

#include "GUI/Properties/Common/PropertiesHolder.h"

#include "Project/2D/Overlay.h"
#include "Project/Project.h"
#include "Project/Scene.h"

using namespace Ogre;

namespace Troll
{
	namespace GUI
	{
		enum NowOverlayFrameIDs
		{
			noIdTemplate,
			noIdOK,
			noIdCancel
		};

		BEGIN_EVENT_TABLE( NewOverlayFrame, wxFrame )
			EVT_COMBOBOX( noIdTemplate, NewOverlayFrame::OnSelectTemplate )
			EVT_BUTTON( noIdOK, NewOverlayFrame::OnOK )
			EVT_BUTTON( noIdCancel, NewOverlayFrame::OnCancel )
		END_EVENT_TABLE()

		NewOverlayFrame::NewOverlayFrame( ProjectFrame * p_parent, wxPoint const & p_position, EMOverlay * p_parentOverlay, wxString const & p_name )
			: wxFrame( p_parent, wxID_ANY, _( "New Overlay" ), p_position, wxSize( 250, 422 ), wxFRAME_TOOL_WINDOW | wxCAPTION | wxCLOSE_BOX | wxFRAME_FLOAT_ON_PARENT )
			, m_parentOverlay( p_parentOverlay )
			, m_parentGroup( NULL )
			, m_gui( ScriptEngine::GetContext()->gui )
			, m_overlay( NULL )
			, m_mustBeClosed( false )
			, m_overlayName( p_name )
			, m_projectFrame( p_parent )
		{
			if ( m_overlayName.empty() )
			{
				static int sl_number = 0;
				m_overlayName = _( "NewOverlay" );
				m_overlayName << sl_number++;
			}

			if ( m_parentOverlay != NULL )
			{
				m_parentGroup = m_parentOverlay->GetOwner();
				wxString l_strName( make_wxString( m_parentOverlay->GetName() ) );
				ProjectComponents::Objects2D::TrollOverlay * l_parentTrollOverlay = m_projectFrame->GetProject()->GetMainScene()->GetOverlay( l_strName );

				if ( l_parentTrollOverlay->GetTextInfos() != NULL )
				{
					wxMessageBox( _( "Can't create an aoverlay in a text" ), TROLL_EDITOR_ERROR, wxID_OK );
					m_mustBeClosed = true;
				}
			}
			else
			{
				m_parentGroup = ScriptEngine::GetContext()->gui->CreateOverlayGroup( make_string( m_overlayName ) );
			}

			wxSize l_size = GetClientSize();
			wxArrayString l_choices;
			_getOverlayTemplates( l_choices );
			wxComboBox * l_box = BuildComboBox( this, wxPoint( 100, 10 ), noIdTemplate, _( "Template" ), l_choices, ( l_choices.size() > 0 ? l_choices[0] : wxT( "" ) ) );
			wxButton * l_button = new wxButton( this, noIdOK, _( "OK" ), wxPoint( 10, 370 ), wxSize( 100, 20 ), wxBORDER_SIMPLE );
			l_button = new wxButton( this, noIdCancel, _( "Cancel" ), wxPoint( 135, 370 ), wxSize( 100, 20 ), wxBORDER_SIMPLE );
			m_propertiesPanel = new Properties::PropertiesHolder{ this, wxPoint( 5, 35 ), wxSize( l_size.x - 10, l_size.y - 80 ) };
		}



		void NewOverlayFrame::_getOverlayTemplates( wxArrayString & p_array )
		{
			p_array.clear();
			OverlayManager::TemplateIterator l_it = OverlayManager::getSingletonPtr()->getTemplateIterator();

			while ( l_it.hasMoreElements() )
			{
				p_array.push_back( make_wxString( l_it.getNext()->getName().c_str() ) );
			}
		}

		void NewOverlayFrame::OnSelectTemplate( wxCommandEvent & p_event )
		{
			EMOverlay * l_emov;
			String l_templateName = make_string( p_event.GetString() );

			if ( m_overlay != NULL )
			{
				m_overlay->Reinitialise( make_wxString( l_templateName.c_str() ) );
			}
			else
			{
				ProjectComponents::Objects2D::TrollOverlay * l_parentTrollOverlay = NULL;
				wxArrayString l_list;
				m_projectFrame->GetProject()->GetMainScene()->GetOverlaysNames( l_list );
				wxString l_fileName;

				if ( l_list.size() > 0 )
				{
					l_fileName = m_projectFrame->GetProject()->GetMainScene()->GetOverlay( l_list[0] )->GetFileName();
				}
				else
				{
					l_fileName = m_projectFrame->GetProject()->GetMainScene()->GetSceneFile( 0 ).FileName;
				}

				if ( m_parentOverlay != NULL )
				{
					l_emov = m_parentOverlay->CreateChild( make_string( m_overlayName ) );
					l_emov->Initialise( l_templateName );
					l_parentTrollOverlay = m_projectFrame->GetProject()->GetMainScene()->GetOverlay( make_wxString( m_parentOverlay->GetName() ) );
				}
				else
				{
					l_emov = m_parentGroup->GetFirstElement();
					l_emov->Initialise( l_templateName );
				}

				m_overlay = new ProjectComponents::Objects2D::TrollOverlay( m_overlayName, l_parentTrollOverlay, l_fileName );
				m_overlay->SetMuseOverlay( l_emov );
			}

			m_overlay->CreateProperties( m_propertiesPanel );
		}

		void NewOverlayFrame::OnOK( wxCommandEvent & p_event )
		{
			if ( m_overlay != NULL )
			{
				m_projectFrame->AddOverlay( m_overlay, m_parentOverlay );
				m_overlay->HideProperties();
			}

			Close();
			m_projectFrame->GetElypseCtrl()->SetNewOverlayFrame( NULL );
		}

		void NewOverlayFrame::OnCancel( wxCommandEvent & p_event )
		{
			if ( m_overlay != NULL )
			{
				EMOverlayGroup * l_group = m_overlay->GetMuseOverlay()->GetOwner();

				if ( m_parentOverlay == NULL )
				{
					m_gui->DestroyOverlayGroup( l_group->GetName() );
				}
				else
				{
					l_group->DestroyElement( m_overlay->GetMuseOverlay()->GetName() );
				}

				delete m_overlay;
			}

			Close();
			m_projectFrame->GetElypseCtrl()->SetNewOverlayFrame( NULL );
		}
	}
}
