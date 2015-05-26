#ifndef ___PAIRED_MAP_H___
#define ___PAIRED_MAP_H___

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/composite_key.hpp>

#include "Tuples.h"

/**
 * Interesting one :
 * Allows you to create a map-like container, which lets you find the 3rd element from a pair of the first 2.
**/

namespace General
{
	namespace Templates
	{
		template <typename T, typename U, typename V>
		struct PairedMap :
				boost::multi_index_container
				<
				Tuple3 <T, U, V>,
				boost::multi_index::indexed_by <
				boost::multi_index::ordered_unique <
				boost::multi_index::composite_key
				<
				Tuple3 <T, U, V>,
				boost::multi_index::member <Tuple3 <T, U, V>, T, & Tuple3 <T, U, V> ::first>,
				boost::multi_index::member <Tuple3 <T, U, V>, U, & Tuple3 <T, U, V> ::second>
				>
				>
				>
				>
		{
			typedef Tuple3			<T, U, V>	value_type;
			typedef boost::tuple	<T, U>		KeyType;
		};
	}
}

#endif
