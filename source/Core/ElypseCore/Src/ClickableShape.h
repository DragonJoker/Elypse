#ifndef ___CLICKABLE_SHAPE_H___
#define ___CLICKABLE_SHAPE_H___

#include "Module_Gui.h"

namespace EMuse
{
	namespace Gui
	{
		class d_dll_export ClickableShape
		{
		protected:
			Real m_x;
			Real m_y;

		public:
			virtual ~ClickableShape() {};

		public:
			virtual bool TestClick( Real p_x, Real p_y ) = 0;

			void SetPosition( Real p_x, Real p_y )
			{
				m_x = p_x;
				m_y = p_y;
			}

		public:
			inline Real GetPosX()const
			{
				return m_x;
			}
			inline Real GetPosY()const
			{
				return m_y;
			}
		};
	}
}

#endif
