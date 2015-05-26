#ifndef ___PAIRED_MAP_H___
#define ___PAIRED_MAP_H___

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/composite_key.hpp>

namespace General
{
	namespace Templates
	{
		template <typename T, typename U>
		struct PairedSet :
				boost::multi_index_container
				<
				std::pair <T, U>,
				boost::multi_index::indexed_by
				<boost::multi_index::ordered_unique
				<boost::multi_index::composite_key
				<
				std::pair <T, U>,
				boost::multi_index::member <std::pair <T, U>, T, & std::pair <T, U> ::first>,
				boost::multi_index::member <std::pair <T, U>, U, & std::pair <T, U> ::second>
				>
				>
				>
				>
		{
			typedef std::pair <T, U> value_type;
			typedef std::pair <T, U> KeyType;
		};
	}
}

#endif
