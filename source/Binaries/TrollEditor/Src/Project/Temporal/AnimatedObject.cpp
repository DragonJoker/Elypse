
#include "PrecompiledHeader.h"

#include "Project/Temporal/AnimatedObject.h"
#include "Project/Temporal/Animation.h"

using namespace Troll::Temporal;


TrollAnimatedObject::TrollAnimatedObject( TrollObject * p_object, TrollAnimationGroup * p_group, const wxString & p_fileName )
	:	TemporalObject( p_object->GetName(), p_fileName, ttAnimatedObject ),
		m_object( p_object ),
		m_group( p_group )
{
}



TrollAnimatedObject::~TrollAnimatedObject()
{
}



int TrollAnimatedObject::BuildPanel( wxWindow * p_parent, int p_width )
{
	return 0;
}



void TrollAnimatedObject::AddAnimation( TrollAnimation * p_anim )
{
	m_animations.insert( std::make_pair( p_anim->GetName(), p_anim ) );
}



void TrollAnimatedObject::Write( wxTextOutputStream * p_stream )
{
	p_stream->WriteString( wxT( "\tadd_object " ) + m_name + wxT( "\n" ) );

	if ( m_animations.size() > 0 )
	{
		p_stream->WriteString( wxT( "\t{\n" ) );
		std::for_each( m_animations.begin(), m_animations.end(), [&]( std::pair< wxString, TrollAnimation * > p_pair )
		{
			p_pair.second->Write( p_stream );
		} );
		p_stream->WriteString( wxT( "\t}\n" ) );
	}
}


