#include "SoundObjectProperties.h"

#include "Project/Media/SoundObject.h"

namespace Troll
{
	namespace GUI
	{
		namespace Properties
		{
			SoundObjectProperties::SoundObjectProperties( ProjectComponents::Media::TrollSoundObject & p_soundObject )
				: ObjectProperty{ DataType::SoundObject }
				, m_soundObject{ p_soundObject }
			{
			}

			SoundObjectProperties::~SoundObjectProperties()
			{
			}

			void SoundObjectProperties::DoCreateProperties( wxPGEditor * p_editor )
			{
				wxString const PROPERTY_CATEGORY{ _( "Sound Object: " ) };
				wxString const PROPERTY_ATTENUATION{ _( "Attenuation" ) };
				wxString const PROPERTY_LOOPED{ _( "Looped" ) };
				wxString const PROPERTY_TIME{ _( "Timer time" ) };
				wxString const PROPERTY_FADEIN{ _( "Fade In Time" ) };
				wxString const PROPERTY_FADEOUT{ _( "Fade Out Time" ) };

				DoAddCategory( PROPERTY_CATEGORY + m_soundObject.GetObjectName() );
				DoAddProperty( PROPERTY_ATTENUATION, m_soundObject.GetInstance().m_attenuation, [this]( wxPGProperty * p_property )
				{
					m_soundObject.SetAttenuation( float( p_property->GetValue().GetDouble() ) );
				},
				[this]( wxPGProperty * p_property )
				{
					p_property->SetValue( m_soundObject.GetInstance().m_attenuation );
				} );
				DoAddProperty( PROPERTY_LOOPED, m_soundObject.GetInstance().m_looped, [this]( wxPGProperty * p_property )
				{
					m_soundObject.SetLooped( p_property->GetValue() );
				},
				[this]( wxPGProperty * p_property )
				{
					p_property->SetValue( m_soundObject.GetInstance().m_looped );
				} );
				DoAddProperty( PROPERTY_TIME, m_soundObject.GetInstance().m_timer, [this]( wxPGProperty * p_property )
				{
					m_soundObject.SetTimer( float( p_property->GetValue().GetDouble() ) );
				},
				[this]( wxPGProperty * p_property )
				{
					p_property->SetValue( m_soundObject.GetInstance().m_timer );
				} );
				DoAddProperty( PROPERTY_FADEIN, m_soundObject.GetInstance().m_fadeInTime, [this]( wxPGProperty * p_property )
				{
					m_soundObject.SetFadeIn( float( p_property->GetValue().GetDouble() ) );
				},
				[this]( wxPGProperty * p_property )
				{
					p_property->SetValue( m_soundObject.GetInstance().m_fadeInTime );
				} );
				DoAddProperty( PROPERTY_FADEOUT, m_soundObject.GetInstance().m_fadeOutTime, [this]( wxPGProperty * p_property )
				{
					m_soundObject.SetFadeOut( float( p_property->GetValue().GetDouble() ) );
				},
				[this]( wxPGProperty * p_property )
				{
					p_property->SetValue( m_soundObject.GetInstance().m_fadeOutTime );
				} );
			}
		}
	}
}
