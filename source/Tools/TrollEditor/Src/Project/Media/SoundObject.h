/*
See LICENSE file in root folder
*/
#ifndef _TrollSoundObject_H_
#define _TrollSoundObject_H_

#include "MediaObject.h"

#include <Module_Media.h>

namespace Troll
{
	namespace ProjectComponents
	{
		namespace Media
		{
			class TrollSoundObject
				: public MediaObject
			{
			public:
				TrollSoundObject( wxString const & p_name, wxString const & p_fileName );
				~TrollSoundObject();

				void SetMuseInstance( Elypse::Media::SoundInstance * p_instance );
				void SetAttenuation( float p_att );
				void SetLooped( bool p_looped );
				void SetTimer( float p_time );
				void SetFadeIn( float p_time );
				void SetFadeOut( float p_time );

				void Write( wxTextOutputStream & p_stream );

				inline TrollSound * GetSound()const
				{
					return m_sound;
				}

				inline SoundInstance GetInstance()const
				{
					return m_instance;
				}

				inline void SetSound( TrollSound * p_sound )
				{
					m_sound = p_sound;
				}

			private:
				GUI::Properties::ObjectProperty * DoCreateProperties()override;

			private:
				TrollSound * m_sound;
				Object * m_object;
				SoundInstance m_instance;
				Elypse::Media::SoundInstance * m_museInstance;
			};
		}
	}
}

#endif
