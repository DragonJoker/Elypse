#include "PrecompiledHeader.h"

#include "MirrorMaterial.h"
#include "Mirror.h"

using namespace Ogre;

MirrorMaterial::MirrorMaterial( const String & p_name, Mirror * p_owner )
	:	named( p_name ),
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
