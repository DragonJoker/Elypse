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
#ifndef ___TROLL_GUI_PROPERTIES_ADDITIONAL_PROPERTIES_H___
#define ___TROLL_GUI_PROPERTIES_ADDITIONAL_PROPERTIES_H___

#include "GUI/Properties/Module_Properties.h"

#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>

#define TE_PG_NS_DECLARE_VARIANT_DATA_EXPORTED( namspace, classname, expdecl )\
	namspace::classname & operator<<( namspace::classname & object, const wxVariant & variant );\
	wxVariant & operator<<( wxVariant & variant, const namspace::classname & object );\
	const namspace::classname & classname##RefFromVariant( const wxVariant & variant );\
	namspace::classname & classname##RefFromVariant( wxVariant & variant );\
	template<> inline wxVariant WXVARIANT( const namspace::classname & value )\
	{\
		wxVariant variant;\
		variant << value;\
		return variant;\
	}\
	extern const char* classname##_VariantType;

#define TE_PG_NS_DECLARE_VARIANT_DATA( namspace, classname )\
	TE_PG_NS_DECLARE_VARIANT_DATA_EXPORTED( namspace, classname, wxEMPTY_PARAMETER_VALUE )

// Implements sans constructor function. Also, first arg is class name, not
// property name.
#define TE_PG_IMPLEMENT_PROPERTY_CLASS_PLAIN( PROPNAME, T, EDITOR )\
	template<> const wxPGEditor* PROPNAME::DoGetEditorClass()const\
	{\
		return wxPGEditor_##EDITOR;\
	}

// common part of the macros below
#define TE_IMPLEMENT_CLASS_COMMON( name, basename, baseclsinfo2, func )\
	template<>\
	wxClassInfo name::ms_classInfo( wxT( #name ),\
						&basename::ms_classInfo,\
						baseclsinfo2,\
						int( sizeof( name ) ),\
						func );\
	template<>\
	wxClassInfo * name::GetClassInfo() const\
	{\
		return &name::ms_classInfo;\
	}

#define TE_IMPLEMENT_CLASS_COMMON1( name, basename, func )\
	TE_IMPLEMENT_CLASS_COMMON( name, basename, nullptr, func )

// Single inheritance with one base class
#define TE_IMPLEMENT_DYNAMIC_CLASS( name, basename )\
	template<>\
	wxObject* name::wxCreateObject()\
	{\
		return new name;\
	}\
	TE_IMPLEMENT_CLASS_COMMON1( name, basename, name::wxCreateObject )

//
// Property class implementation helper macros.
//
#define TE_PG_IMPLEMENT_PROPERTY_CLASS( NAME, UPCLASS, T, T_AS_ARG, EDITOR )\
	TE_IMPLEMENT_DYNAMIC_CLASS( NAME, UPCLASS )\
	TE_PG_IMPLEMENT_PROPERTY_CLASS_PLAIN( NAME, T, EDITOR )

// Add getter (ie. classname << variant) separately to allow
// custom implementations.
#define TE_PG_IMPLEMENT_ALIGNED_VARIANT_DATA_EXPORTED_NO_EQ_NO_GETTER( classname, alignment, expdecl ) \
	const char* classname##_VariantType = #classname; \
	class classname##VariantData\
		: public wxVariantData\
		, public Castor::Aligned< alignment >\
	{ \
	public:\
		classname##VariantData() {} \
		classname##VariantData( const classname & value ) { m_value = value; } \
		classname &GetValue() { return m_value; } \
		const classname &GetValue() const { return m_value; } \
		virtual bool Eq( wxVariantData & data ) const; \
		virtual wxString GetType() const; \
		virtual wxVariantData* Clone() const { return new classname##VariantData( m_value ); } \
	protected:\
		classname m_value; \
	};\
	\
	wxString classname##VariantData::GetType() const\
	{\
		return wxS( #classname );\
	}\
	\
	expdecl wxVariant & operator<<( wxVariant & variant, const classname & value )\
	{\
		classname##VariantData * data = new classname##VariantData( value );\
		variant.SetData( data );\
		return variant;\
	} \
	expdecl classname & classname##RefFromVariant( wxVariant & variant ) \
	{ \
		wxASSERT_MSG( variant.GetType() == wxS( #classname ), \
					  wxString::Format( "Variant type should have been '%s'" \
									   "instead of '%s'", \
									   wxS( #classname ), \
									   variant.GetType().c_str() ) ); \
		classname##VariantData *data = \
			( classname##VariantData * ) variant.GetData(); \
		return data->GetValue();\
	} \
	expdecl const classname& classname##RefFromVariant( const wxVariant& variant ) \
	{ \
		wxASSERT_MSG( variant.GetType() == wxS( #classname ), \
					  wxString::Format( "Variant type should have been '%s'" \
									   "instead of '%s'", \
									   wxS( #classname ), \
									   variant.GetType().c_str() ) ); \
		classname##VariantData * data = \
			( classname##VariantData * ) variant.GetData(); \
		return data->GetValue();\
	}

#define TE_PG_IMPLEMENT_ALIGNED_VARIANT_DATA_GETTER( classname, expdecl ) \
	expdecl classname & operator<<( classname &value, const wxVariant & variant )\
	{\
		wxASSERT( variant.GetType() == #classname );\
		\
		classname##VariantData * data = ( classname##VariantData * ) variant.GetData();\
		value = data->GetValue();\
		return value;\
	}

// with Eq() implementation that always returns false
#define TE_PG_IMPLEMENT_ALIGNED_VARIANT_DATA_EXPORTED_DUMMY_EQ( classname, alignment, expdecl ) \
	TE_PG_IMPLEMENT_ALIGNED_VARIANT_DATA_EXPORTED_NO_EQ_NO_GETTER( classname, alignment, wxEMPTY_PARAMETER_VALUE expdecl ) \
	TE_PG_IMPLEMENT_ALIGNED_VARIANT_DATA_GETTER( classname, wxEMPTY_PARAMETER_VALUE expdecl ) \
	\
	bool classname##VariantData::Eq( wxVariantData & WXUNUSED( data ) ) const \
	{\
		return false; \
	}

namespace Troll
{
	namespace GUI
	{
		namespace Properties
		{
			template< typename T > T GetValue( wxVariant const & p_variant );
			template< typename T > wxVariant SetValue( T const & p_value );

			typedef bool ( wxEvtHandler::*ButtonEventMethod )( wxPGProperty * );
#			define ButtonEventHandler( value ) static_cast< ButtonEventMethod >( &value )

			class ButtonData
				: public wxClientData
			{
			public:
				ButtonData( ButtonEventMethod p_method, wxEvtHandler * p_handler );
				bool Call( wxPGProperty * property );

			private:
				ButtonEventMethod m_method;
				wxEvtHandler * m_handler;
			};

			class ButtonEventEditor
				: public wxPGEditor
			{
			protected:
				virtual wxPGWindowList CreateControls( wxPropertyGrid * p_propgrid, wxPGProperty * p_property, wxPoint const & p_pos, wxSize const & p_size )const;
				virtual void UpdateControl( wxPGProperty * property, wxWindow * ctrl ) const;
				virtual bool OnEvent( wxPropertyGrid * p_propgrid, wxPGProperty * p_property, wxWindow * p_wnd_primary, wxEvent & p_event )const;
			};

			wxPGProperty * CreateProperty( wxString const & p_name, wxPGChoices p_choices, wxString const & p_value );
			wxPGProperty * CreateProperty( wxString const & p_name, wxString const & p_value, ButtonEventMethod p_method, wxEvtHandler * p_handler, wxPGEditor * p_editor );

			template< typename ValueT >
			inline wxPGProperty * CreateProperty( wxString const & p_name, ValueT const & p_value );

			template< typename PropertyType >
			inline PropertyType * CreateProperty( wxString const & p_name, wxVariant && p_value, wxString const & p_help )
			{
				auto l_return = new PropertyType( p_name );
				l_return->SetValue( p_value );
				l_return->SetHelpString( p_help );
				return l_return;
			}
			/**
			 *\~english
			 *\brief		Builds a wxArrayString from an array of wxString.
			 *\param[in]	p_values	The array.
			 *\return		The wxArrayString.
			 *\~french
			 *\brief		Construit un wxArrayString à partir d'un array de wxString.
			 *\param[in]	p_values	L'array.
			 *\return		Le wxArrayString.
			 */
			template< size_t Count >
			inline wxArrayString make_wxArrayString( std::array< wxString, Count > const & p_values );
			/**
			 *\~english
			 *\brief		Builds a wxArrayString from an vector of wxString.
			 *\param[in]	p_values	The array.
			 *\return		The wxArrayString.
			 *\~french
			 *\brief		Construit un wxArrayString à partir d'un vector de wxString.
			 *\param[in]	p_values	L'array.
			 *\return		Le wxArrayString.
			 */
			inline wxArrayString make_wxArrayString( std::vector< wxString > const & p_values );
			/**
			 *\~english
			 *\brief		Builds a wxArrayString from a StringArray.
			 *\param[in]	p_values	The array.
			 *\return		The wxArrayString.
			 *\~french
			 *\brief		Construit un wxArrayString à partir d'un StringArray.
			 *\param[in]	p_values	L'array.
			 *\return		Le wxArrayString.
			 */
			inline wxArrayString make_wxArrayString( StringArray const & p_values );
			/**
			 *\~english
			 *\brief		Builds a wxArrayInt from an array of int.
			 *\param[in]	p_values	The array.
			 *\return		The wxArrayInt.
			 *\~french
			 *\brief		Construit un wxArrayInt à partir d'un array d'int.
			 *\param[in]	p_values	L'array.
			 *\return		Le wxArrayInt.
			 */
			template< size_t Count >
			inline wxArrayInt make_wxArrayInt( std::array< int, Count > const & p_values );
			/**
			 *\~english
			 *\brief		Builds a wxArrayInt from an array of int.
			 *\param[in]	p_values	The array.
			 *\return		The wxArrayInt.
			 *\~french
			 *\brief		Construit un wxArrayInt à partir d'un array d'int.
			 *\param[in]	p_values	L'array.
			 *\return		Le wxArrayInt.
			 */
			inline wxArrayInt make_wxArrayInt( std::vector< int > const & p_values );
			/**
			 *\~english
			 *\brief		Builds a wxColour from a an Ogre::ColourValue.
			 *\param[in]	p_value	The Ogre::ColourValue.
			 *\return		The wxColour.
			 *\~french
			 *\brief		Construit un wxColour à partir d'un Ogre::ColourValue.
			 *\param[in]	p_value	L'Ogre::ColourValue.
			 *\return		Le wxColour.
			 */
			inline wxColour make_wxColour( Ogre::ColourValue const & p_value );
			/**
			 *\~english
			 *\brief		Builds a Ogre::ColourValue from a wxColour.
			 *\param[in]	p_values	The wxColour.
			 *\return		The Ogre::ColourValue.
			 *\~french
			 *\brief		Construit un Ogre::ColourValue à partir d'un wxColour.
			 *\param[in]	p_values	Le wxColour.
			 *\return		Le Ogre::ColourValue.
			 */
			inline Ogre::ColourValue make_ColourValue( wxColour const & p_values );
		}
	}
}

#include "AdditionalProperties.inl"

#endif
