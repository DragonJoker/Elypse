#include "PropertyDialog.h"

#include "Properties/Common/ObjectProperty.h"
#include "Properties/Common/PropertiesHolder.h"

#include <GradientButton.h>

namespace Troll
{
	namespace GUI
	{
		BEGIN_EVENT_TABLE( PropertyDialog, wxDialog )
			EVT_BUTTON( wxID_OK, PropertyDialog::OnOK )
			EVT_BUTTON( wxID_CANCEL, PropertyDialog::OnCancel )
		END_EVENT_TABLE()

		PropertyDialog::PropertyDialog( wxWindow * p_parent, wxString const & p_title, Properties::ObjectProperty * p_props, wxPoint const & p_position, wxSize const & p_size )
			: wxDialog{ p_parent, wxID_ANY, p_title, p_position, p_size }
			, m_props{ p_props }
		{
			auto l_size = GetClientSize();
			SetBackgroundColour( GuiCommon::PanelBackgroundColour );
			SetForegroundColour( GuiCommon::PanelForegroundColour );

			wxBoxSizer * l_sizerAll = new wxBoxSizer{ wxVERTICAL };
			m_holder = new Properties::PropertiesHolder{ this };
			m_holder->SetPropertyData( p_props );
			m_holder->SetSize( p_size - wxSize{ 0, 100 } );
			l_sizerAll->Add( m_holder, wxSizerFlags( 1 ).Expand().Border( wxTOP | wxBOTTOM | wxLEFT | wxRIGHT, 10 ) );

			wxBoxSizer * l_sizerButtons = new wxBoxSizer{ wxHORIZONTAL };
			l_sizerButtons->Add( new GuiCommon::GradientButton{ this, wxID_OK, TE_OK, wxDefaultPosition, wxSize{ 75, 25 }, wxBORDER_SIMPLE }, wxSizerFlags( 1 ).Border( wxLEFT, 10 ) );
			l_sizerButtons->AddStretchSpacer();
			l_sizerButtons->Add( new GuiCommon::GradientButton{ this, wxID_CANCEL, TE_CANCEL, wxDefaultPosition, wxSize( 75, 25 ), wxBORDER_SIMPLE }, wxSizerFlags( 1 ).Border( wxRIGHT, 10 ) );
			l_sizerAll->Add( l_sizerButtons, wxSizerFlags().Expand().Border( wxBOTTOM, 10 ) );

			l_sizerAll->SetMinSize( l_size );
			l_sizerAll->SetSizeHints( this );
			SetSizer( l_sizerAll );
		}

		PropertyDialog::~PropertyDialog()
		{
			delete m_props;
		}

		void PropertyDialog::OnOK( wxCommandEvent & p_event )
		{
			if ( m_props->OnValidate( this ) )
			{
				EndModal( wxID_OK );
			}
		}

		void PropertyDialog::OnCancel( wxCommandEvent & p_event )
		{
			m_props->OnCancel( this );
			EndModal( wxID_CANCEL );
		}
	}
}
