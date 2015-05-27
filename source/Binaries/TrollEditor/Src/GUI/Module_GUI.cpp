
#include "PrecompiledHeader.h"

#include "GUI/Module_GUI.h"
#include "Project/Object.h"



wxString Troll::GUI :: make_wxString( String const & p_value )
{
	return wxString( p_value.c_str(), wxMBConvLibc() );
}



float Troll::GUI :: GetReal( const wxString & p_value, float & p_res )
{
	double l_res = 0.0;
	wxString l_value = p_value;
	l_value.Replace( wxT( "," ), wxT( "." ) );

	if ( l_value.ToDouble( & l_res ) )
	{
		p_res = static_cast <float>( l_res );
		return true;
	}

	wxMessageBox( wxT( "Entrez une valeur numérique flottante" ), wxT( "Erreur de conversion" ) );
	return false;
}



int Troll::GUI :: GetInt( const wxString & p_value, int & p_res )
{
	long l_res = 0;

	if ( p_value.ToLong( & l_res ) )
	{
		p_res = static_cast <int>( l_res );
		return true;
	}

	wxMessageBox( wxT( "Entrez une valeur numérique entière" ), wxT( "Erreur de conversion" ) );
	return false;
}




float Troll::GUI :: GetReal( const String & p_value, float & p_res )
{
	return GetReal( wxString( p_value.c_str(), wxMBConvLibc() ), p_res );
}



int Troll::GUI :: GetInt( const String & p_value, int & p_res )
{
	return GetInt( wxString( p_value.c_str(), wxMBConvLibc() ), p_res );
}



wxPanel * Troll::GUI :: BuildPanel( wxWindow * p_parent, const wxPoint & p_position, const wxSize & p_size,
									int p_id, const wxColour & p_colour, long p_style )
{
	wxPanel * l_result = new wxPanel( p_parent, p_id, p_position, p_size, p_style );
	l_result->SetBackgroundColour( p_colour );
	return l_result;
}



wxTextCtrl * Troll::GUI :: BuildTextCtrl( wxWindow * p_parent, const wxPoint & p_position, int p_id,
		const wxString & p_caption, const wxString & p_value )
{
	wxSize l_size = p_parent->GetClientSize() - wxSize( 10, 0 );
	wxStaticText * l_text = new wxStaticText( p_parent, wxID_ANY, p_caption, wxPoint( 5, p_position.y + 3 ) );
	return new wxTextCtrl( p_parent, p_id, p_value, p_position, wxSize( l_size.x - p_position.x, 20 ),
						   wxBORDER_SIMPLE | wxTE_PROCESS_ENTER );
}



wxTextCtrl * Troll::GUI :: BuildTextCtrl( wxWindow * p_parent, const wxPoint & p_position, int p_id,
		const wxString & p_caption, float p_value )
{
	wxString l_tmp;
	l_tmp.Printf( wxT( "%.2f" ), p_value );
	return BuildTextCtrl( p_parent, p_position, p_id, p_caption, l_tmp );
}



wxTextCtrl * Troll::GUI :: BuildTextCtrl( wxWindow * p_parent, const wxPoint & p_position, int p_id,
		const wxString & p_caption, int p_value )
{
	wxString l_tmp;
	l_tmp << p_value;
	return BuildTextCtrl( p_parent, p_position, p_id, p_caption, l_tmp );
}



wxComboBox * Troll::GUI :: BuildComboBox( wxWindow * p_parent, const wxPoint & p_position, int p_id,
		const wxString & p_caption, const wxArrayString & p_values,
		const wxString & p_value )
{
	wxSize l_size = p_parent->GetClientSize() - wxSize( 10, 0 );
	wxStaticText * l_text = new wxStaticText( p_parent, wxID_ANY, p_caption, wxPoint( 5, p_position.y + 3 ) );
	return new wxComboBox( p_parent, p_id, p_value, p_position, wxSize( l_size.x - p_position.x, 20 ), p_values, wxBORDER_SIMPLE | wxCB_READONLY );
}



wxListCtrl * Troll::GUI :: BuildListBox( wxWindow * p_parent, const wxPoint & p_position, int p_id,
		const wxArrayString & p_captions, const wxArrayString & p_values )
{
	wxSize l_size = p_parent->GetClientSize() - wxSize( 10, 0 );
	unsigned int l_linesNumber = p_values.GetCount();
	wxListCtrl * l_captions = new wxListCtrl( p_parent, wxID_ANY, wxPoint( 5, p_position.y ), wxSize( l_size.x / 2, l_linesNumber * 20 ), wxLC_LIST | wxLC_SINGLE_SEL | wxBORDER_NONE );
	wxListCtrl * l_values = new wxListCtrl( p_parent, p_id, p_position, wxSize( l_size.x - p_position.x, l_linesNumber * 20 ), wxLC_LIST | wxLC_EDIT_LABELS | wxLC_SINGLE_SEL | wxBORDER_NONE );

	for ( unsigned int i = 0 ; i < l_linesNumber ; i++ )
	{
		l_captions->InsertItem( i, p_captions[i] );
		l_values->InsertItem( i, p_values[i] );
	}

	return l_values;
}



wxButton * Troll::GUI :: BuildButton( wxWindow * p_parent, const wxPoint & p_position, int p_id,
									  const wxString & p_caption, const wxString & p_value )
{
	wxSize l_size = p_parent->GetClientSize() - wxSize( 10, 0 );
	wxStaticText * l_text = new wxStaticText( p_parent, wxID_ANY, p_caption, wxPoint( 5, p_position.y + 3 ) );
	wxButton * l_button = new wxButton( p_parent, p_id, p_value, p_position, wxSize( l_size.x - p_position.x, 20 ), wxBORDER_SIMPLE | wxBU_LEFT );
//	l_button->SetBackgroundColour( wxColour( 255, 255, 255));
	return l_button;
}



wxTextCtrl * Troll::GUI :: BuildTextCtrl( wxWindow * p_parent, const wxPoint & p_position, int p_id,
		const String & p_caption, const String & p_value )
{
	return BuildTextCtrl( p_parent, p_position, p_id, make_wxString( p_caption ), make_wxString( p_value ) );
}



wxTextCtrl * Troll::GUI :: BuildTextCtrl( wxWindow * p_parent, const wxPoint & p_position, int p_id,
		const String & p_caption, float p_value )
{
	return BuildTextCtrl( p_parent, p_position, p_id, make_wxString( p_caption ), p_value );
}



wxTextCtrl * Troll::GUI :: BuildTextCtrl( wxWindow * p_parent, const wxPoint & p_position, int p_id,
		const String & p_caption, int p_value )
{
	return BuildTextCtrl( p_parent, p_position, p_id, make_wxString( p_caption ), p_value );
}



wxButton * Troll::GUI :: BuildButton( wxWindow * p_parent, const wxPoint & p_position, int p_id,
									  const String & p_caption, const String & p_value )
{
	return BuildButton( p_parent, p_position, p_id, make_wxString( p_caption ), make_wxString( p_value ) );
}



wxArrayString Troll::GUI :: Split( const wxString & p_toSplit, const wxString & p_delims, unsigned int p_maxSplits )
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
		++ l_numSplits;
	}
	while ( l_pos != wxString::npos );

	return l_toReturn;
}



Troll::GUI::RecursiveTreeItemObject * Troll::GUI::RecursiveTreeItemObject :: RecursiveIsIn( const wxString & p_name )
{
//	std::cout << "TreeItemObject :: RecursiveIsIn - Searching " << p_name << " in " << Object->GetName() << "\n";
	if ( ItemObject.Object->GetName() == p_name )
	{
		return this;
	}

	RecursiveTreeItemObjectPtrMap::iterator l_it = Childs.begin();
	RecursiveTreeItemObject * l_item = NULL;

	while ( l_it != Childs.end() )
	{
//		std::cout << "TreeItemObject :: RecursiveIsIn - " << l_it->first << "\n";
		if ( ( l_item = l_it->second->RecursiveIsIn( p_name ) ) != NULL )
		{
			return l_item;
		}

		++l_it;
	}

	return l_item;
}



Troll::GUI::RecursiveTreeItemObject * Troll::GUI::RecursiveTreeItemObject :: RecursiveIsIn( wxTreeItemIdValue p_index )
{
//	std::cout << "TreeItemObject :: RecursiveIsIn - Searching " << p_name << " in " << Object->GetName() << "\n";
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
	RecursiveTreeItemObject * l_item = NULL;

	while ( l_it != Childs.end() )
	{
//		std::cout << "TreeItemObject :: RecursiveIsIn - " << l_it->first << "\n";
		if ( ( l_item = l_it->second->RecursiveIsIn( p_index ) ) != NULL )
		{
			return l_item;
		}

		++l_it;
	}

	return l_item;
}



Troll::GUI::RecursiveTreeItemObject :: ~RecursiveTreeItemObject()
{
	General::Utils::map::deleteAll( Childs );
}


