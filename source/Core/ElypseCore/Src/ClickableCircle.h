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
#ifndef ___CLICKABLE_CIRCLE_H___
#define ___CLICKABLE_CIRCLE_H___

#include "ClickableShape.h"

#include <OgreMath.h>

namespace Elypse
{
	namespace Gui
	{
		class d_dll_export ClickableCircle : public ClickableShape
		{
		protected:
			Real m_radius;

		public:
			ClickableCircle( Real p_radius )
				: m_radius( p_radius )
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
