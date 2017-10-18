#include "VideoObjectProperties.h"

#include "Project/Media/VideoObject.h"

namespace Troll
{
	namespace GUI
	{
		namespace Properties
		{
			VideoObjectProperties::VideoObjectProperties( ProjectComponents::Media::TrollVideoObject & p_videoObject )
				: ObjectProperty{ ePROPERTY_DATA_TYPE_VIDEO_OBJECT }
				, m_videoObject{ p_videoObject }
			{
			}

			VideoObjectProperties::~VideoObjectProperties()
			{
			}

			void VideoObjectProperties::DoCreateProperties( wxPGEditor * p_editor )
			{
				wxString const PROPERTY_CATEGORY{ _( "Video Object: " ) };
				wxString const PROPERTY_ATTENUATION{ _( "Attenuation" ) };

				DoAddCategory( PROPERTY_CATEGORY + m_videoObject.GetObjectName() );
				DoAddProperty( PROPERTY_ATTENUATION, m_videoObject.GetAttenuation(), [this]( wxPGProperty * p_property )
				{
					m_videoObject.SetAttenuation( float( p_property->GetValue().GetDouble() ) );
				},
				[this]( wxPGProperty * p_property )
				{
					p_property->SetValue( m_videoObject.GetAttenuation() );
				} );
			}
		}
	}
}
