/*
This source file is part of ElypsePlayer (https://sourceforge.net/projects/elypse/)

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
the program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
*/
#include "TrollEditorPch.h"

#include "SequenceProperties.h"

#include "Project/Temporal/Sequence.h"
#include "GUI/MainFrame.h"

BEGIN_TROLL_GUI_PROPERTIES_NAMESPACE
{
	enum SequencePropsIDs
	{
		sequenceName
	};

	BEGIN_EVENT_TABLE( SequenceProperties, wxPanel )
		EVT_TEXT_ENTER( sequenceName, SequenceProperties::OnEditName )
	END_EVENT_TABLE()

	SequenceProperties::SequenceProperties( TROLL_PROJECT_TEMPORAL_NAMESPACE::TrollSequence * p_sequence, wxWindow * parent, const wxPoint & pos, const wxSize & size, long style )
		: ObjectProperties( parent, pos, size, style )
		, m_sequence( p_sequence )
	{
		wxPoint l_position = wxPoint( size.x / 2, 0 );
		m_sequenceName = BuildTextCtrl( this, l_position, sequenceName, _( "Name" ), m_sequence->Object::GetObjectName() );
	}

	SequenceProperties::~SequenceProperties()
	{
	}

	void SequenceProperties::UpdateProperties()
	{
		m_updatingProperties = true;
		m_sequenceName->SetValue( m_sequence->Object::GetObjectName() );
		m_updatingProperties = false;
	}

	void SequenceProperties::OnEditName( wxCommandEvent & p_event )
	{
		if ( ! m_updatingProperties )
		{
			m_sequence->SetObjectName( m_sequenceName->GetValue() );
		}
	}
}
END_TROLL_GUI_PROPERTIES_NAMESPACE
