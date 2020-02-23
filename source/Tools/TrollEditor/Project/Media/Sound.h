/*
See LICENSE file in root folder
*/
#ifndef _TrollSound_H_
#define _TrollSound_H_

#include "MediaObject.h"

#include "Project/3D/Module_3D.h"

#include <Module_Media.h>

namespace Troll
{
	namespace ProjectComponents
	{
		namespace Media
		{
			class TrollSound
				: public MediaObject
			{
			public:
				TrollSound( wxString const & p_name, wxString const & p_fileName );
				~TrollSound();

				TrollSoundObject * AddObject( ProjectComponents::Objects3D::TrollSceneNode * p_node );

				void SetMuseSound( Elypse::Media::SoundObject * p_sound );
				void SetUrl( wxString const & p_filePath );
				void SetSoundType( TrollSoundType p_type );
				void SetLocal();
				void SetGlobalAttenuation( float p_att );
				void SetGlobalLooped( bool p_looped );
				void SetGlobalTimer( float p_time );
				void SetGlobalFadeIn( float p_time );
				void SetGlobalFadeOut( float p_time );

				void Write( wxTextOutputStream & p_stream );

				inline wxString GetUrl()const
				{
					return m_filePath;
				}
				inline TrollSoundType GetSoundType()const
				{
					return m_soundType;
				}
				inline bool IsGlobal()const
				{
					return m_isGlobal;
				}
				inline SoundInstance * GetGlobal()const
				{
					return m_globalInstance;
				}

			private:
				GUI::Properties::ObjectProperty * DoCreateProperties()override;

			private:
				wxString m_filePath;
				TrollSoundType m_soundType;
				bool m_isGlobal;
				TrollSoundObjectMap m_objects;
				Elypse::Media::SoundObject * m_museSound;

				//if global
				SoundInstance * m_globalInstance;
			};
		}
	}
}

#endif
