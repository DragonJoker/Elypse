/*
See LICENSE file in root folder
*/
#include "PrecompiledHeader.h"

#include "MirrorManager.h"

#include "Mirror.h"

MirrorManager::MirrorManager( String const & p_instanceKey )
	: m_instanceKey( p_instanceKey )
{
}

MirrorManager::~MirrorManager()
{
}

void MirrorManager::UpdateAll()const
{
	for ( auto l_it : m_objectMap )
	{
		l_it.second->Update();
	}
}
