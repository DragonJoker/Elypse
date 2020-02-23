/*
See LICENSE file in root folder
*/
#include "PrecompiledHeader.h"

#include "Portal.h"
#include "Space.h"

Portal::Portal( String const & p_name )
	: named( p_name )
	, m_active( true )
	, m_trigger( nullptr )
{
}

Portal::~Portal()
{
}

void Portal::SetSpace( Space * p_space )
{
	genlib_assert( p_space != nullptr );
	genlib_assert( m_trigger != nullptr );
	p_space->AddObject( m_trigger );
}
