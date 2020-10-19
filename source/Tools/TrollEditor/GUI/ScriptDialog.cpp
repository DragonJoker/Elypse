/*
See LICENSE file in root folder
*/
#include "TrollEditorPch.h"

#include "TrollEditorApplication.h"
#include "Elypse/TrollScriptCompiler.h"
#include "GUI/LanguageParser.h"
#include "GUI/MainFrame.h"
#include "GUI/ScriptDialog.h"
#include "GUI/StcTextEditor.h"

#include <GradientButton.h>


namespace Troll::GUI
{
	BEGIN_EVENT_TABLE( ScriptDialog, wxDialog )
		EVT_BUTTON( wxID_OK, ScriptDialog::OnOK )
		EVT_BUTTON( wxID_CANCEL, ScriptDialog::OnCancel )
	END_EVENT_TABLE()

	ScriptDialog::ScriptDialog( wxWindow * parent
		, wxString const & title
		, wxString const & caption
		, wxString const & value )
		: wxDialog{ parent, wxID_ANY, title, wxDefaultPosition, wxSize{ 700, 700 } }
	{
		SetMinClientSize( wxSize{ 700, 500 } );
		SetBackgroundColour( GuiCommon::PanelBackgroundColour );
		SetForegroundColour( GuiCommon::PanelForegroundColour );

		wxBoxSizer * sizerAll = new wxBoxSizer{ wxVERTICAL };
		auto staticTxt = new wxStaticText{ this, wxID_ANY, caption };
		sizerAll->Add( staticTxt, wxSizerFlags().Centre().Border( wxBOTTOM | wxTOP, 10 ) );
		staticTxt->SetBackgroundColour( GuiCommon::PanelBackgroundColour );
		staticTxt->SetForegroundColour( GuiCommon::PanelForegroundColour );

		LanguageFileParser parser( &m_stcContext );
		parser.ParseFile( wxGetApp().GetMainFrame()->GetAppDir() + wxFileName::GetPathSeparator() + wxT( "EMSCRIPT.lang" ) );

		m_script = new wxStcTextEditor{ nullptr, m_stcContext, this, wxID_ANY, wxDefaultPosition, wxDefaultSize };
		m_script->SetMinSize( wxSize{ 680, 420 } );
		m_script->SetText( value );
		m_script->SetBackgroundColour( GuiCommon::PanelBackgroundColour );
		m_script->SetForegroundColour( GuiCommon::PanelForegroundColour );

		ElypseRW::TrollScriptCompiler compiler{ &m_stcContext, "", false };
		compiler.Initialise( m_script );
		compiler.CompileScript( make_string( value ) );

		sizerAll->Add( m_script, wxSizerFlags( 1 ).Expand().Border( wxBOTTOM | wxLEFT | wxRIGHT, 10 ) );

		wxBoxSizer * sizerButtons = new wxBoxSizer{ wxHORIZONTAL };
		sizerButtons->Add( new GuiCommon::GradientButton{ this, wxID_OK, TE_OK, wxDefaultPosition, wxSize{ 75, 25 }, wxBORDER_SIMPLE }, wxSizerFlags( 1 ).Border( wxLEFT, 10 ) );
		sizerButtons->AddStretchSpacer();
		sizerButtons->Add( new GuiCommon::GradientButton{ this, wxID_CANCEL, TE_CANCEL, wxDefaultPosition, wxSize( 75, 25 ), wxBORDER_SIMPLE }, wxSizerFlags( 1 ).Border( wxRIGHT, 10 ) );
		sizerAll->Add( sizerButtons, wxSizerFlags().Expand().Border( wxBOTTOM, 10 ) );

		sizerAll->SetSizeHints( this );
		SetSizer( sizerAll );
	}

	void ScriptDialog::OnOK( wxCommandEvent & event )
	{
		EndModal( wxID_OK );
	}

	void ScriptDialog::OnCancel( wxCommandEvent & event )
	{
		EndModal( wxID_CANCEL );
	}
}
