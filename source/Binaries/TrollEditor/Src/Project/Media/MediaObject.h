//***********************************************************************************************************
#ifndef ___MediaObject_H___
#define ___MediaObject_H___
//***********************************************************************************************************
#include "Module_Media.h"
#include "../Object.h"
//***********************************************************************************************************
namespace Troll
{
	namespace Media
	{
		class MediaObject : public Troll::TrollObject
		{
		protected:
			MediaObjectType m_mediaObjectType;

		public:
			MediaObject( const wxString & p_name, const wxString & p_fileName, MediaObjectType p_type );
			virtual ~MediaObject();

			inline MediaObjectType	GetMediaObjectType()const
			{
				return m_mediaObjectType;
			}
		};
	}
}
//***********************************************************************************************************
#endif
//***********************************************************************************************************
