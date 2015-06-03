
#ifndef ___TrollAnimation___
#define ___TrollAnimation___

#include "Module_Temporal.h"
#include "TemporalObject.h"

#include <Module_Animation.h>

namespace Troll
{
	namespace Temporal
	{
		class TrollAnimation
			: public TemporalObject
		{
		private:
			EMuse::Animation::EMAnimation * m_museAnimation;
			bool m_looped;
			float m_timein;
			float m_timeout;

		public:
			TrollAnimation( const wxString & p_name, const wxString & p_fileName );
			~TrollAnimation();

			virtual int BuildPanel( wxWindow * p_parent, int p_width );

			void SetMuseAnimation( EMuse::Animation::EMAnimation * p_animation );

			void Write( wxTextOutputStream * p_stream );

		public:
			inline void SetLooped( bool p_looped )
			{
				m_looped = p_looped;
			}
		};
	}
}

#endif

