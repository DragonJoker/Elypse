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
#ifndef ___GENERAL_OWNED_BY_H___
#define ___GENERAL_OWNED_BY_H___

namespace General
{
	namespace Theory
	{
		template< typename T >
		class owned_by
		{
		public:
			owned_by( T * p_owner = NULL )
				: m_owner( p_owner )
			{
			}

			~owned_by()
			{
			}

			inline T * GetOwner()const
			{
				return m_owner;
			}

			inline void SetOwner( T * p_owner )
			{
				m_owner = p_owner;
			}

		protected:
			T * m_owner;
		};
	}
}

#endif
