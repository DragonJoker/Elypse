/*
See LICENSE file in root folder
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

			uint32_t m_depth;
			uint32_t m_resolution;
			uint32_t m_quadrant;

		public:
			ParametricLayer( Vector3 * p_controlPoints, uint32_t p_resolution,
							 ParametricLayer * p_parent, uint32_t p_quadrant, uint32_t p_depth );
			~ParametricLayer();

		public:
			void CreateAtDeph( uint32_t p_depth );
			void CreateSubLayer( uint32_t p_quadrant );
			uint32_t CountFinalPoints()const;
			void Draw( Surface * p_surface );

			void CutToDepth( uint32_t p_depth );
			bool UpdateResolution( Camera * p_camera );
			void RemoveQuadrant( uint32_t p_quadrant );

		private:
			void PrecalcCoefficients();
			ParametricLayer * GetNeighbour( uint32_t p_quadrant, uint32_t p_side )const;
			void CreateSurface( uint32_t p_quadrant );
			void CalculateQuadrant( uint32_t p_quadrant );
			void DrawQuadrant( uint32_t p_quadrant, Surface * p_object );

		public:
			inline ParametricLayer * GetSubLayer( uint32_t p_index )
			{
				return m_subLayers[p_index];
			}
			inline void SetBoundNorth( uint32_t p_index, ParametricLayer * p_neighbour )
			{
				m_bounds[p_index] = p_neighbour;
			}
		};
	}
}

#endif
