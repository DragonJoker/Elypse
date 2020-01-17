cmake_minimum_required( VERSION 3.15 )
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
		find_package( Ogre CONFIG REQUIRED OgreMain OgreOverlay )
	else ()
		find_package( Ogre CONFIG REQUIRED OgreMain OgreOverlay Plugin_ParticleFX RenderSystem_GL )
	endif ()
	find_package( ODE CONFIG REQUIRED )
	find_package( Fmod REQUIRED )
	find_package( zlib REQUIRED )
	find_package( libzip CONFIG REQUIRED )
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

	include_directories( ${CMAKE_SOURCE_DIR}/Core/GeneralLib/Src )

	add_subdirectory( Core )
	add_subdirectory( Tools )
	add_subdirectory( Miniline )
	add_subdirectory( Plugins )

	CoreInit()
	ToolsInit()
	MinilineInit()
	PluginsInit()

	CoreBuild()
	ToolsBuild()
	MinilineBuild()
	PluginsBuild()

	set( msg "\nElypse following projects will be built" )
	CoreSummary( ${msg} )
	ToolsSummary( ${msg} )
	MinilineSummary( ${msg} )
	PluginsSummary( ${msg} )
	message( STATUS ${msg} )

	if ( ( "${PROJECTS_PACKAGE_WIX}" STREQUAL "ON" ) OR ( "${PROJECTS_PACKAGE_DEB}" STREQUAL "ON" ) OR ( "${PROJECTS_PACKAGE_RPM}" STREQUAL "ON" ) )
		get_filename_component( PARENT_DIR ${CMAKE_SOURCE_DIR} PATH )
		file( MAKE_DIRECTORY "${PROJECTS_SETUP_OUTPUT_DIR}/${CMAKE_BUILD_TYPE}" )
		set( CPACK_PACKAGE_NAME "${MAIN_PROJECT_NAME}" )
		set( CPACK_PACKAGE_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_BUILD}" )
		set( CPACK_PACKAGE_DESCRIPTION_SUMMARY "${MAIN_PROJECT_NAME}" )
		set( CPACK_PACKAGE_DESCRIPTION "Allows ${MAIN_PROJECT_NAME} developments" )
		set( CPACK_PACKAGE_RELEASE 1 )
		set( CPACK_PACKAGE_CONTACT "dragonjoker59@hotmail.com" )
		set( CPACK_PACKAGE_VENDOR "DragonJoker" )
		set( CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_PACKAGE_RELEASE}.${PROJECTS_PLATFORM}" )
		set( CPACK_OUTPUT_FILE_PREFIX "${PROJECTS_SETUP_OUTPUT_DIR}/${CMAKE_BUILD_TYPE}" )
		set( CPACK_RESOURCE_FILE_LICENSE "${PARENT_DIR}/LICENSE" )

		if ( "${PROJECTS_PACKAGE_WIX}" STREQUAL "ON" )
			set( CPACK_GENERATOR "WIX" )
			set( CPACK_PACKAGE_INSTALL_DIRECTORY "${MAIN_PROJECT_NAME}" )
			set( CPACK_WIX_UPGRADE_GUID "E3FDF993-ADFA-41C5-8608-E5E51A65C627" )
			set( CPACK_WIX_PRODUCT_GUID "D3B9CB1E-733B-4148-B21A-B1308D213D6E" )
			set( CPACK_WIX_PRODUCT_ICON "${PARENT_DIR}/install/${MAIN_PROJECT_NAME}.ico" )
			set( CPACK_WIX_UI_DIALOG "${PARENT_DIR}/install/dialog.bmp" )
			set( CPACK_WIX_UI_BANNER "${PARENT_DIR}/install/banner.bmp" )
			set( CPACK_WIX_PROGRAM_MENU_FOLDER "${MAIN_PROJECT_NAME}" )
			set( CPACK_WIX_CULTURES "fr-FR;en-EN" )
			include(CPack)
		elseif ( "${PROJECTS_PACKAGE_DEB}" STREQUAL "ON" )
			set( CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX} )
			set( CPACK_DEB_COMPONENT_INSTALL ON )
			set( CPACK_GENERATOR "DEB" )
			set( CPACK_DEBIAN_PACKAGE_MAINTAINER "Sylvain Doremus" )
			set( CPACK_DEBIAN_PACKAGE_DEPENDS "${CPACK_DEBIAN_PACKAGE_DEPENDS}libogre (>= 2.4.2)" )
			set( CPACK_DEBIAN_PACKAGE_ARCHITECTURE "${CMAKE_SYSTEM_ARCHITECTURE}" )
			set( CPACK_DEBIAN_PACKAGE_SECTION "devel" )
			set( CPACK_DEBIAN_PACKAGE_PRIORITY "optional" )
			include(CPack)
		elseif ( "${PROJECTS_PACKAGE_RPM}" STREQUAL "ON" )
			set( ${PROJECT_PACKAGE_DIR} ${CMAKE_BINARY_DIR}/_CPack_Packages/Linux/RPM )
			set( CPACK_RPM_COMPONENT_INSTALL ON )
			set( CPACK_RPM_PACKAGE_SUMMARY "${MAIN_PROJECT_NAME}" )
			set( CPACK_RPM_PACKAGE_ARCHITECTURE "${CMAKE_SYSTEM_ARCHITECTURE}" )
			set( CPACK_RPM_PACKAGE_GROUP "Development/Tools/Other" )
			set( CPACK_RPM_PACKAGE_LICENSE "LGPL" )
			set( CPACK_RPM_SPEC_MORE_DEFINE "%define _rpmdir ${PROJECT_PACKAGE_DIR}" )
			set( CPACK_RPM_SPEC_MORE_DEFINE "%define _rpmfilename ${CPACK_PACKAGE_FILE_NAME}.rpm" )
			set( CPACK_RPM_SPEC_MORE_DEFINE "%define _unpackaged_files_terminate_build 0" )
			set( CPACK_RPM_SPEC_MORE_DEFINE "%define _topdir ${PROJECT_PACKAGE_DIR}" )
			include(CPack)
		endif()
	endif ()

	cpack_add_install_type( Minimal )
	cpack_add_install_type( Developer )
	cpack_add_install_type( Full )
	
	CoreSetup()
	ToolsSetup()
	MinilineSetup()
	PluginsSetup()
else()
	message( SEND_ERROR "Please select a build type (Debug or Release)" )
endif()

