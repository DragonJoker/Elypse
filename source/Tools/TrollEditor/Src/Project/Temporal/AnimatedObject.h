/*
See LICENSE file in root folder
*/
#ifndef ___TrollAnimatedObject___
#define ___TrollAnimatedObject___

#include "TemporalObject.h"

namespace Troll
{
	namespace ProjectComponents
	{
		namespace Temporal
		{
			class TrollAnimatedObject
				: public TemporalObject
			{
			public:
				TrollAnimatedObject( Object * p_object, TrollAnimationGroup * p_group, wxString const & p_fileName );
				~TrollAnimatedObject();

				void AddAnimation( TrollAnimation * p_anim );
				void Write( wxTextOutputStream & p_stream );

				inline Object * GetObject()const
				{
					return m_object;
				}

				inline TrollAnimationGroup * GetGroup()const
				{
					return m_group;
				}

			private:
				GUI::Properties::ObjectProperty * DoCreateProperties()override;

			private:
				TrollAnimationGroup * m_group;
				Object * m_object;
				TrollAnimationStrMap m_animations;
			};
		}
	}
}

#endif

