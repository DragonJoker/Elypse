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
#ifndef ___GENLIB_CONFIG_H___
#define ___GENLIB_CONFIG_H___

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#	define GENLIB_LINUX 0
#	define GENLIB_MACOS 0
#	define GENLIB_WINDOWS 1
#	include <sdkddkver.h>
#elif defined(linux) || defined(__linux) || defined(__linux__)
#	define GENLIB_LINUX 1
#	define GENLIB_MACOS 0
#	define GENLIB_WINDOWS 0
#elif defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
#	define GENLIB_LINUX 0
#	define GENLIB_MACOS 1
#	define GENLIB_WINDOWS 0
#endif

#if defined( _MSC_VER)
#	if _MSC_VER >= 1800
#		define GENLIB_HAS_STDTHREAD					1
#		define GENLIB_HAS_STDFUNCTIONAL				1
#		define GENLIB_HAS_TR1FUNCTIONAL				1
#	elif _MSC_VER >= 1700
#		define GENLIB_HAS_STDTHREAD					1
#		define GENLIB_HAS_STDFUNCTIONAL				1
#		define GENLIB_HAS_TR1FUNCTIONAL				1
#	elif _MSC_VER >= 1600
#		define GENLIB_HAS_STDTHREAD					0
#		define GENLIB_HAS_STDFUNCTIONAL				0
#		define GENLIB_HAS_TR1FUNCTIONAL				1
#	elif _MSC_VER > 1500
#		define GENLIB_HAS_STDTHREAD					0
#		define GENLIB_HAS_STDFUNCTIONAL				0
#		define GENLIB_HAS_TR1FUNCTIONAL				0
#	else
#		define GENLIB_HAS_STDTHREAD					0
#		define GENLIB_HAS_STDFUNCTIONAL				0
#		define GENLIB_HAS_TR1FUNCTIONAL				0
#	endif
#elif defined( __GNUG__)
#	define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#	if GCC_VERSION >= 40600
#		if defined( _WIN32 )
#			define GENLIB_HAS_STDTHREAD				0
#			define GENLIB_HAS_STDFUNCTIONAL			1
#			define GENLIB_HAS_TR1FUNCTIONAL			1
#		else
#			define GENLIB_HAS_STDTHREAD				1
#			define GENLIB_HAS_STDFUNCTIONAL			1
#			define GENLIB_HAS_TR1FUNCTIONAL			1
#		endif
#	elif GCC_VERSION >= 40300
#		define GENLIB_HAS_STDTHREAD					0
#		define GENLIB_HAS_STDFUNCTIONAL				0
#		define GENLIB_HAS_TR1FUNCTIONAL				1
#	else
#		define GENLIB_HAS_STDTHREAD					0
#		define GENLIB_HAS_STDFUNCTIONAL				0
#		define GENLIB_HAS_TR1FUNCTIONAL				0
#	endif
#else
#	error "Yet unsupported compiler"
#endif

#endif
