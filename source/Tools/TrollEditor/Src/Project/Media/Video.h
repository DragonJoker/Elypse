/*
See LICENSE file in root folder
*/
#ifndef _TrollVideo_H_
#define _TrollVideo_H_

#include "MediaObject.h"

#include <Module_Media.h>

namespace Troll
{
	namespace ProjectComponents
	{
		namespace Media
		{
			class TrollVideo
				: public MediaObject
			{
			public:
				TrollVideo( wxString const & p_name, wxString const & p_fileName );
				~TrollVideo();

				TrollVideoObject * AddObject( Object * p_object );

				void SetMuseVideo( VideoObject * p_museVideo );
				void SetUrl( wxString const & p_url );
				void SetLooped( bool p_looped );
				void SetEnabled( bool p_enabled );

				void Write( wxTextOutputStream & p_stream );

				inline wxString const &	GetUrl()const
				{
					return m_filePath;
				}

				inline bool IsLooped()const
				{
					return m_looped;
				}

				inline bool IsEnabled()const
				{
					return m_enabled;
				}

			private:
				GUI::Properties::ObjectProperty * DoCreateProperties()override;

			private:
				wxString m_filePath;
				TrollVideoObjectMap m_objects;
				bool m_looped;
				bool m_enabled;
				VideoObject * m_museVideo;
			};
		}
	}
}

#endif

