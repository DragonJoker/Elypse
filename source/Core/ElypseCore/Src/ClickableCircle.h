#ifndef ___CLICKABLE_CIRCLE_H___
#define ___CLICKABLE_CIRCLE_H___

#include "ClickableShape.h"

#include <OgreMath.h>

namespace EMuse
{
	namespace Gui
	{
		class d_dll_export ClickableCircle : public ClickableShape
		{
		protected:
			Real m_radius;

		public:
			ClickableCircle( Real p_radius )
				:	m_radius( p_radius )
			{}
			virtual ~ClickableCircle() {}

		public:
			virtual bool TestClick( Real p_x, Real p_y )
			{
				Real l_diffX = m_x - p_x;
				Real l_diffY = m_y - p_y;
				return ( m_radius >= Math::Sqrt( l_diffX * l_diffX + l_diffY * l_diffY ) );
			}
		};
	}
}

#endif
