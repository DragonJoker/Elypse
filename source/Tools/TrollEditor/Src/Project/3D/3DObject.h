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
#ifndef ___TE3DOBJECT_H___
#define ___TE3DOBJECT_H___

#include "Module_3D.h"

#include "Project/Object.h"

namespace Troll
{
	namespace ProjectComponents
	{
		namespace Objects3D
		{
			class Troll3DObject
				: public Object
			{
			public:
				Troll3DObject( wxString const & p_name, wxString const & p_fileName, Troll3DObjectType p_type );
				virtual ~Troll3DObject();

				virtual void AttachTo( TrollSceneNode * p_node );
				void Detach();

				inline Troll3DObjectType Get3DObjectType()const
				{
					return m_3DObjectType;
				}
				inline TrollSceneNode * GetParent()const
				{
					return m_parent;
				}
				inline void SetParent( TrollSceneNode * p_parent )
				{
					m_parent = p_parent;
				}

			protected:
				TrollSceneNode * m_parent;
				Troll3DObjectType m_3DObjectType;
			};
		}
	}
}

#endif
