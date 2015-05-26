#ifndef ___RANDOM_ACCESS_MAP_H___
#define ___RANDOM_ACCESS_MAP_H___

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/random_access_index.hpp>

/**
 * A weird mix between a map and a vector.
 * it allows for very fast iteration over all the elements( use the [] operator), as well as very fast finds.
 * It does comes at a price tho : very slow insertion.
**/

namespace General
{
	namespace Templates
	{
		template <typename U, typename V>
		struct RaMap :
				boost::multi_index_container
				<
				std::pair <U, V>,
				boost::multi_index::indexed_by
				<boost::multi_index::ordered_unique
				<boost::multi_index::member
				<std::pair <U, V>, U, & std::pair <U, V> ::first>
				>,
				boost::multi_index::random_access <>
				>
				>
		{
		};
	}
}

#endif