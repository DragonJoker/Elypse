/*
See LICENSE file in root folder
*/
#include "TrollEditorPch.h"

#include "GUI/TrollEditorGuiPrerequisites.h"

#include "Project/Object.h"

#include <GradientButton.h>

namespace Troll
{
	namespace GUI
	{
		wxPanel * BuildPanel( wxWindow * p_parent, wxPoint const & p_position, wxSize const & p_size, int p_id, const wxColour & p_colour, long p_style )
		{
			wxPanel * l_result = new wxPanel( p_parent, p_id, p_position, p_size, p_style );
			l_result->SetBackgroundColour( p_colour );
			return l_result;
		}

		wxTextCtrl * BuildTextCtrl( wxWindow * p_parent, wxPoint const & p_position, int p_id, wxString const & p_caption, wxString const & p_value )
		{
			wxSize l_size = p_parent->GetClientSize() - wxSize( 10, 0 );
			wxStaticText * l_text = new wxStaticText( p_parent, wxID_ANY, p_caption, wxPoint( 5, p_position.y + 3 ) );
			l_text->SetBackgroundColour( GuiCommon::PanelBackgroundColour );
			l_text->SetForegroundColour( GuiCommon::PanelForegroundColour );
			auto l_result = new wxTextCtrl( p_parent
				, p_id
				, p_value
				, p_position
				, wxSize( l_size.x - p_position.x, 20 )
				, wxBORDER_SIMPLE | wxTE_PROCESS_ENTER );
			l_result->SetBackgroundColour( GuiCommon::PanelBackgroundColour );
			l_result->SetForegroundColour( GuiCommon::PanelForegroundColour );
			return l_result;
		}

		wxTextCtrl * BuildTextCtrl( wxWindow * p_parent, wxPoint const & p_position, int p_id, wxString const & p_caption, float p_value )
		{
			wxString l_tmp;
			l_tmp.Printf( wxT( "%.2f" ), p_value );
			return BuildTextCtrl( p_parent, p_position, p_id, p_caption, l_tmp );
		}

		wxTextCtrl * BuildTextCtrl( wxWindow * p_parent, wxPoint const & p_position, int p_id, wxString const & p_caption, int p_value )
		{
			wxString l_tmp;
			l_tmp << p_value;
			return BuildTextCtrl( p_parent, p_position, p_id, p_caption, l_tmp );
		}

		wxComboBox * BuildComboBox( wxWindow * p_parent, wxPoint const & p_position, int p_id, wxString const & p_caption, const wxArrayString & p_values, wxString const & p_value )
		{
			wxSize l_size = p_parent->GetClientSize() - wxSize( 10, 0 );
			wxStaticText * l_text = new wxStaticText( p_parent, wxID_ANY, p_caption, wxPoint( 5, p_position.y + 3 ) );
			l_text->SetBackgroundColour( GuiCommon::PanelBackgroundColour );
			l_text->SetForegroundColour( GuiCommon::PanelForegroundColour );
			auto l_result = new wxComboBox( p_parent, p_id, p_value, p_position, wxSize( l_size.x - p_position.x, 20 ), p_values, wxBORDER_SIMPLE | wxCB_READONLY );
			l_result->SetBackgroundColour( GuiCommon::PanelBackgroundColour );
			l_result->SetForegroundColour( GuiCommon::PanelForegroundColour );
			return l_result;
		}

		wxListCtrl * BuildListBox( wxWindow * p_parent, wxPoint const & p_position, int p_id, const wxArrayString & p_captions, const wxArrayString & p_values )
		{
			wxSize l_size = p_parent->GetClientSize() - wxSize( 10, 0 );
			uint32_t l_linesNumber = uint32_t( p_values.GetCount() );
			wxListCtrl * l_captions = new wxListCtrl( p_parent, wxID_ANY, wxPoint( 5, p_position.y ), wxSize( l_size.x / 2, int( l_linesNumber * 20 ) ), wxLC_LIST | wxLC_SINGLE_SEL | wxBORDER_NONE );
			l_captions->SetBackgroundColour( GuiCommon::PanelBackgroundColour );
			l_captions->SetForegroundColour( GuiCommon::PanelForegroundColour );
			wxListCtrl * l_values = new wxListCtrl( p_parent, p_id, p_position, wxSize( l_size.x - p_position.x, int( l_linesNumber * 20 ) ), wxLC_LIST | wxLC_EDIT_LABELS | wxLC_SINGLE_SEL | wxBORDER_NONE );
			l_values->SetBackgroundColour( GuiCommon::PanelBackgroundColour );
			l_values->SetForegroundColour( GuiCommon::PanelForegroundColour );

			for ( uint32_t i = 0; i < l_linesNumber; i++ )
			{
				l_captions->InsertItem( i, p_captions[i] );
				l_values->InsertItem( i, p_values[i] );
			}

			return l_values;
		}

		wxButton * BuildButton( wxWindow * p_parent, wxPoint const & p_position, int p_id, wxString const & p_caption, wxString const & p_value )
		{
			wxSize l_size = p_parent->GetClientSize() - wxSize( 10, 0 );
			wxStaticText * l_text = new wxStaticText( p_parent, wxID_ANY, p_caption, wxPoint( 5, p_position.y + 3 ) );
			l_text->SetBackgroundColour( GuiCommon::PanelBackgroundColour );
			l_text->SetForegroundColour( GuiCommon::PanelForegroundColour );
			return new GuiCommon::GradientButton( p_parent, p_id, p_value, p_position, wxSize( l_size.x - p_position.x, 20 ), wxBORDER_SIMPLE | wxBU_LEFT );
		}

		wxTextCtrl * BuildTextCtrl( wxWindow * p_parent, wxPoint const & p_position, int p_id, String const & p_caption, String const & p_value )
		{
			return BuildTextCtrl( p_parent, p_position, p_id, make_wxString( p_caption ), make_wxString( p_value ) );
		}

		wxTextCtrl * BuildTextCtrl( wxWindow * p_parent, wxPoint const & p_position, int p_id, String const & p_caption, float p_value )
		{
			return BuildTextCtrl( p_parent, p_position, p_id, make_wxString( p_caption ), p_value );
		}

		wxTextCtrl * BuildTextCtrl( wxWindow * p_parent, wxPoint const & p_position, int p_id, String const & p_caption, int p_value )
		{
			return BuildTextCtrl( p_parent, p_position, p_id, make_wxString( p_caption ), p_value );
		}

		wxButton * BuildButton( wxWindow * p_parent, wxPoint const & p_position, int p_id, String const & p_caption, String const & p_value )
		{
			return BuildButton( p_parent, p_position, p_id, make_wxString( p_caption ), make_wxString( p_value ) );
		}

		wxArrayString Split( wxString const & p_toSplit, wxString const & p_delims, uint32_t p_maxSplits )
		{
			wxArrayString l_toReturn;

			if ( p_toSplit.empty() || p_delims.empty() )
			{
				return l_toReturn;
			}

			l_toReturn.reserve( p_maxSplits ? p_maxSplits + 1 : 10 );
			size_t l_numSplits = 0;
			size_t l_start = 0;
			size_t l_pos = 0;

			do
			{
				l_pos = p_toSplit.find_first_of( p_delims, l_start );

				if ( l_pos == l_start )
				{
					l_start = l_pos + 1;
				}
				else if ( l_pos == wxString::npos || ( p_maxSplits && l_numSplits == p_maxSplits ) )
				{
					l_toReturn.push_back( p_toSplit.substr( l_start ) );
					return l_toReturn;
				}
				else
				{
					l_toReturn.push_back( p_toSplit.substr( l_start, l_pos - l_start ) );
					l_start = l_pos + 1;
				}

				l_start = p_toSplit.find_first_not_of( p_delims, l_start );
				++l_numSplits;
			}
			while ( l_pos != wxString::npos );

			return l_toReturn;
		}

		RecursiveTreeItemObject * RecursiveTreeItemObject::RecursiveIsIn( wxString const & p_name )
		{
			if ( ItemObject.Object->GetObjectName() == p_name )
			{
				return this;
			}

			RecursiveTreeItemObjectPtrMap::iterator l_it = Childs.begin();
			RecursiveTreeItemObject * l_item = nullptr;

			while ( l_it != Childs.end() )
			{
				if ( ( l_item = l_it->second->RecursiveIsIn( p_name ) ) != nullptr )
				{
					return l_item;
				}

				++l_it;
			}

			return l_item;
		}

		RecursiveTreeItemObject * RecursiveTreeItemObject::RecursiveIsIn( wxTreeItemIdValue p_index )
		{
			if ( ItemObject.ItemID == p_index )
			{
				return this;
			}

			RecursiveTreeItemObjectPtrMap::iterator l_it = Childs.find( p_index );

			if ( l_it != Childs.end() )
			{
				return l_it->second;
			}

			l_it = Childs.begin();
			RecursiveTreeItemObject * l_item = nullptr;

			while ( l_it != Childs.end() )
			{
				if ( ( l_item = l_it->second->RecursiveIsIn( p_index ) ) != nullptr )
				{
					return l_item;
				}

				++l_it;
			}

			return l_item;
		}

		RecursiveTreeItemObject::~RecursiveTreeItemObject()
		{
			General::Utils::map::deleteAll( Childs );
		}
	}
}
