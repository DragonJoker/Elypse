cmake_minimum_required( VERSION 2.8 )
#--------------------------------------------------------------------------------------------------
#	Initial configurations
#--------------------------------------------------------------------------------------------------
# Set project name, used in folders and in workspace creation
set( ProjectName "ElypsePlayer" )

# Set project version numbers
set( VERSION_MAJOR	0		)
set( VERSION_MINOR	9		)
set( VERSION_BUILD	4		)
set( VERSION_YEAR	2014	)

# Used to look for external modules
set( CMAKE_MODULE_PATH
	"${CMAKE_SOURCE_DIR}/CMake"
	"${CMAKE_SOURCE_DIR}/CMake/Modules"
)
# Experimental Precompiled headers support for GCC
include( PCHSupport )
# Force Unicode projects (MBCS is past :P)
add_definitions( -DUNICODE -D_UNICODE )

set( CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Set only Debug and Release configuration types" FORCE )

# Enable CTest
enable_testing()

# Declare the project
project( ${ProjectName} )

include( Project )
include( CompilerVersion )
include( UnitTest )
include( CompilationFlags )
include( AStyleUtils )
include( ExternalDependencies )

# Organize projects into folders
set_property( GLOBAL PROPERTY USE_FOLDERS ON )

#--------------------------------------------------------------------------------------------------
#	Looking for needed libraries
#--------------------------------------------------------------------------------------------------
if (NOT MINGW)
	set( Boost_USE_STATIC_LIBS ON)
	set( BOOST_THREAD_USE_LIB ON)
endif()

if (CMAKE_COMPILER_IS_GNUCXX)
	COMPILER_DUMPVERSION(Castor_COMPILER_VERSION)
	if (${Castor_COMPILER_VERSION} LESS "46")
		find_package( Boost REQUIRED date_time filesystem system thread)
	else ()
		find_package( Boost REQUIRED date_time filesystem system)
	endif ()
else ()
	find_package( Boost REQUIRED date_time filesystem system)
endif ()

find_package( CURL )
find_package( ZZip )
find_package( ZLIB )
find_package( Ogre )
find_package( ODE )
find_package( FMOD )

#--------------------------------------------------------------------------------------------------
#	Adding include dirs to include search path
#--------------------------------------------------------------------------------------------------
if ( Boost_FOUND )
	message( STATUS "+ Found Boost" )
	include_directories( ${Boost_INCLUDE_DIRS} )
endif ()
if ( ZZIP_FOUND )
	message( STATUS "+ Found ZZIP" )
	include_directories( ${ZZip_INCLUDE_DIR} )
endif ()
if ( CURL_FOUND )
	message( STATUS "+ Found Curl" )
	include_directories( ${CURL_INCLUDE_DIRS} )
endif ()
if ( ZLIB_FOUND )
	message( STATUS "+ Found ZLib" )
	include_directories( ${ZLIB_INCLUDE_DIRS} )
endif ()
if ( OGRE_FOUND )
	message( STATUS "+ Found Ogre" )
	include_directories( ${Ogre_INCLUDE_DIR} )
endif ()
if ( ODE_FOUND )
	message( STATUS "+ Found ODE" )
	include_directories( ${ODE_INCLUDE_DIR} )
endif ()
if ( FMOD_FOUND )
	message( STATUS "+ Found FMOD" )
	include_directories( ${Fmod_INCLUDE_DIR} )
endif ()

#--------------------------------------------------------------------------------------------------
#	Adding libraries dirs to link search path
#--------------------------------------------------------------------------------------------------
link_directories( ${LIBRARY_OUTPUT_PATH} )
link_directories( ${Boost_LIBRARY_DIRS} )
link_directories( ${ZZip_LIBRARY_DIR} )
#link_directories( ${Ogre_LIBRARY_DIR} )
link_directories( ${ODE_LIBRARY_DIR} )
link_directories( ${Fmod_LIBRARY_DIR} )
if ( NOT WIN32 )
	link_directories( ${GSTREAMER_LIBRARY_DIR} )
	link_directories( ${LIBXML_LIBRARY_DIR} )
endif ()

#--------------------------------------------------------------------------------------------------
#	Normalising libraries names
#--------------------------------------------------------------------------------------------------

set( CurlLibraries "" )
FOREACH( Lib ${CURL_LIBRARIES} )
	if ( CurlLibraries )
		set( CurlLibraries "${CurlLibraries}|${Lib}" )
	else()
		set( CurlLibraries "${Lib}" )
	endif()
ENDFOREACH()

set( BoostLibraries "")
FOREACH(Lib ${Boost_LIBRARIES})
	if (BoostLibraries)
		set(BoostLibraries "${BoostLibraries}|${Lib}")
	else()
		set(BoostLibraries "${Lib}")
	endif()
ENDFOREACH()

set( ZZipLibraries "")
FOREACH(Lib ${ZZip_LIBRARIES})
	if (ZZipLibraries)
		set(ZZipLibraries "${ZZipLibraries}|${Lib}")
	else()
		set(ZZipLibraries "${Lib}")
	endif()
ENDFOREACH()

set( ZlibLibraries "" )
FOREACH( Lib ${ZLIB_LIBRARIES} )
	if ( ZlibLibraries )
		set( ZlibLibraries "${ZlibLibraries}|${Lib}" )
	else()
		set( ZlibLibraries "${Lib}" )
	endif()
ENDFOREACH()

set( OgreLibraries "")
FOREACH(Lib ${Ogre_LIBRARIES})
	if (OgreLibraries)
		set(OgreLibraries "${OgreLibraries}|${Lib}")
	else()
		set(OgreLibraries "${Lib}")
	endif()
ENDFOREACH()

set( OdeLibraries "")
FOREACH(Lib ${ODE_LIBRARIES})
	if (OdeLibraries)
		set(OdeLibraries "${OdeLibraries}|${Lib}")
	else()
		set(OdeLibraries "${Lib}")
	endif()
ENDFOREACH()

set( FmodLibraries "" )
FOREACH(Lib ${Fmod_LIBRARIES} )
	if ( FmodLibraries )
		set( FmodLibraries "${FmodLibraries}|${Lib}" )
	else()
		set( FmodLibraries "${Lib}" )
	endif()
ENDFOREACH()

if( MSVC )
	# We remove the existing /Zm flag to replace it by /Zm300
	string( REGEX REPLACE ".*(/Zm[0-9]+).*" "\\1" ZmFlag ${CMAKE_CXX_FLAGS} )
	if( (NOT "${ZmFlag}" STREQUAL "") AND (NOT "${CMAKE_CXX_FLAGS}" STREQUAL "${ZmFlag}") )
		string( REPLACE ${ZmFlag} "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} )
	endif()
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zm300" )
	# We remove the existing /Zm flag to replace it by /Zm300
	string( REGEX REPLACE ".*(/Zm[0-9]+).*" "\\1" ZmFlag ${CMAKE_C_FLAGS} )
	if( (NOT "${ZmFlag}" STREQUAL "") AND (NOT "${CMAKE_C_FLAGS}" STREQUAL "${ZmFlag}") )
		string( REPLACE ${ZmFlag} "" CMAKE_C_FLAGS ${CMAKE_C_FLAGS} )
	endif()
	set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /Zm300" )
endif()

msg_debug( "CMAKE_BUILD_TYPE : ${CMAKE_BUILD_TYPE}")
msg_debug( "CMAKE_C_FLAGS    : ${CMAKE_C_FLAGS}" )
msg_debug( "CMAKE_CXX_FLAGS  : ${CMAKE_CXX_FLAGS}" )

if( Boost_FOUND AND OGRE_FOUND AND ODE_FOUND AND ZLIB_FOUND AND ZZIP_FOUND AND CURL_FOUND AND FMOD_FOUND )
	add_subdirectory( Core )
#	add_subdirectory( Plugins )
	add_subdirectory( Binaries )
else()
	if ( NOT Boost_FOUND )
		MESSAGE( STATUS "Missing Library : Boost" )
	endif ()
	if ( NOT OGRE_FOUND )
		MESSAGE( STATUS "Missing Library : Ogre" )
	endif ()
	if ( NOT ZLIB_FOUND )
		MESSAGE( STATUS "Missing Library : Zlib" )
	endif ()
	if ( NOT ZZIP_FOUND )
		MESSAGE( STATUS "Missing Library : ZZiplib" )
	endif ()
	if ( NOT CURL_FOUND )
		MESSAGE( STATUS "Missing Library : Curl" )
	endif ()
	if ( NOT ODE_FOUND )
		MESSAGE( STATUS "Missing Library : ODE" )
	endif ()
	if ( NOT FMOD_FOUND )
		MESSAGE( STATUS "Missing Library : FMOD" )
	endif ()
endif()