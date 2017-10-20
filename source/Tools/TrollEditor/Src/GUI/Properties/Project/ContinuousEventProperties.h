/*
See LICENSE file in root folder
*/
#ifndef ___Troll_ContinuousEventProperties___
#define ___Troll_ContinuousEventProperties___

#include "GUI/Properties/Common/ObjectProperty.h"

namespace Troll
{
	namespace GUI
	{
		namespace Properties
		{
			class ContinuousEventProperties
				: public ObjectProperty
			{
			public:
				ContinuousEventProperties( ProjectComponents::Temporal::TrollContinuousEvent & p_event );
				~ContinuousEventProperties();

			private:
				/**
				 *\copydoc		Troll::GUI::Properties::ObjectProperty::DoCreateProperties
				 */
				virtual void DoCreateProperties( wxPGEditor * p_editor );

			protected:
				wxArrayString DoGetTargetTypes( wxString const & p_action );
				void DoCreateTargetProperties();
				void DoCreateInterpolationProperties();
				void DoCreateKeyFramesProperties();
				void DoRemoveKeyFrameProperties();

			private:
				void DoUpdateTargetType();
				void DoUpdateTarget();

			private:
				ProjectComponents::Temporal::TrollContinuousEvent & m_event;
				ProjectComponents::Temporal::TrollKeyFrame * m_viewedKeyFrame{ nullptr };
				wxPGProperty * m_targetType{ nullptr };
			};
		}
	}
}

#endif
