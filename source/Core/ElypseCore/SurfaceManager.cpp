/*
See LICENSE file in root folder
*/
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
	for ( auto l_it : m_objectMap )
	{
		l_it.second->Update( m_camera );
	}
}
