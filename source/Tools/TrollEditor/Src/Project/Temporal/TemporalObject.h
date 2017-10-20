/*
See LICENSE file in root folder
*/
#ifndef ___TemporalObject_H___
#define ___TemporalObject_H___

#include "Module_Temporal.h"

#include "Project/Object.h"

namespace Troll
{
	namespace ProjectComponents
	{
		namespace Temporal
		{
			enum TemporalObjectType
			{
				ttAnimationGroup,
				ttAnimatedObject,
				ttAnimation,
				ttSequence,
				ttPonctualEvent,
				ttContinuousEvent,
				ttKeyFrame
			};

			class TemporalObject
				: public ProjectComponents::Object
			{
			public:
				TemporalObject( wxString const & p_name, wxString const & p_fileName, TemporalObjectType p_type );
				virtual ~TemporalObject();

				inline TemporalObjectType GetTemporalObjectType()const
				{
					return m_temporalObjectType;
				}

			protected:
				TemporalObjectType m_temporalObjectType;
			};
		}
	}
}

#endif
