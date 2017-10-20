/*
See LICENSE file in root folder
*/
#ifndef ___MediaObject_H___
#define ___MediaObject_H___

#include "Module_Media.h"

#include "Project/Object.h"

namespace Troll
{
	namespace ProjectComponents
	{
		namespace Media
		{
			class MediaObject
				: public Object
			{
			public:
				MediaObject( wxString const & p_name, wxString const & p_fileName, MediaObjectType p_type );
				virtual ~MediaObject();

				inline MediaObjectType	GetMediaObjectType()const
				{
					return m_mediaObjectType;
				}

			protected:
				MediaObjectType m_mediaObjectType;
			};
		}
	}
}

#endif
