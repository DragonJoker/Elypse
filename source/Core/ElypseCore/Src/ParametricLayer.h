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
#ifndef ___PARAMETRIC_LAYER_H___
#define ___PARAMETRIC_LAYER_H___

#include "Module_Simulation.h"

namespace Elypse
{
	namespace Simulation
	{
		class ParametricLayer : d_noncopyable
		{
		private:
			Real m_positionX;
			Real m_positionY;
			Real m_sizeX;
			Real m_sizeY;

			Real * m_precalcX;
			Real * m_precalcY;

			AxisAlignedBox * m_boxes[4];

			ParametricLayer * m_subLayers[4];
			Vector3 * m_points[4];
			Vector3 * m_controlPoints;

			ParametricLayer * m_parent;
			ParametricLayer * m_bounds[8];

			unsigned int m_depth;
			unsigned int m_resolution;
			unsigned int m_quadrant;

		public:
			ParametricLayer( Vector3 * p_controlPoints, unsigned int p_resolution,
								ParametricLayer * p_parent, unsigned int p_quadrant, unsigned int p_depth );
			~ParametricLayer();

		public:
			void CreateAtDeph( unsigned int p_depth );
			void CreateSubLayer( unsigned int p_quadrant );
			unsigned int CountFinalPoints()const;
			void Draw( Surface * p_surface );

			void CutToDepth( unsigned int p_depth );
			bool UpdateResolution( Camera * p_camera );
			void RemoveQuadrant( unsigned int p_quadrant );

		private:
			void PrecalcCoefficients();
			ParametricLayer * GetNeighbour( unsigned int p_quadrant, unsigned int p_side )const;
			void CreateSurface( unsigned int p_quadrant );
			void CalculateQuadrant( unsigned int p_quadrant );
			void DrawQuadrant( unsigned int p_quadrant, Surface * p_object );

		public:
			inline ParametricLayer * GetSubLayer( unsigned int p_index )
			{
				return m_subLayers[p_index];
			}
			inline void SetBoundNorth( unsigned int p_index, ParametricLayer * p_neighbour )
			{
				m_bounds[p_index] = p_neighbour;
			}
		};
	}
}

#endif
