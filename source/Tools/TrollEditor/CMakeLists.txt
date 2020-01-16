project( TrollEditor )

set( ${PROJECT_NAME}_VERSION_MAJOR	${VERSION_MAJOR} )
set( ${PROJECT_NAME}_VERSION_MINOR	${VERSION_MINOR} )
set( ${PROJECT_NAME}_VERSION_BUILD	${VERSION_BUILD} )

include_directories(
)

add_target(
	${PROJECT_NAME}
	bin
	""
	""
	"TrollEditorPch.h"
	"TrollEditorPch.cpp"
)
target_include_directories( ${PROJECT_NAME}
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/Src
		${Fmod_INCLUDE_DIR}
)
target_link_libraries( ${PROJECT_NAME}
	PRIVATE
		GuiCommon
)
set_target_properties(${PROJECT_NAME}
	PROPERTIES
		VS_DEBUGGER_WORKING_DIRECTORY "$(OutDir)"
		FOLDER "Tools"
)

set( Build "yes (version ${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_BUILD})" PARENT_SCOPE )
add_target_astyle( ${PROJECT_NAME} ".h;.hpp;.inl;.cpp" )

file( GLOB_RECURSE Parser_languages ${CMAKE_CURRENT_SOURCE_DIR}/Data/*.lang )
copy_target_files( ${PROJECT_NAME} "" ${Parser_languages} )
copy_target_directory( ${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/Data/OverlayEditor OverlayEditor )
copy_target_directory( ${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/Data/SceneEditor SceneEditor )

parse_subdir_files( Src/Elypse "Elypse" )
parse_subdir_files( Src/GUI "GUI" )
parse_subdir_files( Src/GUI/Aui "GUI\\\\Aui" )
parse_subdir_files( Src/GUI/Properties "GUI\\\\Properties" )
parse_subdir_files( Src/GUI/Properties/Common "GUI\\\\Properties\\\\Common" )
parse_subdir_files( Src/GUI/Properties/Project "GUI\\\\Properties\\\\Project" )
parse_subdir_files( Src/GUI/TimeSequence "GUI\\\\TimeSequence" )
parse_subdir_files( Src/Project "Project" )
parse_subdir_files( Src/Project/2D "Project\\\\2D" )
parse_subdir_files( Src/Project/3D "Project\\\\3D" )
parse_subdir_files( Src/Project/Media "Project\\\\Media" )
parse_subdir_files( Src/Project/Temporal "Project\\\\Temporal" )
