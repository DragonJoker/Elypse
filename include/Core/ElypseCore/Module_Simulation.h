/*
See LICENSE file in root folder
*/
#ifndef ___MODULE_SIMULATION_H___
#define ___MODULE_SIMULATION_H___

#include "Elypse.h"

namespace Elypse
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

using namespace Elypse::Simulation;

#endif
