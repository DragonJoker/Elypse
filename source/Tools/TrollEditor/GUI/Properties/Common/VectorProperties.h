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
#ifndef ___TROLL_GUI_VECTOR_PROPERTIES_H___
#define ___TROLL_GUI_VECTOR_PROPERTIES_H___

#include "AdditionalProperties.h"

TE_PG_NS_DECLARE_VARIANT_DATA( Ogre, Vector2 );
TE_PG_NS_DECLARE_VARIANT_DATA( Ogre, Vector3 );
TE_PG_NS_DECLARE_VARIANT_DATA( Ogre, Vector4 );

namespace Troll
{
	namespace GUI
	{
		namespace Properties
		{
			enum class Vector2Names
				: uint32_t
			{
				Point,
				Size,
				Count,
			};

			template< Vector2Names NameT >
			class wxVector2Property
				: public wxPGProperty
			{
				WX_PG_DECLARE_PROPERTY_CLASS( Vector2Property )

			public:
				wxVector2Property( wxString const & label = wxPG_LABEL, wxString const & name = wxPG_LABEL, Vector2 const & value = Vector2{} );
				virtual ~wxVector2Property();

				virtual wxVariant ChildChanged( wxVariant & thisValue, int childIndex, wxVariant & childValue ) const;
				virtual void RefreshChildren();

			protected:
				// I stands for internal
				inline void SetValueI( Vector2 const & value );
			};

			using wxPoint2DProperty = wxVector2Property< Vector2Names::Point >;
			using wxSize2DProperty = wxVector2Property< Vector2Names::Size >;

			template< Vector2Names NameT >
			inline wxPGProperty * CreateProperty( wxString const & p_name, Vector2 const & p_value )
			{
				auto l_property = new wxVector2Property< NameT >( p_name, wxPG_LABEL );
				l_property->SetValue( WXVARIANT( p_value ) );
				return l_property;
			}

			enum class Vector3Names
				: uint32_t
			{
				Point,
				Size,
				Spotlight,
				Count,
			};

			template< Vector3Names NameT >
			class wxVector3Property
				: public wxPGProperty
			{
				WX_PG_DECLARE_PROPERTY_CLASS( Vector3Property )

			public:
				wxVector3Property( wxString const & label = wxPG_LABEL, wxString const & name = wxPG_LABEL, Vector3 const & value = Vector3{} );
				virtual ~wxVector3Property();

				virtual wxVariant ChildChanged( wxVariant & thisValue, int childIndex, wxVariant & childValue ) const;
				virtual void RefreshChildren();

			protected:
				// I stands for internal
				inline void SetValueI( Vector3 const & value );
			};

			using wxPoint3DProperty = wxVector3Property< Vector3Names::Point >;
			using wxSize3DProperty = wxVector3Property< Vector3Names::Size >;
			using wxSpotlightProperty = wxVector3Property< Vector3Names::Spotlight >;

			template< Vector3Names NameT >
			inline wxPGProperty * CreateProperty( wxString const & p_name, Vector3 const & p_value )
			{
				auto l_property = new wxVector3Property< NameT >( p_name, wxPG_LABEL );
				l_property->SetValue( WXVARIANT( p_value ) );
				return l_property;
			}

			enum class Vector4Names
				: uint32_t
			{
				Point,
				Attenuation,
				Rectangle,
				Count,
			};

			template< Vector4Names NameT >
			class wxVector4Property
				: public wxPGProperty
			{
				WX_PG_DECLARE_PROPERTY_CLASS( Vector4Property )

			public:
				wxVector4Property( wxString const & label = wxPG_LABEL, wxString const & name = wxPG_LABEL, Vector4 const & value = Vector4{} );
				virtual ~wxVector4Property();

				virtual wxVariant ChildChanged( wxVariant & thisValue, int childIndex, wxVariant & childValue ) const;
				virtual void RefreshChildren();

			protected:
				// I stands for internal
				inline void SetValueI( Vector4 const & value );
			};

			using wxPoint4DProperty = wxVector4Property< Vector4Names::Point >;
			using wxAttenuationProperty = wxVector4Property< Vector4Names::Attenuation >;
			using wxRectangleProperty = wxVector4Property< Vector4Names::Rectangle >;

			template< Vector4Names NameT >
			inline wxPGProperty * CreateProperty( wxString const & p_name, Vector4 const & p_value )
			{
				auto l_property = new wxVector4Property< NameT >( p_name, wxPG_LABEL );
				l_property->SetValue( WXVARIANT( p_value ) );
				return l_property;
			}
		}
	}
}

#include "VectorProperties.inl"

#endif
