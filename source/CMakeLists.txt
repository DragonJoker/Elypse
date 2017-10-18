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
	${CMAKE_SOURCE_DIR}/CMake
	${CMAKE_SOURCE_DIR}/CMake/Modules
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
	if (WIN32)
		set( Boost_USE_STATIC_LIBS ON)
		set( BOOST_THREAD_USE_LIB ON)
	endif()

	find_package( Boost 1.55 REQUIRED date_time filesystem system )
	find_package( CURL )
	if ( WIN32 )
		find_package( Ogre COMPONENTS Main Overlay )
	else ()
		find_package( Ogre COMPONENTS Main Overlay Plugin_ParticleFX RenderSystem_GL )
	endif ()
	find_package( ODE )
	find_package( Fmod )
	find_package( ZLIB 1.2.7 )
	find_package( LibZip )
	find_package( Freetype )

	if ( NOT WIN32 )
		find_package( GTK2 )
		find_package( X11 )
	else ()
		set( GTK2_FOUND TRUE )
	endif ( NOT WIN32 )

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
		message( STATUS "+ Found Ogre (version ${Ogre_VERSION})" )
		include_directories( ${Ogre_INCLUDE_DIR} )
		include_directories( ${Ogre_INCLUDE_DIRS} )
	endif ()
	if ( ODE_FOUND )
		message( STATUS "+ Found ODE" )
		include_directories( ${ODE_INCLUDE_DIR} )
	endif ()
	if ( FMOD_FOUND )
		message( STATUS "+ Found FMOD" )
		include_directories( ${Fmod_INCLUDE_DIR} )
	endif ()
	if ( GTK2_FOUND )
		message( STATUS "+ Found GTK2" )
		include_directories( ${GTK2_INCLUDE_DIRS} )
	endif ()

	#--------------------------------------------------------------------------------------------------
	#	Adding libraries dirs to link search path
	#--------------------------------------------------------------------------------------------------
	link_directories(
		${LIBRARY_OUTPUT_PATH}
		${Boost_LIBRARY_DIRS}
		${ZZip_LIBRARY_DIR}
		#${Ogre_LIBRARY_DIR}
		${ODE_LIBRARY_DIR}
		${Fmod_LIBRARY_DIR}
	)
	if ( NOT WIN32 )
		link_directories(
			${GSTREAMER_LIBRARY_DIR}
			${LIBXML_LIBRARY_DIR}
		)
	endif ()

	#--------------------------------------------------------------------------------------------------
	#	Normalising libraries names
	#--------------------------------------------------------------------------------------------------
	# Curl libs
	set( CurlLibraries "" )
	FOREACH( Lib ${CURL_LIBRARIES} )
		if ( CurlLibraries )
			set( CurlLibraries "${CurlLibraries}|${Lib}" )
		else()
			set( CurlLibraries "${Lib}" )
		endif()
	ENDFOREACH()

	# Boost libs
	set( BoostLibraries "")
	FOREACH(Lib ${Boost_LIBRARIES})
		if (BoostLibraries)
			set(BoostLibraries "${BoostLibraries}|${Lib}")
		else()
			set(BoostLibraries "${Lib}")
		endif()
	ENDFOREACH()
	
	if ( NOT WIN32 )
		set(BoostLibraries "${BoostLibraries}|pthread")
	endif ()

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

	#Gtk2 Libs
	set( Gtk2Libraries "" )
	FOREACH( Lib ${GTK2_LIBRARIES} )
		if ( Gtk2Libraries )
			set( Gtk2Libraries "${Gtk2Libraries}|${Lib}" )
		else()
			set( Gtk2Libraries "${Lib}" )
		endif()
	ENDFOREACH()

	#Ogre Libs
	set( OgreLibraries "")
	FOREACH(Lib ${Ogre_LIBRARIES})
		if (OgreLibraries)
			set(OgreLibraries "${OgreLibraries}|${Lib}")
		else()
			set(OgreLibraries "${Lib}")
		endif()
	ENDFOREACH()

	#Ode Libs
	set( OdeLibraries "")
	FOREACH(Lib ${ODE_LIBRARIES})
		if (OdeLibraries)
			set(OdeLibraries "${OdeLibraries}|${Lib}")
		else()
			set(OdeLibraries "${Lib}")
		endif()
	ENDFOREACH()

	#Fmod Libs
	set( FmodLibraries "" )
	FOREACH(Lib ${Fmod_LIBRARIES} )
		if ( FmodLibraries )
			set( FmodLibraries "${FmodLibraries}|${Lib}" )
		else()
			set( FmodLibraries "${Lib}" )
		endif()
	ENDFOREACH()
	
	#X11 Libs
	set( X11Libraries "" )
	foreach( Lib ${X11_LIBRARIES} )
		if( X11Libraries )
			set( X11Libraries "${X11Libraries}|${Lib}" )
		else()
			set( X11Libraries "${Lib}" )
		endif()
	endforeach()

	set( GeneralLibLibraries
		${BoostLibraries}
		${ZlibLibraries}
		${LibZipLibraries}
		${CurlLibraries}
		${CMAKE_DL_LIBS}
	)

	set( ElypseCoreLibraries
		${GeneralLibLibraries}
		${FmodLibraries}
		${OgreLibraries}
		${OdeLibraries}
		${GstLibraries}
		${DShowLibraries}
		${XmlLibraries}
		${Gtk2Libraries}
		${X11Libraries}
	)

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

	if( Boost_FOUND AND OGRE_FOUND AND ODE_FOUND AND ZLIB_FOUND AND LIBZIP_FOUND AND CURL_FOUND AND FMOD_FOUND )
		include_directories( ${CMAKE_SOURCE_DIR}/Core/GeneralLib/Src )

		add_subdirectory( Core )
		#add_subdirectory( Plugins )
		add_subdirectory( Tools )
		add_subdirectory( Miniline )

		CoreInit()
		ToolsInit()
		MinilineInit()

		CoreBuild()
		ToolsBuild()
		MinilineBuild()

		set( msg "\nElypse following projects will be built" )
		CoreSummary( ${msg} )
		ToolsSummary( ${msg} )
		MinilineSummary( ${msg} )
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
			set( CPACK_RESOURCE_FILE_LICENSE "${PARENT_DIR}/LGPL.txt" )

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
	else()
		set( msg "Elypse projects can't be built because of following missing libraries :" )
		if ( NOT Boost_FOUND )
			set( msg "${msg}\n\tBoost library" )
		endif ()
		if ( NOT OGRE_FOUND )
			set( msg "${msg}\n\tOgre library" )
		endif ()
		if ( NOT CURL_FOUND )
			set( msg "${msg}\n\tCurl library" )
		endif ()
		if ( NOT ODE_FOUND )
			set( msg "${msg}\n\tODE library" )
		endif ()
		if ( NOT FMOD_FOUND )
			set( msg "${msg}\n\tFMOD library" )
		endif ()
		if ( NOT ZLIB_FOUND )
			set( msg "${msg}\n\tZLib library" )
		endif ()
		if ( NOT LIBZIP_FOUND )
			set( msg "${msg}\n\tlibzip library" )
		endif ()
		message( SEND_ERROR ${msg} )
	endif()
else()
	message( SEND_ERROR "Please select a build type (Debug or Release)" )
endif()

