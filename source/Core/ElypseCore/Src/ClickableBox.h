#ifndef ___CLICKABLE_BOX_H___
#define ___CLICKABLE_BOX_H___

#include "ClickableShape.h"

namespace EMuse
{
	namespace Gui
	{
		class d_dll_export ClickableBox : public ClickableShape
		{
		protected:
			Real m_width;
			Real m_height;

		public:
			ClickableBox( Real p_width, Real p_height )
				:	m_width( p_width ),
					m_height( p_height )
			{}
			virtual ~ClickableBox() {}

		public:
			virtual bool TestClick( Real p_x, Real p_y )
			{
				return ( p_x >= m_x && p_x <= ( m_x + m_width )
						 && p_y >= m_y && p_y <= ( m_y + m_height ) );
			}
		};
	}
}

#endif
