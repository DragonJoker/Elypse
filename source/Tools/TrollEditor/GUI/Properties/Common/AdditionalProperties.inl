namespace Troll
{
	namespace GUI
	{
		namespace Properties
		{
			//************************************************************************************************

			template<>
			inline bool GetValue< bool >( wxVariant const & p_variant )
			{
				return p_variant.GetBool();
			}

			template<>
			inline int GetValue< int >( wxVariant const & p_variant )
			{
				return p_variant.GetInteger();
			}

			template<>
			inline uint32_t GetValue< uint32_t >( wxVariant const & p_variant )
			{
				return uint32_t( p_variant.GetInteger() );
			}

			template<>
			inline float GetValue< float >( wxVariant const & p_variant )
			{
				return float( p_variant.GetDouble() );
			}

			template<>
			inline double GetValue< double >( wxVariant const & p_variant )
			{
				return p_variant.GetDouble();
			}

			//************************************************************************************************

			template<>
			inline wxVariant SetValue< bool >( bool const & p_value )
			{
				return wxVariant( p_value );
			}

			template<>
			inline wxVariant SetValue< int >( int const & p_value )
			{
				return wxVariant( p_value );
			}

			template<>
			inline wxVariant SetValue< uint32_t >( uint32_t const & p_value )
			{
				return wxVariant( int( p_value ) );
			}

			template<>
			inline wxVariant SetValue< float >( float const & p_value )
			{
				return wxVariant( p_value );
			}

			template<>
			inline wxVariant SetValue< double >( double const & p_value )
			{
				return wxVariant( p_value );
			}

			//************************************************************************************************

			template<>
			inline wxPGProperty * CreateProperty< double >( wxString const & p_name, double const & p_value )
			{
				auto l_property = new wxFloatProperty( p_name, wxPG_LABEL );
				l_property->SetValue( p_value );
				return l_property;
			}

			template<>
			inline wxPGProperty * CreateProperty< float >( wxString const & p_name, float const & p_value )
			{
				auto l_property = new wxFloatProperty( p_name, wxPG_LABEL );
				l_property->SetValue( p_value );
				return l_property;
			}

			template<>
			inline wxPGProperty * CreateProperty< int >( wxString const & p_name, int const & p_value )
			{
				auto l_property = new wxIntProperty( p_name, wxPG_LABEL );
				l_property->SetValue( p_value );
				return l_property;
			}

			template<>
			inline wxPGProperty * CreateProperty< uint32_t >( wxString const & p_name, uint32_t const & p_value )
			{
				auto l_property = new wxUIntProperty( p_name, wxPG_LABEL );
				l_property->SetValueFromInt( p_value );
				return l_property;
			}

			template<>
			inline wxPGProperty * CreateProperty< bool >( wxString const & p_name, bool const & p_value )
			{
				auto l_property = new wxBoolProperty( p_name, wxPG_LABEL );
				l_property->SetAttribute( wxT( "UseCheckbox" ), true );
				l_property->SetValue( p_value );
				return l_property;
			}

			template<>
			inline wxPGProperty * CreateProperty< wxString >( wxString const & p_name, wxString const & p_value )
			{
				auto l_property = new wxStringProperty( p_name, wxPG_LABEL );
				l_property->SetValue( p_value );
				return l_property;
			}

			template<>
			inline wxPGProperty * CreateProperty< wxColour >( wxString const & p_name, wxColour const & p_value )
			{
				auto l_property = new wxColourProperty( p_name, wxPG_LABEL );
				l_property->SetValue( WXVARIANT( p_value ) );
				return l_property;
			}

			//************************************************************************************************

			template< size_t Count >
			inline wxArrayString make_wxArrayString( std::array< wxString, Count > const & p_values )
			{
				return wxArrayString{ Count, p_values.data() };
			}

			inline wxArrayString make_wxArrayString( std::vector< wxString > const & p_values )
			{
				return wxArrayString{ p_values.size(), p_values.data() };
			}

			inline wxArrayString make_wxArrayString( StringArray const & p_values )
			{
				wxArrayString l_return;
				l_return.reserve( p_values.size() );

				for ( auto l_value : p_values )
				{
					l_return.push_back( l_value );
				}

				return l_return;
			}

			template< size_t Count >
			inline wxArrayInt make_wxArrayInt( std::array< int, Count > const & p_values )
			{
				wxArrayInt l_return{ Count };
				std::memcpy( &l_return[0], p_values.data(), Count * sizeof( int ) );
				return l_return;
			}

			inline wxArrayInt make_wxArrayInt( std::vector< int > const & p_values )
			{
				wxArrayInt l_return{ p_values.size() };
				std::memcpy( &l_return[0], p_values.data(), p_values.size() * sizeof( int ) );
				return l_return;
			}

			inline wxColour make_wxColour( Ogre::ColourValue const & p_value )
			{
				return wxColour{ uint8_t( p_value.r * 255 ), uint8_t( p_value.g * 255 ), uint8_t( p_value.b * 255 ) };
			}

			inline Ogre::ColourValue make_ColourValue( wxColour const & p_values )
			{
				return Ogre::ColourValue{ p_values.Red() / 255.0f, p_values.Green() / 255.0f, p_values.Blue() / 255.0f, 1.0f };
			}

			//************************************************************************************************
		}
	}
}
