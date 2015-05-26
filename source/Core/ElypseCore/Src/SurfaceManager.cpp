#include "PrecompiledHeader.h"

#include "SurfaceManager.h"
#include "ParametricSurface.h"

SurfaceManager::SurfaceManager()
{
}

SurfaceManager::~SurfaceManager()
{
}

void SurfaceManager::UpdateAllSurfaces()
{
	General::Utils::map::cycle( m_objectMap, & ParametricSurface::Update, m_camera );
}
