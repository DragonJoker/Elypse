/*
This source file is part of ElypsePlayer (https://sourceforge.net/projects/elypse/)

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
the program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
*/
#ifndef ___GENERAL_LIB_THREAD_H___
#define ___GENERAL_LIB_THREAD_H___

#include "MultiThreadConfig.h"

#include <functional>
#include <thread>

namespace General
{
	namespace MultiThreading
	{
		typedef std::thread Thread;
	}
}

#define GENLIB_THREAD_CLASS_FUNCTOR( p_instance, p_class, p_function ) std::bind( &p_class::p_function, p_instance )
#define GENLIB_THREAD_DECLARE_THREAD_FUNC void BeginThread()
#define GENLIB_THREAD_DEFINE_THREAD_FUNC( p_class ) void p_class::BeginThread()
#define GENLIB_THREAD_CREATE_CLASS_THREAD( p_class, p_instance ) new General::MultiThreading::Thread( GENLIB_THREAD_CLASS_FUNCTOR( p_instance, p_class, BeginThread ) )
#define GENLIB_THREAD_CREATE_MEMBER_FUNC_THREAD( p_class, p_instance, p_func ) new General::MultiThreading::Thread( GENLIB_THREAD_CLASS_FUNCTOR( p_instance, p_class, p_func ) )
#define GENLIB_THREAD_DELETE_THREAD( p_thread ) delete p_thread
#define GENLIB_THREAD_WAIT_FOR_END_OF( p_thread ) p_thread->join()

#endif
