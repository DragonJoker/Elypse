#include "AnimationGroup.h"

#include "AnimatedObject.h"

namespace Troll
{
	namespace ProjectComponents
	{
		namespace Temporal
		{
			TrollAnimationGroup::TrollAnimationGroup( wxString const & p_name, wxString const & p_fileName )
				: TemporalObject( p_name, p_fileName, ttAnimationGroup )
			{
			}

			TrollAnimationGroup::~TrollAnimationGroup()
			{
				General::Utils::map::deleteAll( m_objects );
			}

			TrollAnimatedObject * TrollAnimationGroup::CreateAnimatedObject( Object * p_object, wxString const & p_fileName )
			{
				TrollAnimatedObject * l_return = nullptr;
				auto && l_it = m_objects.find( p_object->GetObjectName() );

				if ( l_it != m_objects.end() )
				{
					l_return = l_it->second;
				}
				else
				{
					l_return = new TrollAnimatedObject( p_object, this, m_fileName );
					m_objects.insert( std::make_pair( p_object->GetObjectName(), l_return ) );
				}

				return l_return;
			}

			void TrollAnimationGroup::AddChainedAnimations( wxString const & p_anim1, wxString const & p_anim2 )
			{
				m_chainedAnimations.insert( std::make_pair( p_anim1, p_anim2 ) );
			}

			void TrollAnimationGroup::Write( wxTextOutputStream & p_stream )
			{
				p_stream.WriteString( wxT( "animation_group " ) + m_name + wxT( "\n{\n" ) );
				General::Utils::map::cycle( m_objects, &TrollAnimatedObject::Write, p_stream );

				if ( m_chainedAnimations.size() > 0 )
				{
					p_stream.WriteString( wxT( "\n" ) );

					for ( auto && l_it : m_chainedAnimations )
					{
						p_stream.WriteString( wxT( "\tchain_animations " ) + l_it.first + wxT( " " ) + l_it.second + wxT( "\n" ) );
					}
				}

				p_stream.WriteString( wxT( "}\n\n" ) );
			}

			GUI::Properties::ObjectProperty * TrollAnimationGroup::DoCreateProperties()
			{
				return nullptr;
			}
		}
	}
}
