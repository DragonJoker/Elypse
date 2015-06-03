
#ifndef _TrollVideoObject_H_
#define _TrollVideoObject_H_

#include "Module_Media.h"
#include "MediaObject.h"

#include <Module_Media.h>

namespace Troll
{
	namespace Media
	{
		class TrollVideoObject
			: public MediaObject
		{
		private:
			TrollVideo * m_video;
			TrollObject * m_object;
			float m_attenuation;
			EMuse::Media::VideoObject * m_videoObject;
			EMuse::Media::VideoOverlay * m_videoOverlay;

//		MediaEntity * m_museMediaEntity;

		public:
			TrollVideoObject( const wxString & p_name, const wxString & p_fileName );
			~TrollVideoObject();

			virtual int BuildPanel( wxWindow * p_parent, int p_width );

//		void SetMuseMediaEntity( MediaEntity * p_me);
			void SetAttenuation( float p_att );

			void Write( wxTextOutputStream * p_stream );

			void SetMuseVideoObject( EMuse::Media::VideoObject * p_videoObject );
			void SetMuseVideoOverlay( EMuse::Media::VideoOverlay * p_videoOverlay );

		public:
			inline void SetVideo( TrollVideo * p_video )
			{
				m_video = p_video;
			}
			inline TrollVideo *	GetVideo()const
			{
				return m_video;
			}
			inline float		GetAttenuation()const
			{
				return m_attenuation;
			}

		};
	}
}

#endif

