#include "PrecompiledHeader.h"

#include "PubInstance.h"
#include "PubObject.h"
#include "Zone.h"

#include <OgreEntity.h>
#include <OgreSubEntity.h>

PubInstance::PubInstance( PubObject * p_owner, Entity * p_entity )
	:	owned_by <PubObject>	( p_owner ),
		   m_entity( p_entity ),
		   m_zone( NULL ),
		   m_submat( 0 )
{
}

PubInstance::~PubInstance()
{
	if ( m_zone != NULL )
	{
		m_zone->RemoveZoneObject( this );
	}
}

void PubInstance::Apply()
{
	if ( m_submat >= m_entity->getNumSubEntities() )
	{
		m_entity->setMaterialName( m_owner->GetName() );
	}
	else
	{
		m_entity->getSubEntity( m_submat )->setMaterialName( m_owner->GetName() );
	}
}

void PubInstance::RemoveFromZone( Zone * p_zone )
{
	m_owner->RemoveInstance( this );
	m_zone = NULL;
}
