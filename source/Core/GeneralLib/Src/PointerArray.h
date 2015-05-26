#ifndef ___POINTER_ARRAY_H___
#define ___POINTER_ARRAY_H___

#include <vector>
#include "STLMacros.h"

namespace General
{
	namespace Templates
	{
		template <class T>
		class PointerArray
		{
		private:
			std::vector <T *> m_objects;

		public:
			PointerArray() {}
			~PointerArray()
			{
				clear();
			}

		public:
			inline T * operator []( unsigned int p_index )
			{
				return m_objects[p_index];
			}

			inline size_t size()const
			{
				return m_objects.size();
			}

			inline void push_back( T * p_element )
			{
				m_objects.push_back( p_element );
			}

			inline T * remove( unsigned int p_index )
			{
				std::vector <T *> ::iterator l_it = m_objects.begin() + p_index;
				T * l_obj = ( * l_it );
				m_objects.erase( l_it );
				return l_obj;
			}

			inline void del( unsigned int p_index )
			{
				m_objects.erase( m_objects.begin() + p_index );
			}

			inline void clear()
			{
				General::Utils::vector::deleteAll( m_objects )
			}
		};
	}
}

#endif
