#include "PrecompiledHeader.h"

#include "AnimationManager.h"
#include "AnimatedObjectGroup.h"

AnimationManager :: AnimationManager()
{
}

AnimationManager :: ~AnimationManager()
{
}

void AnimationManager :: Update( Real p_delta )const
{
	General::Utils::map::cycle( m_objectMap, & AnimatedObjectGroup::Update, p_delta );
}
