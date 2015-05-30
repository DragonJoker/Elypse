#include "PrecompiledHeader.h"

#include "VideoInstance.h"
#include "VideoObject.h"

VideoInstance::VideoInstance( VideoObject * p_owner, Entity * p_entity )
	:	owned_by <VideoObject>	( p_owner ),
		 m_entity( p_entity ),
		 m_noSubMaterial( -1 ),
		 m_delayLoad( false )
{
	genlib_assert( m_entity != NULL );
	genlib_assert( m_owner != NULL );
}

VideoInstance::~VideoInstance()
{
}

void VideoInstance::Initialise( const String & p_videoTextureName )
{
	if ( m_noSubMaterial < 0 )
	{
		m_oldMaterialName = m_entity->getSubEntity( 0 )->getMaterialName();
		m_entity->setMaterialName( p_videoTextureName );
		return;
	}

	SubEntity * l_subEntity = m_entity->getSubEntity( m_noSubMaterial );
	m_oldMaterialName = l_subEntity->getMaterialName();
	l_subEntity->setMaterialName( p_videoTextureName );
}

void VideoInstance::Reset()
{
	if ( m_noSubMaterial < 0 )
	{
		m_entity->setMaterialName( m_oldMaterialName );
		return;
	}

	m_entity->getSubEntity( m_noSubMaterial )->setMaterialName( m_oldMaterialName );
}

void VideoInstance::SetNoSubMaterial( unsigned short p_noSubMat )
{
	if ( p_noSubMat > m_entity->getNumSubEntities() )
	{
		m_noSubMaterial = -1;
		return;
	}

	m_noSubMaterial = signed( p_noSubMat );
}
