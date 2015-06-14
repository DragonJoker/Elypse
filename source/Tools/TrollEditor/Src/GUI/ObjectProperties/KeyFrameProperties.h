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
#ifndef ___Troll_KeyFrameProperties___
#define ___Troll_KeyFrameProperties___

#include "ObjectProperties.h"

BEGIN_TROLL_GUI_PROPERTIES_NAMESPACE
{
	class KeyFrameProperties
		: public ObjectProperties
	{
	public:
		KeyFrameProperties( TROLL_PROJECT_TEMPORAL_NAMESPACE::TrollKeyFrame * p_keyFrame, wxWindow * parent, const wxPoint & pos = wxDefaultPosition, const wxSize & size = wxDefaultSize, long style = wxBORDER_NONE );
		~KeyFrameProperties();

		virtual void UpdateProperties();

	protected:
		void _buildArguments( const wxPoint & p_position );

	private:
		DECLARE_EVENT_TABLE()
		void OnEditFireTime( wxCommandEvent & p_event );
		void OnEditReal( wxCommandEvent & p_event );
		void OnEditV3X( wxCommandEvent & p_event );
		void OnEditV3Y( wxCommandEvent & p_event );
		void OnEditV3Z( wxCommandEvent & p_event );

	private:
		TROLL_PROJECT_TEMPORAL_NAMESPACE::TrollKeyFrame * m_key;
		wxTextCtrl * m_fireTime;
		wxTextCtrl * m_real;
		wxTextCtrl * m_v3X;
		wxTextCtrl * m_v3Y;
		wxTextCtrl * m_v3Z;
	};
}
END_TROLL_GUI_PROPERTIES_NAMESPACE

#endif
