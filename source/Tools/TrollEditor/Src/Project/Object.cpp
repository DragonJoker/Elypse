/*
See LICENSE file in root folder
*/
#include "TrollEditorPch.h"

#include "Object.h"

namespace Troll
{
	namespace ProjectComponents
	{
		Object::Object( wxString const & p_name, wxString const & p_fileName, ObjectType p_type )
			: Displayable()
			, m_name( p_name )
			, m_state( Normal )
			, m_visible( true )
			, m_objectType( p_type )
			, m_fileName( p_fileName )
		{
		}
	}
}
