/*
See LICENSE file in root folder
*/
#ifndef ___GENERAL_LIB_THREAD_H___
#define ___GENERAL_LIB_THREAD_H___

#include "MultiThreadConfig.h"

#include <functional>
#include <thread>

#define GENLIB_THREAD_CLASS_FUNCTOR( p_instance, p_class, p_function ) std::bind( &p_class::p_function, p_instance )
#define GENLIB_THREAD_DECLARE_THREAD_FUNC void BeginThread()
#define GENLIB_THREAD_DEFINE_THREAD_FUNC( p_class ) void p_class::BeginThread()
#define GENLIB_THREAD_CREATE_CLASS_THREAD( p_class, p_instance ) new std::thread( GENLIB_THREAD_CLASS_FUNCTOR( p_instance, p_class, BeginThread ) )
#define GENLIB_THREAD_CREATE_MEMBER_FUNC_THREAD( p_class, p_instance, p_func ) new std::thread( GENLIB_THREAD_CLASS_FUNCTOR( p_instance, p_class, p_func ) )
#define GENLIB_THREAD_DELETE_THREAD( p_thread ) delete p_thread
#define GENLIB_THREAD_WAIT_FOR_END_OF( p_thread ) p_thread->join()

#endif
