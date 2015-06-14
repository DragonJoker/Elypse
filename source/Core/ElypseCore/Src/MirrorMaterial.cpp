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

#include "MirrorMaterial.h"
#include "Mirror.h"

using namespace Ogre;

MirrorMaterial::MirrorMaterial( const String & p_name, Mirror * p_owner )
	: named( p_name ),
		owned_by<Mirror>	( p_owner ),
		m_opacity( 0.0 ),
		m_tus( NULL )
{
//	std::cout << "Mirror material ; " << p_name << std::endl;
}

MirrorMaterial::~MirrorMaterial()
{
	if ( m_tus != NULL )
	{
		Remove();
	}
}

void MirrorMaterial::Remove()
{
	m_tus->getParent()->removeTextureUnitState( m_tus->getParent()->getTextureUnitStateIndex( m_tus ) );
	m_tus = NULL;
}

void MirrorMaterial::_setOpacity()
{
	if ( m_tus != NULL )
	{
		m_tus->setColourOperationEx( LBX_BLEND_MANUAL, LBS_TEXTURE, LBS_CURRENT, ColourValue::White, ColourValue::White, m_opacity );
	}
}

void MirrorMaterial::Apply()
{
	if ( m_tus != NULL )
	{
		return;
	}

	Material * l_material = static_cast <MaterialPtr>( MaterialManager::getSingletonPtr()->getByName( m_name ) ).getPointer();

	if ( l_material != NULL )
	{
		m_tus = l_material->getTechnique( 0 )->getPass( 0 )->createTextureUnitState( "__INTERNAL_MirrorTexture_" + m_owner->GetName() );
		m_tus->setColourOperationEx( LBX_BLEND_MANUAL, LBS_TEXTURE, LBS_CURRENT, ColourValue::White, ColourValue::White, m_opacity );
		m_tus->setTextureAddressingMode( TextureUnitState::TAM_CLAMP );
		m_tus->setProjectiveTexturing( true, m_owner->GetCamera() );
	}
}
