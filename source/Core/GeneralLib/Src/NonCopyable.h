#ifndef ___NON_COPYABLE_H___
#define ___NON_COPYABLE_H___

namespace General
{
	namespace Theory
	{
		class noncopyable
		{
		protected:
			noncopyable() {}
			~noncopyable() {}

		private:
			noncopyable( const noncopyable & );
			const noncopyable & operator =( const noncopyable & );
		};
	}
}

#define d_noncopyable	private General::Theory::noncopyable
#define d_unique		private General::Theory::noncopyable
#define d_debug_check_noncopyable

#endif
