#include "PrecompiledHeader.h"

#include "MirrorManager.h"

#include "Mirror.h"

MirrorManager::MirrorManager( const String & p_instanceKey )
	:	m_instanceKey( p_instanceKey )
{
}

MirrorManager::~MirrorManager()
{
}

void MirrorManager::UpdateAll()const
{
	General::Utils::map::cycle( m_objectMap, & Mirror::Update );
}
