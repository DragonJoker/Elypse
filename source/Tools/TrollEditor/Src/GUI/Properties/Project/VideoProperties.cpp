#include "VideoProperties.h"

#include "Project/Media/Video.h"
#include "Project/Project.h"
#include "Project/Scene.h"
#include "GUI/ProjectFrame.h"

namespace Troll
{
	namespace GUI
	{
		namespace Properties
		{
			VideoProperties::VideoProperties( ProjectComponents::Media::TrollVideo & p_video )
				: ObjectProperty{ ePROPERTY_DATA_TYPE_VIDEO }
				, m_video{ p_video }
			{
			}

			VideoProperties::~VideoProperties()
			{
			}

			void VideoProperties::DoCreateProperties( wxPGEditor * p_editor )
			{
				wxString const PROPERTY_CATEGORY{ _( "Video: " ) };
				wxString const PROPERTY_MATERIAL{ _( "Material" ) };
				wxString const PROPERTY_URL{ _( "URL" ) };
				wxString const PROPERTY_LOOPED{ _( "Looped" ) };
				wxString const PROPERTY_ENABLED{ _( "Enabled" ) };

				DoAddCategory( PROPERTY_CATEGORY );
				DoAddProperty( PROPERTY_MATERIAL, m_video.GetObjectName(), [this]( wxPGProperty * p_property )
				{
					m_video.SetObjectName( p_property->GetValue() );
				},
				[this]( wxPGProperty * p_property )
				{
					p_property->SetValue( m_video.GetObjectName() );
				} );
				DoAddProperty( PROPERTY_URL, ButtonEventHandler( VideoProperties::OnEditUrl ), this, p_editor );
				DoAddProperty( PROPERTY_LOOPED, m_video.IsLooped(), [this]( wxPGProperty * p_property )
				{
					m_video.SetLooped( p_property->GetValue() );
				},
				[this]( wxPGProperty * p_property )
				{
					p_property->SetValue( m_video.IsLooped() );
				} );
				DoAddProperty( PROPERTY_ENABLED, m_video.IsEnabled(), [this]( wxPGProperty * p_property )
				{
					m_video.SetEnabled( p_property->GetValue() );
				},
				[this]( wxPGProperty * p_property )
				{
					p_property->SetValue( m_video.IsEnabled() );
				} );
			}

			bool VideoProperties::OnEditUrl( wxPGProperty * p_property )
			{
				wxFileDialog l_dialog( wxGetApp().GetProjectFrame(), _( "Choose a video file" ), wxEmptyString, wxEmptyString,
									   wxString() << _( "Video files" ) << wxT( " (*.mpeg;*.avi;*.ogm;*.wmv)|*.mpeg;*.avi;*.ogm;*.wmv" ) );

				if ( l_dialog.ShowModal() )
				{
					wxString l_filePath = l_dialog.GetPath();
					m_video.SetUrl( l_filePath );
				}

				return false;
			}
		}
	}
}
