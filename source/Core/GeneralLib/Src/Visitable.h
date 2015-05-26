#ifndef ___VISITABLE_H___
#define ___VISITABLE_H___

#include "Macros.h"

namespace General
{
	namespace Theory
	{
		template <typename T>
		d_interface Visitor
		{
public:
			virtual void Visits( T * p_visited ) = 0;
		};

		template <typename T>
		class Visitable
		{
		public:
			void AcceptVisitor( Visitor <T> * p_visitor )
			{
				p_visitor->Visits( reinterpret_cast <T *>( this ) );
			}
		};
	}
}

#endif
