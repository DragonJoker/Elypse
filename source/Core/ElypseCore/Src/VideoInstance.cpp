/*
This source file is part of ElypsePlayer (https://sourceforge.net/projects/elypse/)

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
the program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
*/
#include "PrecompiledHeader.h"

#include "VideoInstance.h"
#include "VideoObject.h"

VideoInstance::VideoInstance( VideoObject & p_owner, Entity * p_entity )
	: owned_by< VideoObject >( p_owner )
	, m_entity( p_entity )
	, m_noSubMaterial( -1 )
	, m_delayLoad( false )
{
	genlib_assert( m_entity != NULL );
}

VideoInstance::~VideoInstance()
{
}

void VideoInstance::Initialise( String const & p_videoTextureName )
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

void VideoInstance::SetNoSubMaterial( uint16_t p_noSubMat )
{
	if ( p_noSubMat > m_entity->getNumSubEntities() )
	{
		m_noSubMaterial = -1;
		return;
	}

	m_noSubMaterial = signed( p_noSubMat );
}
