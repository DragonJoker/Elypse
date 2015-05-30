#ifndef ___GENERAL_LIB_THREAD_H___
#define ___GENERAL_LIB_THREAD_H___

#include "MultiThreadConfig.h"

#include "BoostHeaders.h"

#if GENLIB_MT_USE_BOOST || defined( __GNUG__)

namespace General
{
	namespace MultiThreading
	{
		typedef boost::thread Thread;

#	ifndef GENLIB_THREAD_INIT
#		define GENLIB_THREAD_INIT()
#	endif

		template <class TClass>
		class ClassFunctor
		{
		public:
			TClass * m_instance;
			void( TClass::* m_function )();

		public:
			ClassFunctor( TClass * p_instance, void( TClass::* p_function )() )
				:	m_instance( p_instance ),
					m_function( p_function )
			{
			}
			void operator()()const
			{
				GENLIB_THREAD_INIT();
				( m_instance->* m_function )();
			}
		};
	}
}

#	define GENLIB_THREAD_CLASS_FUNCTOR( p_instance, p_class, p_function)		General::MultiThreading::ClassFunctor <p_class> ( p_instance, & p_class::p_function)
#	define GENLIB_THREAD_DECLARE_THREAD_FUNC									void BeginThread()
#	define GENLIB_THREAD_DEFINE_THREAD_FUNC( p_class)							void p_class::BeginThread()
#	define GENLIB_THREAD_CREATE_CLASS_THREAD( p_class, p_instance)				new General::MultiThreading::Thread( GENLIB_THREAD_CLASS_FUNCTOR( p_instance, p_class, BeginThread))
#	define GENLIB_THREAD_CREATE_MEMBER_FUNC_THREAD( p_class, p_instance, p_func)new General::MultiThreading::Thread( GENLIB_THREAD_CLASS_FUNCTOR( p_instance, p_class, p_func))
#	define GENLIB_THREAD_DELETE_THREAD( p_thread)								delete p_thread
#	define GENLIB_THREAD_WAIT_FOR_END_OF( p_thread)								p_thread->join()

#elif GENLIB_MT_USE_MFC

#	ifndef WINVER
#		define WINVER 0x0501
#	endif

#	include <afxwin.h>

namespace General
{
	namespace MultiThreading
	{
		typedef CWinThread Thread;
	}
}

#	define GENLIB_THREAD_DECLARE_THREAD_FUNC						static unsigned int BeginThread( void * p_instance)
#	define GENLIB_THREAD_DEFINE_THREAD_FUNC( p_class)				unsigned int  p_class::BeginThread ( void * p_instance)
#	define GENLIB_THREAD_CREATE_CLASS_THREAD( p_class, p_instance)	AfxBeginThread( p_class::BeginThread, static_cast <void *> ( p_instance), 0, 0, 0, 0)
#	define GENLIB_THREAD_CREATE_FUNC_THREAD( p_func)				AfxBeginThread( p_func, 0, 0, 0, 0, 0)
#	define GENLIB_THREAD_DELETE_THREAD( p_thread)					delete ( p_thread)

#endif

#endif
