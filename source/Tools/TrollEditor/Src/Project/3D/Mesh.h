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
#ifndef _TrollMesh_H_
#define _TrollMesh_H_

#include "Module_3D.h"

namespace Troll
{
	namespace ProjectComponents
	{
		namespace Objects3D
		{
			class TrollMesh
			{
			public:
				TrollMesh( wxString const & p_meshName );
				~TrollMesh();

				void AddSubMaterial( wxString const & p_name );
				void SetSubMaterial( wxString const & p_name, uint32_t p_index );
				wxString GetMaterial( uint32_t p_index );

				inline wxString const & GetName()const
				{
					return m_meshName;
				}
				inline uint32_t GetNumSubMaterials()const
				{
					return uint32_t( m_subMaterials.size() );
				}

			private:
				wxString m_meshName;
				wxStringUIntMap m_subMaterials;
			};
		}
	}
}

#endif
