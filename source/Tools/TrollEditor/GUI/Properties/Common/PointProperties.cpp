#include "PointProperties.h"

#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>

namespace Troll
{
	namespace GUI
	{
		namespace Properties
		{
			WX_PG_IMPLEMENT_PROPERTY_CLASS( wxPointProperty, wxPGProperty, wxPoint, wxPoint const &, TextCtrl )

				wxPointProperty::wxPointProperty( wxString const & label, wxString const & name, wxPoint const & value )
				: wxPGProperty( label, name )
			{
				SetValueI( value );
				AddPrivateChild( new wxIntProperty( wxT( "X" ), wxPG_LABEL, value.x ) );
				AddPrivateChild( new wxIntProperty( wxT( "Y" ), wxPG_LABEL, value.y ) );
			}

			wxPointProperty::~wxPointProperty()
			{
			}

			void wxPointProperty::RefreshChildren()
			{
				if ( GetChildCount() )
				{
					wxPoint const & point = wxPointRefFromVariant( m_value );
					Item( 0 )->SetValue( long( point.x ) );
					Item( 1 )->SetValue( long( point.y ) );
				}
			}

			wxVariant wxPointProperty::ChildChanged( wxVariant & thisValue, int childIndex, wxVariant & childValue )const
			{
				wxPoint & point = wxPointRefFromVariant( thisValue );
				int val = childValue.GetLong();

				switch ( childIndex )
				{
				case 0:
					point.x = val;
					break;

				case 1:
					point.y = val;
					break;
				}

				wxVariant newVariant;
				newVariant << point;
				return newVariant;
			}
		}
	}
}
