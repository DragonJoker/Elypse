
#include "PrecompiledHeader.h"

#include "NewOverlayFrame.h"

#include "ElypseCtrl.h"
#include "ElypseFrame.h"
#include "ElypsePanel.h"
#include "MainFrame.h"
#include "ObjectsTree.h"
#include "Project/2D/Overlay.h"
#include "Project/Project.h"
#include "Project/Scene.h"

using namespace Troll::GUI;
using namespace Troll::Objects2D;
using namespace Ogre;

enum NowOverlayFrameIDs
{
	noIdTemplate,
	noIdOK,
	noIdCancel
};

BEGIN_EVENT_TABLE( NewOverlayFrame, wxFrame )
	EVT_COMBOBOX(	noIdTemplate,			NewOverlayFrame::_onSelectTemplate )
	EVT_BUTTON(	noIdOK,					NewOverlayFrame::_onOK )
	EVT_BUTTON(	noIdCancel,				NewOverlayFrame::_onCancel )
END_EVENT_TABLE()


NewOverlayFrame::NewOverlayFrame( const wxPoint & p_position, EMOverlay * p_parentOverlay,
								  const wxString & p_name )
	:	wxFrame( g_mainFrame, wxID_ANY, wxT( "Nouvel Overlay" ), p_position,
				 wxSize( 250, 422 ), wxFRAME_TOOL_WINDOW | wxCAPTION | wxCLOSE_BOX | wxFRAME_FLOAT_ON_PARENT ),
	m_parentOverlay( p_parentOverlay ),
	m_parentGroup( NULL ),
	m_gui( ScriptEngine::GetContext()->gui ),
	m_overlay( NULL ),
	m_mustBeClosed( false ),
	m_overlayName( p_name )
{
	if ( m_overlayName.empty() )
	{
		static int sl_number = 0;
		m_overlayName = wxT( "NewOverlay" );
		m_overlayName << sl_number++;
	}

	if ( m_parentOverlay != NULL )
	{
		m_parentGroup = m_parentOverlay->GetOwner();
		wxString l_strName( m_parentOverlay->GetName().c_str(), wxMBConvLibc() );
		TrollOverlay * l_parentTrollOverlay = g_mainFrame->GetCurrentProject()->GetMainScene()->GetOverlay( l_strName );

		if ( l_parentTrollOverlay->GetTextInfos() != NULL )
		{
			wxMessageBox( wxT( "Impossible de créer un overlay dans du texte" ), wxT( "Problème" ), wxID_OK );
			m_mustBeClosed = true;
		}
	}
	else
	{
		m_parentGroup = ScriptEngine::GetContext()->gui->CreateOverlayGroup( String( m_overlayName.char_str() ) );
	}

	wxSize l_size = GetClientSize();
	wxArrayString l_choices;
	_getOverlayTemplates( l_choices );
	wxComboBox * l_box = BuildComboBox( this, wxPoint( 100, 10 ), noIdTemplate, wxT( "Template" ), l_choices,
										( l_choices.size() > 0 ? l_choices[0] : wxT( "" ) ) );
	wxButton * l_button = new wxButton( this, noIdOK, wxT( "OK" ), wxPoint( 10, 370 ), wxSize( 100, 20 ), wxBORDER_SIMPLE );
	l_button = new wxButton( this, noIdCancel, wxT( "Cancel" ), wxPoint( 135, 370 ), wxSize( 100, 20 ), wxBORDER_SIMPLE );
	m_propertiesPanel = new wxPanel( this, wxID_ANY, wxPoint( 5, 35 ), wxSize( l_size.x - 10, l_size.y - 80 ) );
}



void NewOverlayFrame::_getOverlayTemplates( wxArrayString & p_array )
{
	p_array.clear();
	OverlayManager::TemplateIterator l_it = OverlayManager::getSingletonPtr()->getTemplateIterator();

	while ( l_it.hasMoreElements() )
	{
		p_array.push_back( wxString( l_it.getNext()->getName().c_str(), wxMBConvLibc() ) );
	}
}



void NewOverlayFrame::_onSelectTemplate( wxCommandEvent & p_event )
{
	EMOverlay * l_emov;
	String l_templateName = p_event.GetString().char_str().data();

	if ( m_overlay != NULL )
	{
		m_overlay->Reinitialise( wxString( l_templateName.c_str(), wxMBConvLibc() ) );
	}
	else
	{
//		std::cout << "NewOverlayFrame::_onSelectTemplate - " << l_templateName << "\n";
		TrollOverlay * l_parentTrollOverlay = NULL;
		wxArrayString l_list;
		g_mainFrame->GetCurrentProject()->GetMainScene()->GetOverlaysNames( l_list );
		wxString l_fileName;

		if ( l_list.size() > 0 )
		{
			l_fileName = g_mainFrame->GetCurrentProject()->GetMainScene()->GetOverlay( l_list[0] )->GetFileName();
		}
		else
		{
			l_fileName = g_mainFrame->GetCurrentProject()->GetMainScene()->GetSceneFile( 0 )->FileName;
		}

		if ( m_parentOverlay != NULL )
		{
			l_emov = m_parentOverlay->CreateChild( m_overlayName.char_str().data() );
			l_emov->Initialise( l_templateName );
			l_parentTrollOverlay = g_mainFrame->GetCurrentProject()->GetMainScene()->GetOverlay( wxString( m_parentOverlay->GetName().c_str(), wxMBConvLibc() ) );
		}
		else
		{
			l_emov = m_parentGroup->GetFirstElement();
			l_emov->Initialise( l_templateName );
		}

		m_overlay = new TrollOverlay( m_overlayName, l_parentTrollOverlay, l_fileName );
		m_overlay->SetMuseOverlay( l_emov );
	}

	m_overlay->BuildPanel( m_propertiesPanel, m_propertiesPanel->GetClientSize().x );
}



void NewOverlayFrame::_onOK( wxCommandEvent & p_event )
{
	if ( m_overlay != NULL )
	{
		g_mainFrame->GetCurrentProject()->GetMainScene()->AddOverlay( m_overlay );
		g_mainFrame->m_objectsList->AddOverlay( m_overlay, ( m_parentOverlay != NULL ? wxString( m_parentOverlay->GetName().c_str(), wxMBConvLibc() ) : wxT( "" ) ) );
		m_overlay->HideProperties();
	}

	Close();
	g_mainFrame->GetElypseCtrl()->SetNewOverlayFrame( NULL );
}



void NewOverlayFrame::_onCancel( wxCommandEvent & p_event )
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
	g_mainFrame->GetElypseCtrl()->SetNewOverlayFrame( NULL );
}


