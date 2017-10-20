/*
See LICENSE file in root folder
*/
#include "PrecompiledHeader.h"

#include "PubManager.h"
#include "PubObject.h"
#include "PhysicsSimulation.h"
#include "PhysicsObject.h"

#include <OgreCamera.h>
#include <OgreSceneManager.h>
#include <OgreEntity.h>

GENLIB_INIT_SINGLETON( PubManager );

PubManager::PubManager()
{
	GENLIB_SET_SINGLETON();
}

PubManager::~PubManager()
{
	General::Utils::map::deleteAll( m_objectList );
}

void PubManager::RemovePubObject( String const & p_name )
{
	General::Utils::map::deleteValue( m_objectList, p_name );
}

void PubManager::OnClick( Real p_x, Real p_y, SceneManager * p_sceneManager, Camera * p_camera, PhysicsSimulation * p_physics )
{
	genlib_assert( p_physics != NULL );
	genlib_assert( p_camera != NULL );
	genlib_assert( p_sceneManager != NULL );

	if ( ! p_camera->isAttached() )
	{
		return;
	}

	Ray l_mouseRay = p_camera->getCameraToViewportRay( p_x, p_y );
	DistanceMap l_result = p_physics->Raytrace( l_mouseRay, false, true, false );

	if ( l_result.empty() )
	{
		return;
	}

	Entity * l_entity = l_result.begin()->second->GetEntity();

	if ( l_entity != NULL )
	{
		PubObjectMap::iterator l_it = m_objectList.begin();

		while ( l_it != m_objectList.end() )
		{
			if ( l_it->second->HasEntity( l_entity->getName() ) )
			{
				l_it->second->OnClick();
			}
			else
			{
				l_it->second->SetUnclicked();
			}

			++ l_it;
		}
	}
}

PubObject * PubManager::CreatePubObject( String const & p_name, ElypsePlugin * p_plugin )
{
	return General::Utils::map::insert( m_objectList, p_name, p_name, p_plugin );
}
