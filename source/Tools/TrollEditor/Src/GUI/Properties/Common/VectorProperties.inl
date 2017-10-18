namespace Troll
{
	namespace GUI
	{
		namespace Properties
		{
			//************************************************************************************************

			namespace
			{
				template< Vector2Names Names > wxString const * GetVector2Name();

				template<>
				inline wxString const * GetVector2Name< Vector2Names::Point >()
				{
					static wxString const Value[2]{ _( "X" ), _( "Y" ) };
					return Value;
				}

				template<>
				inline wxString const * GetVector2Name< Vector2Names::Size >()
				{
					static wxString const Value[2]{ _( "Width" ), _( "Height" ) };
					return Value;
				}

				template< Vector3Names Names > wxString const * GetVector3Name();

				template<>
				inline wxString const * GetVector3Name< Vector3Names::Point >()
				{
					static wxString const Value[3]{ _( "X" ), _( "Y" ), _( "Z" ) };
					return Value;
				}

				template<>
				inline wxString const * GetVector3Name< Vector3Names::Size >()
				{
					static wxString const Value[3]{ _( "Width" ), _( "Height" ), _( "Depth" ) };
					return Value;
				}

				template<>
				inline wxString const * GetVector3Name< Vector3Names::Spotlight >()
				{
					static wxString const Value[3]{ _( "Inner Angle" ), _( "Outer Angle" ), _( "Falloff" ) };
					return Value;
				}

				template< Vector4Names Names > wxString const * GetVector4Name();

				template<>
				inline wxString const * GetVector4Name< Vector4Names::Point >()
				{
					static wxString const Value[4]{ _( "X" ), _( "Y" ), _( "Z" ), _( "W" ) };
					return Value;
				}

				template<>
				inline wxString const * GetVector4Name< Vector4Names::Attenuation >()
				{
					static wxString const Value[4]{ _( "Range" ), _( "Constant" ), _( "Linear" ), _( "Quadratic" ) };
					return Value;
				}

				template<>
				inline wxString const * GetVector4Name< Vector4Names::Rectangle >()
				{
					static wxString const Value[4]{ _( "Left" ), _( "Top" ), _( "Right" ), _( "Bottom" ) };
					return Value;
				}
			}

			//************************************************************************************************

			template< Vector2Names NameT >
			wxVector2Property< NameT >::wxVector2Property( wxString const & label, wxString const & name, Vector2 const & value )
				: wxPGProperty( label, name )
			{
				SetValueI( value );
				AddPrivateChild( new wxFloatProperty( GetVector2Name< NameT >()[0], wxPG_LABEL, value.x ) );
				AddPrivateChild( new wxFloatProperty( GetVector2Name< NameT >()[1], wxPG_LABEL, value.y ) );
			}

			template< Vector2Names NameT >
			wxVector2Property< NameT >::~wxVector2Property()
			{
			}

			template< Vector2Names NameT >
			void wxVector2Property< NameT >::RefreshChildren()
			{
				if ( GetChildCount() )
				{
					Vector2 const & point = Vector2RefFromVariant( m_value );
					Item( 0 )->SetValue( point.x );
					Item( 1 )->SetValue( point.y );
				}
			}

			template< Vector2Names NameT >
			wxVariant wxVector2Property< NameT >::ChildChanged( wxVariant & thisValue, int childIndex, wxVariant & childValue )const
			{
				Vector2 & point = Vector2RefFromVariant( thisValue );
				auto val = Real( childValue.GetDouble() );

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

			template< Vector2Names NameT >
			inline void wxVector2Property< NameT >::SetValueI( Vector2 const & value )
			{
				m_value = WXVARIANT( value );
			}

			//**********************************************************************************************

			template< Vector3Names NameT >
			wxVector3Property< NameT >::wxVector3Property( wxString const & label, wxString const & name, Vector3 const & value )
				: wxPGProperty( label, name )
			{
				SetValueI( value );
				AddPrivateChild( new wxFloatProperty( GetVector3Name< NameT >()[0], wxPG_LABEL, value.x ) );
				AddPrivateChild( new wxFloatProperty( GetVector3Name< NameT >()[1], wxPG_LABEL, value.y ) );
				AddPrivateChild( new wxFloatProperty( GetVector3Name< NameT >()[2], wxPG_LABEL, value.z ) );
			}

			template< Vector3Names NameT >
			wxVector3Property< NameT >::~wxVector3Property()
			{
			}

			template< Vector3Names NameT >
			void wxVector3Property< NameT >::RefreshChildren()
			{
				if ( GetChildCount() )
				{
					Vector3 const & point = Vector3RefFromVariant( m_value );
					Item( 0 )->SetValue( point.x );
					Item( 1 )->SetValue( point.y );
					Item( 2 )->SetValue( point.z );
				}
			}

			template< Vector3Names NameT >
			wxVariant wxVector3Property< NameT >::ChildChanged( wxVariant & thisValue, int childIndex, wxVariant & childValue )const
			{
				Vector3 & point = Vector3RefFromVariant( thisValue );
				auto val = Real( childValue.GetDouble() );

				switch ( childIndex )
				{
				case 0:
					point.x = val;
					break;

				case 1:
					point.y = val;
					break;

				case 2:
					point.z = val;
					break;
				}

				wxVariant newVariant;
				newVariant << point;
				return newVariant;
			}

			template< Vector3Names NameT >
			inline void wxVector3Property< NameT >::SetValueI( Vector3 const & value )
			{
				m_value = WXVARIANT( value );
			}

			//**********************************************************************************************

			template< Vector4Names NameT >
			wxVector4Property< NameT >::wxVector4Property( wxString const & label, wxString const & name, Vector4 const & value )
				: wxPGProperty( label, name )
			{
				SetValueI( value );
				AddPrivateChild( new wxFloatProperty( GetVector4Name< NameT >()[0], wxPG_LABEL, value.x ) );
				AddPrivateChild( new wxFloatProperty( GetVector4Name< NameT >()[1], wxPG_LABEL, value.y ) );
				AddPrivateChild( new wxFloatProperty( GetVector4Name< NameT >()[2], wxPG_LABEL, value.z ) );
				AddPrivateChild( new wxFloatProperty( GetVector4Name< NameT >()[3], wxPG_LABEL, value.w ) );
			}

			template< Vector4Names NameT >
			wxVector4Property< NameT >::~wxVector4Property()
			{
			}

			template< Vector4Names NameT >
			void wxVector4Property< NameT >::RefreshChildren()
			{
				if ( GetChildCount() )
				{
					Vector4 const & point = Vector4RefFromVariant( m_value );
					Item( 0 )->SetValue( point.x );
					Item( 1 )->SetValue( point.y );
					Item( 2 )->SetValue( point.z );
					Item( 3 )->SetValue( point.z );
				}
			}

			template< Vector4Names NameT >
			wxVariant wxVector4Property< NameT >::ChildChanged( wxVariant & thisValue, int childIndex, wxVariant & childValue )const
			{
				Vector4 & point = Vector4RefFromVariant( thisValue );
				auto val = Real( childValue.GetDouble() );

				switch ( childIndex )
				{
				case 0:
					point.x = val;
					break;

				case 1:
					point.y = val;
					break;

				case 2:
					point.z = val;
					break;

				case 3:
					point.w = val;
					break;
				}

				wxVariant newVariant;
				newVariant << point;
				return newVariant;
			}

			template< Vector4Names NameT >
			inline void wxVector4Property< NameT >::SetValueI( Vector4 const & value )
			{
				m_value = WXVARIANT( value );
			}

			//************************************************************************************************
		}
	}
}
