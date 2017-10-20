/*
See LICENSE file in root folder
*/
#ifndef ___SURFACE_MANAGER_H___
#define ___SURFACE_MANAGER_H___

#include "Module_Simulation.h"

#include <Manager.h>

namespace Elypse
{
	namespace Simulation
	{
		class d_dll_export SurfaceManager : public Manager <ParametricSurface>
		{
		private:
			Camera * m_camera;

		public:
			SurfaceManager();
			~SurfaceManager();

		public:
			void UpdateAllSurfaces();

		public:
			inline void SetCamera( Camera * p_camera )
			{
				m_camera = p_camera;
			}
		};
	}
}

#endif
