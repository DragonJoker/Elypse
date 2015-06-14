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
#ifndef ___MIRROR_MATERIAL_H___
#define ___MIRROR_MATERIAL_H___

#include "Module_SpecialEffects.h"

namespace Elypse
{
	namespace SpecialEffects
	{
		class d_dll_export MirrorMaterial : public named, public owned_by<Mirror>
		{
			Real m_opacity;
			TextureUnitState * m_tus;
		public:
			MirrorMaterial( const String & p_name, Mirror * p_owner );
			~MirrorMaterial();

		public:
			void Apply();
			void Remove();

		private:
			void _setOpacity();

		public:
			inline void SetOpacity( Real p_opacity )
			{
				m_opacity = p_opacity;
				_setOpacity();
			}
		};
	}
}

#endif
