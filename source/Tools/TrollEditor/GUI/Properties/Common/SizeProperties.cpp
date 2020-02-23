#include "SizeProperties.h"

#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>

namespace Troll
{
	namespace GUI
	{
		namespace Properties
		{
			WX_PG_IMPLEMENT_PROPERTY_CLASS( wxSizeProperty, wxPGProperty, wxSize, wxSize const &, TextCtrl )

			wxSizeProperty::wxSizeProperty( wxString const & label, wxString const & name, wxSize const & value )
				: wxPGProperty( label, name )
			{
				SetValueI( value );
				AddPrivateChild( new wxIntProperty( _( "Width" ), wxPG_LABEL, value.x ) );
				AddPrivateChild( new wxIntProperty( _( "Height" ), wxPG_LABEL, value.y ) );
			}

			wxSizeProperty::~wxSizeProperty()
			{
			}

			void wxSizeProperty::RefreshChildren()
			{
				if ( GetChildCount() )
				{
					wxSize const & size = wxSizeRefFromVariant( m_value );
					Item( 0 )->SetValue( long( size.x ) );
					Item( 1 )->SetValue( long( size.y ) );
				}
			}

			wxVariant wxSizeProperty::ChildChanged( wxVariant & thisValue, int childIndex, wxVariant & childValue )const
			{
				wxSize & size = wxSizeRefFromVariant( thisValue );
				int val = childValue.GetLong();

				switch ( childIndex )
				{
				case 0:
					size.x = val;
					break;

				case 1:
					size.y = val;
					break;
				}

				wxVariant newVariant;
				newVariant << size;
				return newVariant;
			}
		}
	}
}
