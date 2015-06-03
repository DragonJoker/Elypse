#include "PrecompiledHeader.h"

#include "SequenceProperties.h"

#include "Project/Temporal/Sequence.h"
#include "GUI/MainFrame.h"

using namespace Troll::GUI;
using namespace Troll::Temporal;

enum SequencePropsIDs
{
	sequenceName
};

extern MainFrame * g_mainFrame;

BEGIN_EVENT_TABLE( SequenceProperties, wxPanel )
	EVT_TEXT_ENTER(	sequenceName,					SequenceProperties::_onEditName )
END_EVENT_TABLE()


SequenceProperties::SequenceProperties( TrollSequence * p_sequence, wxWindow * parent, const wxPoint & pos,
										const wxSize & size, long style )
	:	ObjectProperties( parent, pos, size, style ),
		m_sequence( p_sequence )
{
	wxPoint l_position = wxPoint( size.x / 2, 0 );
	m_sequenceName = BuildTextCtrl( this, l_position, sequenceName, wxT( "Name" ), m_sequence->TrollObject::GetName() );
}



SequenceProperties::~SequenceProperties()
{
}



void SequenceProperties::UpdateProperties()
{
	m_updatingProperties = true;
	m_sequenceName->SetValue( m_sequence->TrollObject::GetName() );
	m_updatingProperties = false;
}



void SequenceProperties::_onEditName( wxCommandEvent & p_event )
{
	if ( ! m_updatingProperties )
	{
		m_sequence->SetName( m_sequenceName->GetValue() );
	}
}


