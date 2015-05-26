option( BUILD_TROLL_EDITOR "Build TrollEditor" TRUE )

project( TrollEditor )

set( TE_VERSION_MAJOR	0 )
set( TE_VERSION_MINOR	9 )
set( TE_VERSION_BUILD	4 )

set( wxWidgets_USE_STATIC OFF)
if (WIN32)
	find_package( wxWidgets REQUIRED stc scintilla core base aui)
else ()
	find_package( wxWidgets REQUIRED core base stc)
endif()

if ( NOT WIN32 )
	find_package( GTK )
else ()
	set( GTK_FOUND TRUE )
endif ( NOT WIN32 )

if ( wxWidgets_FOUND AND GTK_FOUND )
	string( COMPARE EQUAL "${wxWidgets_USE_DEBUG}" "ON" IsWxDebug)
	set( wxWidgets_CXXFLAGS "${wxWidgets_CXX_FLAGS} -D_FILE_OFFSET_BITS=64 -D_LARGE_FILES")
	if (NOT WIN32)
		set( wxWidgets_CXXFLAGS "${wxWidgets_CXXFLAGS} -D__WXGTK__")
	endif ()
	if (IsWxDebug)
		set( wxWidgets_CXXFLAGS "${wxWidgets_CXXFLAGS} -D__WXDEBUG__")
	endif ()

	include_directories( ${CMAKE_SOURCE_DIR}/Core/GeneralLib/Src )
	include_directories( ${CMAKE_SOURCE_DIR}/Core/ElypseCore/Src )
	include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/Src )
	include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/Data )
	include_directories( ${Ogre_INCLUDE_DIRS} )
	include_directories( ${wxWidgets_INCLUDE_DIRS} )

	if (NOT WIN32)
		include_directories( ${GTK_INCLUDE_DIR} )
	endif (NOT WIN32)
	
	link_directories( ${wxWidgets_LIBRARY_DIRS} )
	
	set( wxWidgetsLibraries "" )

	FOREACH( Lib ${wxWidgets_LIBRARIES} )
		if ( wxWidgetsLibraries )
			set( wxWidgetsLibraries "${wxWidgetsLibraries}|${Lib}" )
		else()
			set( wxWidgetsLibraries "${Lib}" )
		endif()
	ENDFOREACH()
	
	FOREACH( Lib ${GTK_LIBRARY} )
		if ( wxWidgetsLibraries )
			set( wxWidgetsLibraries "${wxWidgetsLibraries}|${Lib}" )
		else()
			set( wxWidgetsLibraries "${Lib}" )
		endif()
	ENDFOREACH()
	
	#Few configuration for wxWidgets
	string( COMPARE EQUAL "${wxWidgets_USE_DEBUG}" "ON" IsWxDebug )
	set( wxWidgets_CXXFLAGS "${wxWidgets_CXX_FLAGS} -D_FILE_OFFSET_BITS=64 -D_LARGE_FILES" )
	if ( NOT WIN32 )
		set( wxWidgets_CXXFLAGS "${wxWidgets_CXXFLAGS} -D__WXGTK__" )
	endif ()
	if ( IsWxDebug )
		set( wxWidgets_CXXFLAGS "${wxWidgets_CXXFLAGS} -D__WXDEBUG__" )
	endif ()
	
	add_target(
		TrollEditor
		bin
		"GeneralLib;ElypseCore"
		"GeneralLib;ElypseCore;${wxWidgetsLibraries};${BoostLibraries};${ZZipLibraries};${OgreLibraries};${OdeLibraries};${GstLibraries};${GTK2_LIBRARIES};${XmlLibraries};${GtkmmLibraries}"
		"PrecompiledHeader.h"
		"PrecompiledHeader.cpp"
		"${wxWidgets_CFLAGS}"
		"${wxWidgets_CXXFLAGS}"	""
	)
	
	set_property( TARGET TrollEditor PROPERTY FOLDER "Binaries" )
	set( Build "yes (version ${TE_VERSION_MAJOR}.${TE_VERSION_MINOR}.${TE_VERSION_BUILD})" PARENT_SCOPE )
	add_target_astyle( TrollEditor ".h;.hpp;.inl;.cpp" )

	file( GLOB_RECURSE Parser_languages 	${CMAKE_CURRENT_SOURCE_DIR}/Data/*.lang )
	file( GLOB_RECURSE OvEditor_files 		${CMAKE_CURRENT_SOURCE_DIR}/Data/OverlayEditor/*.* )
	file( GLOB_RECURSE ScEditor_files 		${CMAKE_CURRENT_SOURCE_DIR}/Data/SceneEditor/*.* )
	
	file( COPY ${Parser_languages} DESTINATION ${PROJECTS_BINARIES_OUTPUT_PATH}/Debug/bin )
	file( MAKE_DIRECTORY "${PROJECTS_BINARIES_OUTPUT_PATH}/Debug/bin/SceneEditor" )
	file( MAKE_DIRECTORY "${PROJECTS_BINARIES_OUTPUT_PATH}/Debug/bin/OverlayEditor" )
	file( COPY ${ScEditor_files} DESTINATION ${PROJECTS_BINARIES_OUTPUT_PATH}/Debug/bin/SceneEditor )
	file( COPY ${OvEditor_files} DESTINATION ${PROJECTS_BINARIES_OUTPUT_PATH}/Debug/bin/OverlayEditor )

	file( COPY ${Parser_languages} DESTINATION ${PROJECTS_BINARIES_OUTPUT_PATH}/Release/bin )
	file( MAKE_DIRECTORY "${PROJECTS_BINARIES_OUTPUT_PATH}/Release/bin/SceneEditor" )
	file( MAKE_DIRECTORY "${PROJECTS_BINARIES_OUTPUT_PATH}/Release/bin/OverlayEditor" )
	file( COPY ${ScEditor_files} DESTINATION ${PROJECTS_BINARIES_OUTPUT_PATH}/Release/bin/SceneEditor )
	file( COPY ${OvEditor_files} DESTINATION ${PROJECTS_BINARIES_OUTPUT_PATH}/Release/bin/OverlayEditor )

	install(
		TARGETS	TrollEditor
		RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
	)
else()
	if ( NOT wxWidgets_FOUND )
		set( Build "No (missing wxWidgets library" )
	endif()
	if ( NOT GTK_FOUND )
		set( Build "No (missing GTK library" )
	endif()
endif()

