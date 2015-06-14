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
#ifndef ___Troll_PonctualEventProperties___
#define ___Troll_PonctualEventProperties___

#include "ObjectProperties.h"

BEGIN_TROLL_GUI_PROPERTIES_NAMESPACE
{
	class PonctualEventProperties
		: public ObjectProperties
	{
	public:
		PonctualEventProperties( TROLL_PROJECT_TEMPORAL_NAMESPACE::TrollPonctualEvent * p_event, wxWindow * parent, const wxPoint & pos = wxDefaultPosition, const wxSize & size = wxDefaultSize, long style = wxBORDER_NONE );
		~PonctualEventProperties();

		virtual void UpdateProperties();

	protected:
		wxArrayString _getTargetTypes( const wxString & p_action );
		wxString _buildTargetChoice( const wxPoint & p_position, const wxString & p_targetType );
		void _buildActionArguments( wxPoint & p_position, const wxString & p_action );

	private:
		DECLARE_EVENT_TABLE()
		void OnSelectAction( wxCommandEvent & p_event );
		void OnSelectTargetType( wxCommandEvent & p_event );
		void OnEditTarget( wxCommandEvent & p_event );
		void OnEditTime( wxCommandEvent & p_event );
		void OnEditXParam( wxCommandEvent & p_event );
		void OnEditYParam( wxCommandEvent & p_event );
		void OnEditZParam( wxCommandEvent & p_event );

	private:
		TROLL_PROJECT_TEMPORAL_NAMESPACE::TrollPonctualEvent * m_event;
		wxComboBox * m_actionName;
		wxComboBox * m_targetType;
		wxComboBox * m_targetCB;
		wxTextCtrl * m_targetTC;
		wxTextCtrl * m_fireTime;
		wxTextCtrl * m_v3ArgX;
		wxTextCtrl * m_v3ArgY;
		wxTextCtrl * m_v3ArgZ;
	};
}
END_TROLL_GUI_PROPERTIES_NAMESPACE

#endif

