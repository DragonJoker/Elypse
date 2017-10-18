/*
This source file is part of Castor3D (http://castor3d.developpez.com/castor3d.htm)

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
#ifndef ___TROLL_GUI_QUATERNION_PROPERTIES_H___
#define ___TROLL_GUI_QUATERNION_PROPERTIES_H___

#include "AdditionalProperties.h"

TE_PG_NS_DECLARE_VARIANT_DATA( Ogre, Quaternion );

namespace Troll
{
	namespace GUI
	{
		namespace Properties
		{
			class wxQuaternionProperty
				: public wxPGProperty
			{
				WX_PG_DECLARE_PROPERTY_CLASS( QuaternionProperty )

			public:
				wxQuaternionProperty( wxString const & label = wxPG_LABEL, wxString const & name = wxPG_LABEL, Quaternion const & value = Quaternion{} );
				virtual ~wxQuaternionProperty();

				virtual wxVariant ChildChanged( wxVariant & thisValue, int childIndex, wxVariant & childValue ) const;
				virtual void RefreshChildren();

			protected:
				// I stands for internal
				inline void SetValueI( Quaternion const & value )
				{
					m_value = WXVARIANT( value );
				}
			};

			template<>
			inline wxPGProperty * CreateProperty< Quaternion >( wxString const & p_name, Quaternion const & p_value )
			{
				auto l_property = new wxQuaternionProperty( p_name, wxPG_LABEL );
				l_property->SetValue( WXVARIANT( p_value ) );
				return l_property;
			}
		}
	}
}

#endif
