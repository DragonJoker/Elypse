/*
This source file is part of Castor3D (http://castor3d.developpez.com/castor3d.htm)

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
#ifndef ___TROLL_GUI_PROPERTIES_HOLDER_H___
#define ___TROLL_GUI_PROPERTIES_HOLDER_H___

#include "ObjectProperty.h"

#include "Project/2D/Module_2D.h"
#include "Project/3D/Module_3D.h"
#include "Project/Media/Module_Media.h"
#include "Project/Temporal/Module_Temporal.h"

#include <wx/propgrid/propgrid.h>

namespace Troll
{
	namespace GUI
	{
		namespace Properties
		{
			class PropertiesHolder
				: public wxPropertyGrid
			{
			public:
				PropertiesHolder( wxWindow * p_parent, wxPoint const & p_ptPos = wxDefaultPosition, wxSize const & p_size = wxDefaultSize );
				~PropertiesHolder();

				void SetPropertyData( ObjectProperty * p_data );
				/**
				 *\~english
				 *\brief		Retrieves the button editor
				 *\return		The value
				 *\~french
				 *\brief		Récupère l'éditeur bouton
				 *\return		La valeur
				 */
				static inline wxPGEditor * GetButtonEditor()
				{
					return m_buttonEditor;
				}

			private:
				void OnPropertyChange( wxPropertyGridEvent & p_event );

			private:
				ObjectProperty * m_data{ nullptr };
				static wxPGEditor * m_buttonEditor;
			};
		}
	}
}

#endif
