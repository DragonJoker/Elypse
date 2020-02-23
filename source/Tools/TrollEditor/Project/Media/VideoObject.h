/*
See LICENSE file in root folder
*/
#ifndef _TrollVideoObject_H_
#define _TrollVideoObject_H_

#include "Module_Media.h"
#include "MediaObject.h"

#include <Module_Media.h>

namespace Troll
{
	namespace ProjectComponents
	{
		namespace Media
		{
			class TrollVideoObject
				: public MediaObject
			{
			public:
				TrollVideoObject( wxString const & p_name, wxString const & p_fileName );
				~TrollVideoObject();

				void SetAttenuation( float p_att );
				void Write( wxTextOutputStream & p_stream );
				void SetMuseVideoObject( Elypse::Media::VideoObject * p_videoObject );
				void SetMuseVideoOverlay( Elypse::Media::VideoOverlay * p_videoOverlay );

				inline void SetVideo( TrollVideo * p_video )
				{
					m_video = p_video;
				}

				inline TrollVideo * GetVideo()const
				{
					return m_video;
				}

				inline float GetAttenuation()const
				{
					return m_attenuation;
				}

			private:
				GUI::Properties::ObjectProperty * DoCreateProperties()override;

			private:
				TrollVideo * m_video;
				Object * m_object;
				float m_attenuation;
				Elypse::Media::VideoObject * m_videoObject;
				Elypse::Media::VideoOverlay * m_videoOverlay;
			};
		}
	}
}

#endif
