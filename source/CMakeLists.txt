cmake_minimum_required( VERSION 2.8.10 )
#--------------------------------------------------------------------------------------------------
#	Initial configurations
#--------------------------------------------------------------------------------------------------
# Set project name, used in folders and in workspace creation
set( MAIN_PROJECT_NAME "ElypsePlayer" )

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
project( ${MAIN_PROJECT_NAME} )

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
if (WIN32)
	set( Boost_USE_STATIC_LIBS ON)
	set( BOOST_THREAD_USE_LIB ON)
endif()

if (CMAKE_COMPILER_IS_GNUCXX)
	DumpCompilerVersion(Castor_COMPILER_VERSION)
	if (${Castor_COMPILER_VERSION} LESS "46")
		find_package( Boost REQUIRED date_time filesystem system thread)
	else ()
		find_package( Boost REQUIRED date_time filesystem system)
	endif ()
else ()
	find_package( Boost REQUIRED date_time filesystem system)
endif ()

find_package( CURL )
find_package( Ogre )
find_package( ODE )
find_package( Fmod )
find_package( ZLIB 1.2.7 )
find_package( LibZip )

#--------------------------------------------------------------------------------------------------
#	Adding include dirs to include search path
#--------------------------------------------------------------------------------------------------
if ( Boost_FOUND )
	message( STATUS "+ Found Boost" )
	include_directories( ${Boost_INCLUDE_DIRS} )
endif ()
if ( CURL_FOUND )
	message( STATUS "+ Found Curl" )
	include_directories( ${CURL_INCLUDE_DIRS} )
endif ()
if( ZLIB_FOUND )
	message( STATUS "+ Found Zlib (version ${ZLIB_VERSION_STRING})" )
	include_directories( ${ZLIB_INCLUDE_DIRS} )
endif()
if( LIBZIP_FOUND )
	message( STATUS "+ Found libzip" )
	include_directories( ${LIBZIP_INCLUDE_DIRS} )
endif()
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

#Zlib Libs
set( ZlibLibraries "")
foreach(Lib ${ZLIB_LIBRARIES})
	if (ZlibLibraries)
		set(ZlibLibraries "${ZlibLibraries}|${Lib}")
	else()
		set(ZlibLibraries "${Lib}")
	endif()
endforeach()

#libzip Libs
set( LibZipLibraries "")
foreach(Lib ${LIBZIP_LIBRARY})
	if (LibZipLibraries)
		set(LibZipLibraries "${LibZipLibraries}|${Lib}")
	else()
		set(LibZipLibraries "${Lib}")
	endif()
endforeach()

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

macro( parse_subdir_files _FOLDER _GROUP )
	file( GLOB _FOLDER_HEADER_FILES ${_FOLDER}/*.h ${_FOLDER}/*.hpp ${_FOLDER}/*.inl )
	file( GLOB _FOLDER_SOURCE_FILES ${_FOLDER}/*.c ${_FOLDER}/*.cpp )
	source_group( "Header Files\\${_GROUP}" FILES ${_FOLDER_HEADER_FILES} )
	source_group( "Source Files\\${_GROUP}" FILES ${_FOLDER_SOURCE_FILES} )
endmacro()

if( Boost_FOUND AND OGRE_FOUND AND ODE_FOUND AND ZLIB_FOUND AND LIBZIP_FOUND AND CURL_FOUND AND FMOD_FOUND )
	add_subdirectory( Core )
#	add_subdirectory( Plugins )
	add_subdirectory( Binaries )
	add_subdirectory( Tools )
	add_subdirectory( Miniline )
else()
	if ( NOT Boost_FOUND )
		MESSAGE( STATUS "Missing Library : Boost" )
	endif ()
	if ( NOT OGRE_FOUND )
		MESSAGE( STATUS "Missing Library : Ogre" )
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
	if ( NOT ZLIB_FOUND )
		message( SEND_ERROR "Missing ZLib library" )
	endif ()
	if ( NOT LIBZIP_FOUND )
		message( SEND_ERROR "Missing libzip library" )
	endif ()
endif()
