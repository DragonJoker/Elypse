/*
See LICENSE file in root folder
*/
#include "PrecompiledHeader.h"

#include "MaterialTarget.h"

#include "EMOverlay.h"

#include <OgreEntity.h>
#include <OgreSubEntity.h>

void MaterialTarget_Entity::Apply( String const & p_materialName )
{
	m_entity->setMaterialName( p_materialName );
}

void MaterialTarget_SubEntity::Apply( String const & p_materialName )
{
	m_subEntity->setMaterialName( p_materialName );
}

void MaterialTarget_Overlay::Apply( String const & p_materialName )
{
	m_overlay->SetBaseMaterial( p_materialName );
}
