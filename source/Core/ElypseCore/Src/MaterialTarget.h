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
#ifndef ___MATERIAL_TARGET_H___
#define ___MATERIAL_TARGET_H___

#include "Module_SpecialEffects.h"
#include "Module_Gui.h"

namespace Elypse
{
	namespace SpecialEffects
	{
		class MaterialTarget
		{
		public:
			virtual ~MaterialTarget() {}
		public:
			virtual void Apply( const String & p_materialName ) = 0;
		};

		class MaterialTarget_Entity : public MaterialTarget
		{
		private:
			Entity * m_entity;

		public:
			MaterialTarget_Entity( Entity * p_entity )
				: m_entity( p_entity )
			{
			}
			virtual ~MaterialTarget_Entity() {}

		public:
			virtual void Apply( const String & p_materialName );
		};

		class MaterialTarget_SubEntity : public MaterialTarget
		{
		private:
			SubEntity * m_subEntity;

		public:
			MaterialTarget_SubEntity( SubEntity * p_subEntity )
				: m_subEntity( p_subEntity )
			{
			}
			virtual ~MaterialTarget_SubEntity() {}

		public:
			virtual void Apply( const String & p_materialName );
		};

		class MaterialTarget_Overlay : public MaterialTarget
		{
		private:
			EMOverlay * m_overlay;

		public:
			MaterialTarget_Overlay( EMOverlay * p_overlay )
				: m_overlay( p_overlay )
			{
			}
			virtual ~MaterialTarget_Overlay() {}

		public:
			virtual void Apply( const String & p_materialName );
		};
	}
}

#endif
