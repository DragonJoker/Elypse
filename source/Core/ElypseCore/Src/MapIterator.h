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
#ifndef ___MAP_ITERATOR_H___
#define ___MAP_ITERATOR_H___

namespace Elypse
{
	namespace Script
	{
		template <class T>
		class EMMapIterator
		{
		private:
			typename T::iterator mCurrent;
			typename T::iterator mEnd;
			/// Private constructor since only the parameterised constructor should be used
		public:
			EMMapIterator() {};
			typedef typename T::mapped_type MappedType;
			typedef typename T::key_type KeyType;

			/** Constructor.
			@remarks
			    Provide a start and end iterator to initialise.
			*/
			EMMapIterator( typename T::iterator start, typename T::iterator end )
				: mCurrent( start ), mEnd( end )
			{
			}

			/** Constructor.
			@remarks
			    Provide a container to initialise.
			*/
			explicit EMMapIterator( T & c )
				: mCurrent( c.begin() ), mEnd( c.end() )
			{
			}

			/** Returns true if there are more items in the collection. */
			bool hasMoreElements( void ) const
			{
				return mCurrent != mEnd;
			}

			/** Returns the next value element in the collection, and advances to the next. */
			typename T::mapped_type getNext( void )
			{
				return ( mCurrent++ )->second;
			}
			/** Returns the next value element in the collection, without advancing to the next. */
			typename T::mapped_type peekNextValue( void )
			{
				return mCurrent->second;
			}
			/** Returns the next key element in the collection, without advancing to the next. */
			typename T::key_type peekNextKey( void )
			{
				return mCurrent->first;
			}
			/** Required to overcome intermittent bug */
			EMMapIterator<T> & operator = ( EMMapIterator<T> & rhs )
			{
				mCurrent = rhs.mCurrent;
				mEnd = rhs.mEnd;
				return *this;
			}
			/** Returns a pointer to the next value element in the collection, without
			    advancing to the next afterwards. */
			typename T::mapped_type * peekNextValuePtr( void )
			{
				return &( mCurrent->second );
			}
			/** Moves the iterator on one element. */
			void moveNext( void )
			{
				++mCurrent;
			}



		};
	}
}

#endif
