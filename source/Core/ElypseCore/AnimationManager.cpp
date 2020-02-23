/*
See LICENSE file in root folder
*/
#include "PrecompiledHeader.h"

#include "AnimationManager.h"
#include "AnimatedObjectGroup.h"

AnimationManager::AnimationManager()
{
}

AnimationManager::~AnimationManager()
{
}

void AnimationManager::Update( Real p_delta )const
{
	for ( auto l_it : m_objectMap )
	{
		l_it.second->Update( p_delta );
	}
}
