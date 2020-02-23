/*
See LICENSE file in root folder
*/
#include "PrecompiledHeader.h"

#include "PubInstance.h"
#include "PubObject.h"
#include "Zone.h"

#include <OgreEntity.h>
#include <OgreSubEntity.h>

PubInstance::PubInstance( PubObject & p_owner, Entity * p_entity )
	: owned_by< PubObject >( p_owner )
	, m_entity( p_entity )
	, m_zone( nullptr )
	, m_submat( 0 )
{
}

PubInstance::~PubInstance()
{
	if ( m_zone != nullptr )
	{
		m_zone->RemoveZoneObject( this );
	}
}

void PubInstance::Apply()
{
	if ( m_submat >= m_entity->getNumSubEntities() )
	{
		m_entity->setMaterialName( GetOwner()->GetName() );
	}
	else
	{
		m_entity->getSubEntity( m_submat )->setMaterialName( GetOwner()->GetName() );
	}
}

void PubInstance::RemoveFromZone( Zone * p_zone )
{
	GetOwner()->RemoveInstance( this );
	m_zone = nullptr;
}
