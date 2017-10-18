#include "SoundProperties.h"

#include "Project/Media/Sound.h"
#include "Project/Project.h"
#include "Project/Scene.h"
#include "GUI/ProjectFrame.h"
#include "GUI/FilesTree.h"

namespace Troll
{
	using namespace ProjectComponents;
	using namespace ProjectComponents::Media;

	namespace GUI
	{
		namespace Properties
		{
			SoundProperties::SoundProperties( TrollSound & p_sound )
				: ObjectProperty{ ePROPERTY_DATA_TYPE_SOUND }
				, m_sound{ p_sound }
			{
			}

			SoundProperties::~SoundProperties()
			{
			}

			void SoundProperties::DoCreateProperties( wxPGEditor * p_editor )
			{
				wxString const PROPERTY_CATEGORY{ _( "Sound: " ) };
				wxString const PROPERTY_NAME{ _( "Name" ) };
				wxString const PROPERTY_FILE{ _( "File" ) };
				wxString const PROPERTY_TYPE{ _( "Type" ) };

				wxArrayString l_types;
				l_types.Add( _( "SFX" ) );
				l_types.Add( _( "Music" ) );

				DoAddCategory( PROPERTY_CATEGORY );
				DoAddProperty( PROPERTY_NAME, m_sound.GetObjectName(), [this]( wxPGProperty * p_property )
				{
					m_sound.SetObjectName( p_property->GetValue() );
				},
				[this]( wxPGProperty * p_property )
				{
					p_property->SetValue( m_sound.GetObjectName() );
				} );
				DoAddProperty( PROPERTY_FILE, ButtonEventHandler( SoundProperties::OnEditUrl ), this, p_editor );
				DoAddProperty( PROPERTY_TYPE, l_types, l_types[m_sound.GetSoundType() - 1], [this]( wxPGProperty * p_property )
				{
					m_sound.SetSoundType( TrollSoundType( p_property->GetValue().GetInteger() + 1 ) );
				},
				[this, l_types]( wxPGProperty * p_property )
				{
					p_property->SetValue( l_types[m_sound.GetSoundType() - 1] );
				} );

				if ( m_sound.IsGlobal() )
				{
					wxString const PROPERTY_ATTENUATION{ _( "Attenuation" ) };
					wxString const PROPERTY_LOOPED{ _( "Looped" ) };
					wxString const PROPERTY_TIME{ _( "Timer time" ) };
					wxString const PROPERTY_FADEIN{ _( "Fade In Time" ) };
					wxString const PROPERTY_FADEOUT{ _( "Fade Out Time" ) };

					DoAddProperty( PROPERTY_ATTENUATION, m_sound.GetGlobal()->m_attenuation, [this]( wxPGProperty * p_property )
					{
						m_sound.SetGlobalAttenuation( float( p_property->GetValue().GetDouble() ) );
					},
					[this]( wxPGProperty * p_property )
					{
						p_property->SetValue( m_sound.GetGlobal()->m_attenuation );
					} );
					DoAddProperty( PROPERTY_LOOPED, m_sound.GetGlobal()->m_looped, [this]( wxPGProperty * p_property )
					{
						m_sound.SetGlobalLooped( p_property->GetValue() );
					},
					[this]( wxPGProperty * p_property )
					{
						p_property->SetValue( m_sound.GetGlobal()->m_looped );
					} );
					DoAddProperty( PROPERTY_TIME, m_sound.GetGlobal()->m_timer, [this]( wxPGProperty * p_property )
					{
						m_sound.SetGlobalTimer( float( p_property->GetValue().GetDouble() ) );
					},
					[this]( wxPGProperty * p_property )
					{
						p_property->SetValue( m_sound.GetGlobal()->m_timer );
					} );
					DoAddProperty( PROPERTY_FADEIN, m_sound.GetGlobal()->m_fadeInTime, [this]( wxPGProperty * p_property )
					{
						m_sound.SetGlobalFadeIn( float( p_property->GetValue().GetDouble() ) );
					},
					[this]( wxPGProperty * p_property )
					{
						p_property->SetValue( m_sound.GetGlobal()->m_fadeInTime );
					} );
					DoAddProperty( PROPERTY_FADEOUT, m_sound.GetGlobal()->m_fadeOutTime, [this]( wxPGProperty * p_property )
					{
						m_sound.SetGlobalFadeOut( float( p_property->GetValue().GetDouble() ) );
					},
					[this]( wxPGProperty * p_property )
					{
						p_property->SetValue( m_sound.GetGlobal()->m_fadeOutTime );
					} );
				}
			}

			bool SoundProperties::OnEditUrl( wxPGProperty * p_property )
			{
				wxFileDialog l_dialog( wxGetApp().GetProjectFrame(), _( "Choose an audio file" ), wxEmptyString,
									   wxEmptyString,
									   wxString() << _( "Audio files" ) << wxT( " (*.wav;*.mp3;*.ogg;*.wma)|*.wav;*.mp3;*.ogg;*.wma" ) );

				if ( l_dialog.ShowModal() )
				{
					Path l_filePath = make_string( l_dialog.GetPath() );
					Path l_folderPath = make_string( wxGetApp().GetProjectFrame()->GetProject()->GetPath() + wxT( "Sons" ) );

					if ( !DirectoryExists( l_folderPath ) )
					{
						DirectoryCreate( l_folderPath );
					}

					wxString text = _( "Sounds" );
					auto & l_file = wxGetApp().GetProjectFrame()->GetProject()->GetMainScene()->AddFile( dataFolder, text, true, false );
					l_file.FolderId = wxGetApp().GetProjectFrame()->GetFilesList()->GetFolderId( OTHER_DATA_FOLDERS );
					l_file.ItemId = wxGetApp().GetProjectFrame()->GetFilesList()->AddExistingFileToProjet( l_file.FolderId, TITDataFolder, text );
					FileCopy( l_filePath, l_folderPath / l_filePath.GetLeaf() );
					Path l_relPath = make_string( wxGetApp().GetProjectFrame()->GetProject()->GetMainScene()->GetName() );
					l_relPath /= "Sons/";
					m_sound.SetUrl( make_wxString( l_relPath ) );
				}

				return false;
			}
		}
	}
}
