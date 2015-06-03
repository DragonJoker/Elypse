#include "PrecompiledHeader.h"

#include "AnimationGroup.h"
#include "AnimatedObject.h"

using namespace Troll::Temporal;


TrollAnimationGroup::TrollAnimationGroup( const wxString & p_name, const wxString & p_fileName )
	:	TemporalObject( p_name, p_fileName, ttAnimationGroup )
{
}



TrollAnimationGroup::~TrollAnimationGroup()
{
	General::Utils::map::deleteAll( m_objects );
}



TrollAnimatedObject * TrollAnimationGroup::CreateAnimatedObject( TrollObject * p_object, const wxString & p_fileName )
{
	TrollAnimatedObjectStrMap::iterator l_it = m_objects.find( p_object->GetName() );

	if ( l_it != m_objects.end() )
	{
		return l_it->second;
	}

	TrollAnimatedObject * l_object = new TrollAnimatedObject( p_object, this, m_fileName );
	m_objects.insert( TrollAnimatedObjectStrMap::value_type( p_object->GetName(), l_object ) );
	return l_object;
}



int TrollAnimationGroup::BuildPanel( wxWindow * p_parent, int p_width )
{
	return 0;
}



void TrollAnimationGroup::AddChainedAnimations( const wxString & p_anim1, const wxString & p_anim2 )
{
	m_chainedAnimations.insert( StrStrMap::value_type( p_anim1, p_anim2 ) );
}



void TrollAnimationGroup::Write( wxTextOutputStream * p_stream )
{
	p_stream->WriteString( wxT( "animation_group " ) + m_name + wxT( "\n{\n" ) );
	General::Utils::map::cycle( m_objects, & TrollAnimatedObject::Write, p_stream );

	if ( m_chainedAnimations.size() > 0 )
	{
		p_stream->WriteString( wxT( "\n" ) );
		StrStrMap::iterator l_it = m_chainedAnimations.begin();

		for ( ; l_it != m_chainedAnimations.end() ; ++l_it )
		{
			p_stream->WriteString( wxT( "\tchain_animations " ) + l_it->first + wxT( " " ) + l_it->second + wxT( "\n" ) );
		}
	}

	p_stream->WriteString( wxT( "}\n\n" ) );
}


