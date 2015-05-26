#ifndef ___PARAMETRIC_SURFACE_H___
#define ___PARAMETRIC_SURFACE_H___

#include "Module_Simulation.h"
#include "Surface.h"

namespace EMuse
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
