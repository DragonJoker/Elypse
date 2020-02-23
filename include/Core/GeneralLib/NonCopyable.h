/*
See LICENSE file in root folder
*/
#ifndef ___NON_COPYABLE_H___
#define ___NON_COPYABLE_H___

namespace General
{
	namespace Theory
	{
		class noncopyable
		{
		protected:
			noncopyable() = default;
			~noncopyable() = default;
			noncopyable( const noncopyable & ) = delete;
			const noncopyable & operator=( const noncopyable & ) = delete;
		};
	}
}

#define d_noncopyable private General::Theory::noncopyable
#define d_unique private General::Theory::noncopyable
#define d_debug_check_noncopyable

#endif
