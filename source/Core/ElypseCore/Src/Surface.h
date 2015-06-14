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
#ifndef ___SURFACE_H___
#define ___SURFACE_H___

#include "Module_Simulation.h"


namespace Elypse
{
	namespace Simulation
	{
		class Surface : d_noncopyable
		{
		protected:
			ManualObject * m_object;
			String m_name;
			SceneManager * m_manager;

		public:
			Surface();
			Surface( const String & p_name, SceneManager * p_manager );
			~Surface();

		public:
			void CreateFromHeightMap( Real * p_heightMap, unsigned int p_width, unsigned int p_height, const String & p_materialName );
			void CreateFromSpaceMap( Vector3 * p_spaceMap,
										unsigned int p_width,
										unsigned int p_height,
										const String & p_materialName,
										bool p_seamNorth = false,
										bool p_seamEast = false,
										bool p_seamSouth = false,
										bool p_seamWest = false );
			void Clear();

		public:
			inline const String & GetName() const
			{
				return m_name;
			}
			inline ManualObject * GetObject() const
			{
				return m_object;
			}
		};
	}
}

#endif
