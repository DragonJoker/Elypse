#include "Animation.h"

using namespace Elypse::Animation;

namespace Troll
{
	namespace ProjectComponents
	{
		namespace Temporal
		{
			TrollAnimation::TrollAnimation( wxString const & p_name, wxString const & p_fileName )
				: TemporalObject( p_name, p_fileName, ttAnimation )
				, m_looped( false )
				, m_timein( 0.0 )
				, m_timeout( 0.0 )
			{
			}

			TrollAnimation::~TrollAnimation()
			{
			}

			void TrollAnimation::SetMuseAnimation( EMAnimation * p_animation )
			{
				m_museAnimation = p_animation;
				m_looped = p_animation->IsLooped();
				m_timein = p_animation->GetTimeIn();
				m_timeout = p_animation->GetTimeOut();
			}

			void TrollAnimation::Write( wxTextOutputStream & p_stream )
			{
				wxString l_tmp;
				p_stream.WriteString( wxT( "\t\tanimation " ) + m_name + wxT( "\n\t\t{\n" ) );

				if ( m_looped )
				{
					p_stream.WriteString( wxT( "\t\t\tlooped true\n" ) );
				}

				if ( m_timein != 0.0 )
				{
					l_tmp << m_timein;
					p_stream.WriteString( wxT( "\t\t\ttimein " ) + l_tmp + wxT( "\n" ) );
					l_tmp.clear();
				}

				if ( m_timeout != 0.0 )
				{
					l_tmp << m_timeout;
					p_stream.WriteString( wxT( "\t\t\ttimein " ) + l_tmp + wxT( "\n" ) );
					l_tmp.clear();
				}

				p_stream.WriteString( wxT( "\t\t}\n" ) );
			}

			GUI::Properties::ObjectProperty * TrollAnimation::DoCreateProperties()
			{
				return nullptr;
			}
		}
	}
}
