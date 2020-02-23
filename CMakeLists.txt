cmake_minimum_required( VERSION 3.15 )

# Set project name, used in folders and in workspace creation
set( MAIN_PROJECT_NAME "ElypsePlayer" )

# Set project version numbers
set( VERSION_MAJOR	0		)
set( VERSION_MINOR	9		)
set( VERSION_BUILD	4		)
set( VERSION_YEAR	2014	)

# Used to look for external modules
set( CMAKE_MODULE_PATH
	${CMAKE_SOURCE_DIR}/CMake
	${CMAKE_SOURCE_DIR}/CMake/Modules
)
# Experimental Precompiled headers support for GCC
include( PCHSupport )

set( CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Set only Debug and Release configuration types" FORCE )

# Enable CTest
enable_testing()

# Declare the project
project( ${MAIN_PROJECT_NAME} )

include( Setup )
include( Project )
include( CompilerVersion )
include( UnitTest )
include( CompilationFlags )
include( AStyleUtils )
include( ExternalDependencies )

if ( MSVC OR NOT "${CMAKE_BUILD_TYPE}" STREQUAL "" )
	# Organize projects into folders
	set_property( GLOBAL PROPERTY USE_FOLDERS ON )

	#--------------------------------------------------------------------------------------------------
	#	Looking for needed libraries
	#--------------------------------------------------------------------------------------------------
	find_package( Boost REQUIRED date_time filesystem system )
	find_package( CURL CONFIG REQUIRED )
	if ( WIN32 )
		find_package( Ogre CONFIG REQUIRED OgreMain OgreOverlay Codec_STBI )
	else ()
		find_package( Ogre CONFIG REQUIRED OgreMain OgreOverlay Plugin_ParticleFX RenderSystem_GL Codec_STBI )
	endif ()
	find_package( ODE CONFIG REQUIRED )
	find_package( Fmod REQUIRED )
	find_package( zlib REQUIRED )
	find_package( libzip REQUIRED )
	find_package( Freetype CONFIG REQUIRED )

	if ( NOT WIN32 )
		find_package( GTK2 )
		find_package( X11 )
	else ()
		set( GTK2_FOUND TRUE )
	endif ( NOT WIN32 )

	#--------------------------------------------------------------------------------------------------
	#	Normalising libraries names
	#--------------------------------------------------------------------------------------------------
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
		file( GLOB _${_FOLDER}_HEADER_FILES ${_FOLDER}/*.h ${_FOLDER}/*.hpp ${_FOLDER}/*.inl )
		file( GLOB _${_FOLDER}_SOURCE_FILES ${_FOLDER}/*.c ${_FOLDER}/*.cpp )
		source_group( "Header Files\\${_GROUP}" FILES ${_${_FOLDER}_HEADER_FILES} )
		source_group( "Source Files\\${_GROUP}" FILES ${_${_FOLDER}_SOURCE_FILES} )
	endmacro()

	add_subdirectory( source )
else()
	message( SEND_ERROR "Please select a build type (Debug or Release)" )
endif()

