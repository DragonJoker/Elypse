#include "AnimatedObject.h"

#include "Animation.h"

namespace Troll
{
	namespace ProjectComponents
	{
		namespace Temporal
		{
			TrollAnimatedObject::TrollAnimatedObject( Object * p_object, TrollAnimationGroup * p_group, wxString const & p_fileName )
				: TemporalObject( p_object->GetObjectName(), p_fileName, ttAnimatedObject )
				, m_object( p_object )
				, m_group( p_group )
			{
			}

			TrollAnimatedObject::~TrollAnimatedObject()
			{
			}

			void TrollAnimatedObject::AddAnimation( TrollAnimation * p_anim )
			{
				m_animations.insert( std::make_pair( p_anim->GetObjectName(), p_anim ) );
			}

			void TrollAnimatedObject::Write( wxTextOutputStream & p_stream )
			{
				p_stream.WriteString( wxT( "\tadd_object " ) + m_name + wxT( "\n" ) );

				if ( m_animations.size() > 0 )
				{
					p_stream.WriteString( wxT( "\t{\n" ) );

					for ( auto && l_it : m_animations )
					{
						l_it.second->Write( p_stream );
					}

					p_stream.WriteString( wxT( "\t}\n" ) );
				}
			}

			GUI::Properties::ObjectProperty * TrollAnimatedObject::DoCreateProperties()
			{
				return nullptr;
			}
		}
	}
}
