#ifndef ___MODULE_SIMULATION_H___
#define ___MODULE_SIMULATION_H___

#include "EMuse.h"

namespace EMuse
{
	namespace Simulation
	{
		class ParametricSurface;
		class ParametricLayer;
		class Surface;
		class SurfaceManager;

		typedef std::vector <ParametricSurface *> ParametricSurfaceArray;
	}
}

using namespace EMuse::Simulation;

#endif
