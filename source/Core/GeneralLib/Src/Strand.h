#ifndef ___GENLIB_STRAND_H___
#define ___GENLIB_STRAND_H___

#include "Thread.h"
#include "Mutex.h"

namespace General
{
	namespace MultiThreading
	{
		class Strand;

		template <class TClass>
		class StrandFunctor
		{
		private:
			Strand * m_strand;
			TClass * m_instance;
			void ( TClass::* m_function )();

		public:
			StrandFunctor( Strand * p_strand, const ClassFunctor<TClass> & p_functor )
				:	m_strand( p_strand ),
					m_instance( p_functor.m_instance ),
					m_function( p_functor.m_function )
			{
			}
		public:
			void operator()();
		};

		class Strand
		{
		public:
			Mutex m_mutex;

		public:
			template<typename TClass>
			StrandFunctor<TClass> Synchronise( const ClassFunctor<TClass> & p_functor )
			{
				return StrandFunctor<TClass>( this, p_functor );
			}
		};

		template<class TClass>
		void StrandFunctor<TClass>::operator()()
		{
			GENLIB_THREAD_INIT();
			boost::mutex::scoped_lock scoped_lock( m_strand->m_mutex );
			( m_instance->* m_function )();
		}
	}
}

#endif
