/*
See LICENSE file in root folder
*/
#ifndef ___POST_EFFECT_H___
#define ___POST_EFFECT_H___

#include "Module_SpecialEffects.h"

namespace Elypse
{
	namespace SpecialEffects
	{
		class PostEffect : public named, noncopyable
		{
		protected:
			Viewport * m_viewport;
			CompositorInstance * m_instance;

		public:
			PostEffect( String const & p_name, Viewport * p_viewport, int p_position = -1, bool p_autocreate = true );
			virtual ~PostEffect();

		public:
			virtual void SetEnabled( bool p_enable );
		};
	}
}

#endif
