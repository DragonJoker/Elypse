#include "QuaternionProperties.h"

#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>

WX_PG_IMPLEMENT_VARIANT_DATA_EXPORTED_DUMMY_EQ( Quaternion, wxEMPTY_PARAMETER_VALUE )

namespace Troll
{
	namespace GUI
	{
		namespace Properties
		{
			WX_PG_IMPLEMENT_PROPERTY_CLASS( wxQuaternionProperty, wxPGProperty, Quaternion, Quaternion const &, TextCtrl );

			wxQuaternionProperty::wxQuaternionProperty( wxString const & label, wxString const & name, Quaternion const & value )
				: wxPGProperty( label, name )
			{
				SetValueI( value );
				Vector3 l_axis;
				Degree l_angle;
				value.ToAngleAxis( l_angle, l_axis );
				AddPrivateChild( new wxFloatProperty( _( "Axis X" ), wxPG_LABEL, l_axis.x ) );
				AddPrivateChild( new wxFloatProperty( _( "Axis Y" ), wxPG_LABEL, l_axis.y ) );
				AddPrivateChild( new wxFloatProperty( _( "Axis Z" ), wxPG_LABEL, l_axis.z ) );
				AddPrivateChild( new wxFloatProperty( _( "Angle" ), wxPG_LABEL, l_angle.valueDegrees() ) );
			}

			wxQuaternionProperty::~wxQuaternionProperty()
			{
			}

			void wxQuaternionProperty::RefreshChildren()
			{
				if ( GetChildCount() )
				{
					Quaternion const & l_quat = QuaternionRefFromVariant( m_value );
					Vector3 l_axis;
					Degree l_angle;
					l_quat.ToAngleAxis( l_angle, l_axis );
					Item( 0 )->SetValue( l_axis.x );
					Item( 1 )->SetValue( l_axis.y );
					Item( 2 )->SetValue( l_axis.z );
					Item( 3 )->SetValue( l_angle.valueDegrees() );
				}
			}

			wxVariant wxQuaternionProperty::ChildChanged( wxVariant & thisValue, int childIndex, wxVariant & childValue )const
			{
				Quaternion & l_quat = QuaternionRefFromVariant( thisValue );
				Vector3 l_axis;
				Degree l_angle;
				l_quat.ToAngleAxis( l_angle, l_axis );
				auto val = Real( childValue.GetDouble() );

				switch ( childIndex )
				{
				case 0:
					l_axis.x = val;
					break;

				case 1:
					l_axis.y = val;
					break;

				case 2:
					l_axis.y = val;
					break;

				case 3:
					l_angle = val;
					break;
				}

				l_quat.FromAngleAxis( l_angle, l_axis );
				wxVariant newVariant;
				newVariant << l_quat;
				return newVariant;
			}
		}
	}
}
