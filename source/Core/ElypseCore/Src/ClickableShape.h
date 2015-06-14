/*
This source file is part of ElypsePlayer (https://sourceforge.net/projects/elypse/)

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
the program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
*/
#ifndef ___CLICKABLE_SHAPE_H___
#define ___CLICKABLE_SHAPE_H___

#include "Module_Gui.h"

namespace Elypse
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
