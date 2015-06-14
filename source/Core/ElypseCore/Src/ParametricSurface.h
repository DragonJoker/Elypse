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
#ifndef ___PARAMETRIC_SURFACE_H___
#define ___PARAMETRIC_SURFACE_H___

#include "Module_Simulation.h"
#include "Surface.h"

namespace Elypse
{
	namespace Simulation
	{
		class ParametricSurface : protected Surface
		{
		private:
			ParametricLayer * m_firstLayer;
			Vector3 * m_controlPoints;
			SceneManager * m_manager;
			AxisAlignedBox * m_box;
//		ManualObject * m_object;

			unsigned int m_resolution;
			unsigned int m_numControlPoints;
			unsigned int m_currentControlPointIndex;
			unsigned int m_currentDepth;

//		String m_name;

		public:
			ParametricSurface( SceneManager * p_manager, const String & p_name, unsigned int p_resolution, unsigned int p_numControlPoints );
			~ParametricSurface();

		public:
			void Draw();
			void AddControlPoint( const Vector3 & p_controlPoint );
			void CreateAtDepth( unsigned int p_depth );

			void Update( Camera * p_camera );

			using Surface::GetName;

		public:
			inline ParametricLayer * GetLayer()const
			{
				return m_firstLayer;
			}
		};
	}
}

#endif
