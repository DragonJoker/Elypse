#ifndef ___UNI_LIST_H___
#define ___UNI_LIST_H___

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/hashed_index.hpp>

namespace General
{
	namespace Templates
	{
		template <typename T>
		struct UniList :
				boost::multi_index_container
				<
				T,
				boost::multi_index::indexed_by
				<boost::multi_index::sequenced <>, boost::multi_index::ordered_unique <boost::multi_index::identity <T>>>
				>
		{}
	}
}

#endif
