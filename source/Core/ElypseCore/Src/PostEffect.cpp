#include "PrecompiledHeader.h"

#include "PostEffect.h"

#include <OgreCompositorManager.h>

PostEffect::PostEffect( const String & p_name, Viewport * p_viewport, int p_position, bool p_autocreate )
	:	named( p_name ),
		m_viewport( p_viewport )
{
	if ( p_autocreate )
	{
		m_instance = CompositorManager::getSingleton().addCompositor( m_viewport, m_name, p_position );
		SetEnabled( true );
	}
}

PostEffect::~PostEffect()
{
	SetEnabled( false );
}

void PostEffect::SetEnabled( bool p_enable )
{
	if ( m_instance != NULL )
	{
		CompositorManager::getSingleton().setCompositorEnabled( m_viewport, m_name, p_enable );
	}
}
