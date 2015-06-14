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
#ifndef ___REFERENCE_COUNTED___
#define ___REFERENCE_COUNTED___

namespace General
{
	namespace Theory
	{
		template< typename T >
		class ReferenceCounted
		{
		protected:
			ReferenceCounted()
				: m_count( 0 )
			{
			}

		public:
			void Use()
			{
				m_count ++;
			}
			void Release()
			{
				m_count --;

				if ( m_count == 0 )
				{
					delete static_cast< T * >( this );
				}
			}

		protected:
			unsigned int m_count;
		};

		template< typename T >
		class ReferenceHolder
		{
		public:
			ReferenceHolder( ReferenceCounted< T > * p_ref )
				: m_ref( p_ref )
			{
				m_ref->Use();
			}

			~ReferenceHolder()
			{
				m_ref->Release();
				m_ref = NULL;
			}

		protected:
			ReferenceCounted< T > * m_ref;
		};
	}
}

#endif
