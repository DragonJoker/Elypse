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
#ifndef ___Troll_ContinuousEventProperties___
#define ___Troll_ContinuousEventProperties___

#include "ObjectProperties.h"

BEGIN_TROLL_GUI_PROPERTIES_NAMESPACE
{
	class ContinuousEventProperties
	: public ObjectProperties
	{
	public:
		ContinuousEventProperties( TROLL_PROJECT_TEMPORAL_NAMESPACE::TrollContinuousEvent & p_event, wxWindow * parent, const wxPoint & pos = wxDefaultPosition, const wxSize & size = wxDefaultSize, long style = wxBORDER_NONE );
		~ContinuousEventProperties();

		virtual void UpdateProperties();

	protected:
		wxArrayString DoGetTargetTypes( const wxString & p_action );
		wxString DoBuildTargetChoice( const wxPoint & p_position, const wxString & p_targetType );
		wxString DoBuildInterpolationChoice( const wxPoint & p_position );
		void DoBuildKeyFramesList( const wxPoint & p_position );

	private:
		void OnSelectAction( wxCommandEvent & p_event );
		void OnSelectTargetType( wxCommandEvent & p_event );
		void OnEditTarget( wxCommandEvent & p_event );
		void OnSelectInterpolation( wxCommandEvent & p_event );
		void OnSelectKey( wxCommandEvent & p_event );
		DECLARE_EVENT_TABLE()

	private:
		TROLL_PROJECT_TEMPORAL_NAMESPACE::TrollContinuousEvent & m_event;
		TROLL_PROJECT_TEMPORAL_NAMESPACE::TrollKeyFrame * m_viewedKeyFrame;

		wxComboBox * m_actionName;
		wxComboBox * m_targetType;
		void * m_target;
		wxComboBox * m_interpolation;
		wxComboBox * m_keys;
		wxPanel * m_keyPanel;
	};
}
END_TROLL_GUI_PROPERTIES_NAMESPACE

#endif
