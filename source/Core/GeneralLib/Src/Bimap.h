#ifndef ___BIMAP_H___
#define ___BIMAP_H___

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>
#include <utility>				//for std::pair

/**
 * Bi directional map.
 *
 * usage : , for instance :
 * Bimap<std::string, class*> l_map
 * l_map.find( "name");
 * l_map.mirror().find( ptr);
**/

namespace General
{
	namespace Templates
	{
		template <typename T, typename U>
		struct Bimap :
				boost::multi_index_container
				<
				std::pair <T, U>,
				boost::multi_index::indexed_by
				<
				boost::multi_index::ordered_unique <boost::multi_index::member <std::pair <T, U>, T, & std::pair <T, U> ::first>>,
						boost::multi_index::ordered_unique <boost::multi_index::member <std::pair <T, U>, U, & std::pair <T, U> ::second>>
						>
						>
		{
		public:
			typedef typename nth_index <1> ::type Mirror;
			typedef std::pair <T, U> value_type;

		public:
			inline Mirror & mirror()
			{
				return get <1> ();
			}
			inline const Mirror & mirror()const
			{
				return get <1> ();
			}
		};
	}
}

#endif
