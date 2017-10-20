/*
See LICENSE file in root folder
*/
#ifndef ___TrollAnimationGroup___
#define ___TrollAnimationGroup___

#include "TemporalObject.h"

namespace Troll
{
	namespace ProjectComponents
	{
		namespace Temporal
		{
			class TrollAnimationGroup
				: public TemporalObject
			{
			public:
				TrollAnimationGroup( wxString const & p_name, wxString const & p_fileName );
				~TrollAnimationGroup();

				TrollAnimatedObject * CreateAnimatedObject( Object * p_object, wxString const & p_fileName );

				void AddChainedAnimations( wxString const & p_anim1, wxString const & p_anim2 );
				void Write( wxTextOutputStream & p_stream );

			private:
				GUI::Properties::ObjectProperty * DoCreateProperties()override;

			private:
				TrollAnimatedObjectStrMap m_objects;
				StrStrMap m_chainedAnimations;
			};
		}
	}
}

#endif
