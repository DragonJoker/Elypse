//***********************************************************************************************************
#ifndef _TrollVideo_H_
#define _TrollVideo_H_
//***********************************************************************************************************
#include "Module_Media.h"
#include "MediaObject.h"
#include <Module_Media.h>
//***********************************************************************************************************
namespace Troll
{
	namespace Media
	{
		class TrollVideo : public MediaObject
		{
		private:
			wxString m_filePath;
			TrollVideoObjectMap m_objects;
			bool m_looped;
			bool m_enabled;
			VideoObject * m_museVideo;

		public:
			TrollVideo( const wxString & p_name, const wxString & p_fileName );
			~TrollVideo();

			TrollVideoObject * AddObject( TrollObject * p_object );

			virtual int BuildPanel( wxWindow * p_parent, int p_width );

			void SetMuseVideo( VideoObject * p_museVideo );
			void SetUrl( const wxString & p_url );
			void SetLooped( bool p_looped );
			void SetEnabled( bool p_enabled );

			void Write( wxTextOutputStream * p_stream );

		public:
			inline const wxString &	GetUrl()const
			{
				return m_filePath;
			}
			inline bool				IsLooped()const
			{
				return m_looped;
			}
			inline bool				IsEnabled()const
			{
				return m_enabled;
			}
		};
	}
}
//***********************************************************************************************************
#endif
//***********************************************************************************************************
