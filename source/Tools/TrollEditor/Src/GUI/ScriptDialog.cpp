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
#include "TrollEditorPch.h"

#include "GUI/ScriptDialog.h"

#include "GUI/StcTextEditor.h"

#include <GradientButton.h>

namespace Troll
{
	namespace GUI
	{
		BEGIN_EVENT_TABLE( ScriptDialog, wxDialog )
			EVT_BUTTON( wxID_OK, ScriptDialog::OnOK )
			EVT_BUTTON( wxID_CANCEL, ScriptDialog::OnCancel )
		END_EVENT_TABLE()

		ScriptDialog::ScriptDialog( wxWindow * p_parent, wxString const & p_title, wxString const & p_caption, wxString const & p_value )
			: wxDialog{ p_parent, wxID_ANY, p_title, wxDefaultPosition, wxSize{ 250, 250 } }
		{
			SetBackgroundColour( GuiCommon::PanelBackgroundColour );
			SetForegroundColour( GuiCommon::PanelForegroundColour );

			wxBoxSizer * l_sizerAll = new wxBoxSizer{ wxVERTICAL };
			auto l_static = new wxStaticText{ this, wxID_ANY, p_caption };
			l_sizerAll->Add( l_static, wxSizerFlags().Centre().Border( wxBOTTOM | wxTOP, 10 ) );
			l_static->SetBackgroundColour( GuiCommon::PanelBackgroundColour );
			l_static->SetForegroundColour( GuiCommon::PanelForegroundColour );

			m_script = new wxTextCtrl{ this, wxID_ANY, p_value, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE };
			m_script->SetBackgroundColour( GuiCommon::PanelBackgroundColour );
			m_script->SetForegroundColour( GuiCommon::PanelForegroundColour );
			l_sizerAll->Add( m_script, wxSizerFlags( 1 ).Expand().Border( wxBOTTOM | wxLEFT | wxRIGHT, 10 ) );

			wxBoxSizer * l_sizerButtons = new wxBoxSizer{ wxHORIZONTAL };
			l_sizerButtons->Add( new GuiCommon::GradientButton{ this, wxID_OK, TE_OK, wxDefaultPosition, wxSize{ 75, 25 }, wxBORDER_SIMPLE }, wxSizerFlags( 1 ).Border( wxLEFT, 10 ) );
			l_sizerButtons->AddStretchSpacer();
			l_sizerButtons->Add( new GuiCommon::GradientButton{ this, wxID_CANCEL, TE_CANCEL, wxDefaultPosition, wxSize( 75, 25 ), wxBORDER_SIMPLE }, wxSizerFlags( 1 ).Border( wxRIGHT, 10 ) );
			l_sizerAll->Add( l_sizerButtons, wxSizerFlags().Expand().Border( wxBOTTOM, 10 ) );

			l_sizerAll->SetSizeHints( this );
			SetSizer( l_sizerAll );
		}

		void ScriptDialog::OnOK( wxCommandEvent & p_event )
		{
			EndModal( wxID_OK );
		}

		void ScriptDialog::OnCancel( wxCommandEvent & p_event )
		{
			EndModal( wxID_CANCEL );
		}
	}
}
