project( TrollEditor )

option( BUILD_${PROJECT_NAME} "Build ${PROJECT_NAME}" TRUE )

set( ${PROJECT_NAME}_VERSION_MAJOR	0 )
set( ${PROJECT_NAME}_VERSION_MINOR	9 )
set( ${PROJECT_NAME}_VERSION_BUILD	4 )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/Src )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/Data )

add_target(
	${PROJECT_NAME}
	bin
	"GeneralLib;ElypseCore"
	"GeneralLib;ElypseCore;${ElypseCoreLibraries};${BinariesLibraries}"
	"PrecompiledHeader.h"
	"PrecompiledHeader.cpp"
)

set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Tools" )
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

