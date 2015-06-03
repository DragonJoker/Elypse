
#ifndef _TrollSoundObject_H_
#define _TrollSoundObject_H_

#include "Module_Media.h"
#include "MediaObject.h"

#include <Module_Media.h>

namespace Troll
{
	namespace Media
	{
		class TrollSoundObject
			: public MediaObject
		{
		private:
			TrollSound * m_sound;
			TrollObject * m_object;
			SoundInstance m_instance;
			EMuse::Media::SoundInstance * m_museInstance;

		public:
			TrollSoundObject( const wxString & p_name, const wxString & p_fileName );
			~TrollSoundObject();

			virtual int BuildPanel( wxWindow * p_parent, int p_width );

			void SetMuseInstance( EMuse::Media::SoundInstance * p_instance );
			void SetAttenuation( float p_att );
			void SetLooped( bool p_looped );
			void SetTimer( float p_time );
			void SetFadeIn( float p_time );
			void SetFadeOut( float p_time );

			void Write( wxTextOutputStream * p_stream );

		public:
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
		};
	}
}

#endif

