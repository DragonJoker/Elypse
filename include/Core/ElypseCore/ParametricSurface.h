/*
See LICENSE file in root folder
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

			uint32_t m_resolution;
			uint32_t m_numControlPoints;
			uint32_t m_currentControlPointIndex;
			uint32_t m_currentDepth;

//		String m_name;

		public:
			ParametricSurface( SceneManager * p_manager, String const & p_name, uint32_t p_resolution, uint32_t p_numControlPoints );
			~ParametricSurface();

		public:
			void Draw();
			void AddControlPoint( const Vector3 & p_controlPoint );
			void CreateAtDepth( uint32_t p_depth );

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
