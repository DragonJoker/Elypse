project( TrollEditor )

option( BUILD_${PROJECT_NAME} "Build ${PROJECT_NAME}" TRUE )

set( ${PROJECT_NAME}_VERSION_MAJOR	0 )
set( ${PROJECT_NAME}_VERSION_MINOR	9 )
set( ${PROJECT_NAME}_VERSION_BUILD	4 )

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
		${PROJECT_NAME}
		bin
		"GeneralLib;ElypseCore"
		"GeneralLib;ElypseCore;${wxWidgetsLibraries};${BoostLibraries};${ZZipLibraries};${OgreLibraries};${OdeLibraries};${GstLibraries};${GTK2_LIBRARIES};${XmlLibraries};${GtkmmLibraries}"
		"PrecompiledHeader.h"
		"PrecompiledHeader.cpp"
		"${wxWidgets_CFLAGS}"
		"${wxWidgets_CXXFLAGS}"
	)

	set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Binaries" )
	set( Build "yes (version ${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_BUILD})" PARENT_SCOPE )
	add_target_astyle( ${PROJECT_NAME} ".h;.hpp;.inl;.cpp" )

	file( GLOB_RECURSE Parser_languages ${CMAKE_CURRENT_SOURCE_DIR}/Data/*.lang )
	copy_target_files( ${PROJECT_NAME} "" ${Parser_languages} )
	copy_target_directory( ${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/Data/OverlayEditor OverlayEditor )
	copy_target_directory( ${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/Data/SceneEditor SceneEditor )

	parse_subdir_files( Src/Data "Data" )
	parse_subdir_files( Src/GUI "GUI" )
	parse_subdir_files( Src/GUI/ObjectProperties "GUI\\\\ObjectProperties" )
	parse_subdir_files( Src/GUI/TimeSequence "GUI\\\\TimeSequence" )
	parse_subdir_files( Src/Main "Main" )
	parse_subdir_files( Src/Project "Project" )
	parse_subdir_files( Src/Project/2D "Project\\\\2D" )
	parse_subdir_files( Src/Project/3D "Project\\\\3D" )
	parse_subdir_files( Src/Project/Media "Project\\\\Media" )
	parse_subdir_files( Src/Project/Temporal "Project\\\\Temporal" )
	parse_subdir_files( Src/SceneFile "SceneFile" )
	parse_subdir_files( Src/Script "Script" )
else()
	if ( NOT wxWidgets_FOUND )
		set( Build "No (missing wxWidgets library" )
	endif()
	if ( NOT GTK_FOUND )
		set( Build "No (missing GTK library" )
	endif()
endif()

