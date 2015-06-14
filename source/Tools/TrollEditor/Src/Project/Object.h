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
#ifndef ___TEOBJECT_H___
#define ___TEOBJECT_H___

#include "Module_Project.h"

#include "Displayable.h"

BEGIN_TROLL_PROJECT_NAMESPACE
{
	class Object
		: public Displayable
	{
	public:
		Object( const wxString & p_name, const wxString & p_fileName, ObjectType p_type );

		inline ObjectType GetObjectType()const
		{
			return m_objectType;
		}
		inline const wxString & GetObjectName()const
		{
			return m_name;
		}
		inline State GetState()const
		{
			return m_state;
		}
		inline bool IsVisible()const
		{
			return m_visible;
		}
		inline const wxString & GetFileName()const
		{
			return m_fileName;
		}
		inline void SetObjectName( const wxString & p_name )
		{
			m_name = p_name;
		}
		inline void SetState( State p_state )
		{
			m_state = p_state;
		}
		inline void SetVisible( bool p_visible )
		{
			m_visible = p_visible;
		}
		inline void SetFileName( const wxString & p_name )
		{
			m_fileName = p_name;
		}

	protected:
		wxString m_name;
		State m_state;
		bool m_visible;
		wxString m_fileName;
		ObjectType m_objectType;
	};
}
END_TROLL_PROJECT_NAMESPACE

#endif
