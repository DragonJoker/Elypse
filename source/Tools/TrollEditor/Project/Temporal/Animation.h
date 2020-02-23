/*
See LICENSE file in root folder
*/
#ifndef ___TrollAnimation___
#define ___TrollAnimation___

#include "TemporalObject.h"

#include <Module_Animation.h>

namespace Troll
{
	namespace ProjectComponents
	{
		namespace Temporal
		{
			class TrollAnimation
				: public TemporalObject
			{
			public:
				TrollAnimation( wxString const & p_name, wxString const & p_fileName );
				~TrollAnimation();

				void SetMuseAnimation( Elypse::Animation::EMAnimation * p_animation );

				void Write( wxTextOutputStream & p_stream );

				inline void SetLooped( bool p_looped )
				{
					m_looped = p_looped;
				}

			private:
				GUI::Properties::ObjectProperty * DoCreateProperties()override;

			private:
				Elypse::Animation::EMAnimation * m_museAnimation;
				bool m_looped;
				float m_timein;
				float m_timeout;
			};
		}
	}
}

#endif
