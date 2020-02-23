#include "KeyFrameProperties.h"

#include "Project/Temporal/ContinuousEvent.h"
#include "GUI/MainFrame.h"

namespace Troll
{
	namespace GUI
	{
		namespace Properties
		{
			KeyFrameProperties::KeyFrameProperties( ProjectComponents::Temporal::TrollKeyFrame & p_keyFrame )
				: ObjectProperty{ DataType::Entity }
				, m_key{ p_keyFrame }
			{
			}

			KeyFrameProperties::~KeyFrameProperties()
			{
			}

			void KeyFrameProperties::DoCreateProperties( wxPGEditor * p_editor )
			{
				wxString const PROPERTY_CATEGORY_KEYFRAME{ _( "KeyFrame: " ) };
				wxString const PROPERTY_KEYFRAME_TIME{ _( "Time" ) };

				DoAddCategory( PROPERTY_CATEGORY_KEYFRAME );
				DoAddProperty( PROPERTY_KEYFRAME_TIME, m_key.GetFireTime(), [this]( wxPGProperty * p_property )
				{
					m_key.SetFireTime( Real( double( p_property->GetValue() ) ) );
				},
				[this]( wxPGProperty * p_property )
				{
					p_property->SetValue( m_key.GetFireTime() );
				} );

				DoBuildArguments();
			}

			void KeyFrameProperties::DoBuildArguments()
			{
				wxString const PROPERTY_KEYFRAME_VALUE{ _( "Value" ) };

				switch ( m_key.GetArgsType() )
				{
				case ProjectComponents::Temporal::ceReal:
				{
					Real l_value;
					if ( GetReal( m_key.GetAction(), l_value ) )
					{
						DoAddProperty( PROPERTY_KEYFRAME_VALUE, l_value, [this]( wxPGProperty * p_property )
						{
							m_key.SetAction( wxString() << double( p_property->GetValue() ) );
						},
						[this]( wxPGProperty * p_property )
						{
							Real l_value;
							if ( GetReal( m_key.GetAction(), l_value ) )
							{
								p_property->SetValue( m_key.GetAction() );
							}
						} );
					}
					break;
				}

				case ProjectComponents::Temporal::ceVector2:
				{
					Vector2 l_value;
					Ogre::StringVector l_split = Ogre::StringUtil::split( make_string( m_key.GetAction() ), ", \t", 1 );
					if ( l_split.size() >= 2 && GetReal( l_split[0], l_value.x ) && GetReal( l_split[1], l_value.y ) )
					{
						DoAddProperty< Vector2Names::Point >( PROPERTY_KEYFRAME_VALUE, l_value, [this]( wxPGProperty * p_property )
						{
							Vector2 l_value = Vector2RefFromVariant( p_property->GetValue() );
							m_key.SetAction( wxString() << l_value.x << " " << l_value.y );
						},
						[this]( wxPGProperty * p_property )
						{
							Vector2 l_value;
							Ogre::StringVector l_split = Ogre::StringUtil::split( make_string( m_key.GetAction() ), ", \t", 1 );
							if ( l_split.size() >= 2 && GetReal( l_split[0], l_value.x ) && GetReal( l_split[1], l_value.y ) )
							{
								p_property->SetValue( WXVARIANT( l_value ) );
							}
						} );
					}
					break;
				}

				case ProjectComponents::Temporal::ceVector3:
				{
					Vector3 l_value;
					Ogre::StringVector l_split = Ogre::StringUtil::split( make_string( m_key.GetAction() ), ", \t", 2 );
					if ( l_split.size() >= 3 && GetReal( l_split[0], l_value.x ) && GetReal( l_split[1], l_value.y ) && GetReal( l_split[2], l_value.z ) )
					{
						DoAddProperty< Vector3Names::Point >( PROPERTY_KEYFRAME_VALUE, l_value, [this]( wxPGProperty * p_property )
						{
							Vector3 l_value = Vector3RefFromVariant( p_property->GetValue() );
							m_key.SetAction( wxString() << l_value.x << " " << l_value.y << " " << l_value.z );
						},
						[this]( wxPGProperty * p_property )
						{
							Vector3 l_value;
							Ogre::StringVector l_split = Ogre::StringUtil::split( make_string( m_key.GetAction() ), ", \t", 2 );
							if ( l_split.size() >= 3 && GetReal( l_split[0], l_value.x ) && GetReal( l_split[1], l_value.y ) && GetReal( l_split[2], l_value.z ) )
							{
								p_property->SetValue( WXVARIANT( l_value ) );
							}
						} );
					}
					break;
				}

				case ProjectComponents::Temporal::ceQuaternion:
				{
					Real l_angle;
					Vector3 l_axis;
					Ogre::StringVector l_split = Ogre::StringUtil::split( make_string( m_key.GetAction() ), ", \t", 3 );
					if ( l_split.size() >= 3 && GetReal( l_split[0], l_axis.x ) && GetReal( l_split[1], l_axis.y ) && GetReal( l_split[2], l_axis.z ) && GetReal( l_split[3], l_angle ) )
					{
						Quaternion l_quat{ Degree{ l_angle }, l_axis };
						DoAddProperty( PROPERTY_KEYFRAME_VALUE, l_quat, [this]( wxPGProperty * p_property )
						{
							Quaternion l_value = QuaternionRefFromVariant( p_property->GetValue() );
							Degree l_angle;
							Vector3 l_axis;
							l_value.ToAngleAxis( l_angle, l_axis );
							m_key.SetAction( wxString() << l_axis.x << " " << l_axis.y << " " << l_axis.z << " " << l_angle.valueDegrees() );
						},
						[this]( wxPGProperty * p_property )
						{
							Real l_angle;
							Vector3 l_axis;
							Ogre::StringVector l_split = Ogre::StringUtil::split( make_string( m_key.GetAction() ), ", \t", 3 );
							if ( l_split.size() >= 3 && GetReal( l_split[0], l_axis.x ) && GetReal( l_split[1], l_axis.y ) && GetReal( l_split[2], l_axis.z ) && GetReal( l_split[3], l_angle ) )
							{
								Quaternion l_quat{ Degree{ l_angle }, l_axis };
								p_property->SetValue( WXVARIANT( l_quat ) );
							}
						} );
					}
					break;
				}

				default:
					break;
				}
			}
		}
	}
}
