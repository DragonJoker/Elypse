#ifndef ___TUPLES_H___
#define ___TUPLES_H___

namespace General
{
	namespace Templates
	{
		template <typename Type1, typename Type2, typename Type3>
		struct Tuple3
		{
		public:
			Tuple3(	const Type1 & p_first, const Type2 & p_second,
					const Type3 & p_third )
				:	first( p_first ),
					second( p_second ),
					third( p_third )
			{}

		public:
			Type1 first;
			Type2 second;
			Type3 third;
		};

		template <typename Type1, typename Type2, typename Type3, typename Type4>
		struct Tuple4
		{
			Tuple4(	const Type1 & p_first, const Type2 & p_second,
					const Type3 & p_third, const Type4 & p_fourth )
				:	first( p_first ),
					second( p_second ),
					third( p_third ),
					fourth( p_fourth )
			{}

		public:
			Type1 first;
			Type2 second;
			Type3 third;
			Type4 fourth;
		};
	}
}

#endif
