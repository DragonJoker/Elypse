/*
See LICENSE file in root folder
*/
#include "TrollEditorPch.h"

#include "TemporalObject.h"

namespace Troll
{
	namespace ProjectComponents
	{
		namespace Temporal
		{
			TemporalObject::TemporalObject( wxString const & p_name, wxString const & p_fileName, TemporalObjectType p_type )
				: Object( p_name, p_fileName, ProjectComponents::ttTemporalObject )
				, m_temporalObjectType( p_type )
			{
				wxString l_msg = wxT( "Creating Temporal object : " );

				if ( p_type == ttAnimationGroup )
				{
					l_msg += wxT( "Animation Group - " );
				}
				else if ( p_type == ttAnimatedObject )
				{
					l_msg += wxT( "Animated Object - " );
				}
				else if ( p_type == ttAnimation )
				{
					l_msg += wxT( "Animation - " );
				}

				l_msg += wxT( "Name : " ) + p_name;
				LogMessage( l_msg );
			}

			TemporalObject::~TemporalObject()
			{
			}
		}
	}
}
