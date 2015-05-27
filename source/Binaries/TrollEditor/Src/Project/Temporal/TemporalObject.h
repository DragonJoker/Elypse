
#ifndef ___TemporalObject_H___
#define ___TemporalObject_H___

#include "Module_Temporal.h"
#include "../Object.h"

namespace Troll
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

		class TemporalObject : public Troll::TrollObject
		{
		protected:
			TemporalObjectType m_temporalObjectType;

		public:
			TemporalObject( const wxString & p_name, const wxString & p_fileName, TemporalObjectType p_type );
			virtual ~TemporalObject();

			inline TemporalObjectType	GetTemporalObjectType()const
			{
				return m_temporalObjectType;
			}
		};
	}
}

#endif

