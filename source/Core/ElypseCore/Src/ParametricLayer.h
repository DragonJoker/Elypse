#ifndef ___PARAMETRIC_LAYER_H___
#define ___PARAMETRIC_LAYER_H___

#include "Module_Simulation.h"

namespace EMuse
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
			ParametricLayer(	Vector3 * p_controlPoints, unsigned int p_resolution,
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
