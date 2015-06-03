project( ElypseBinary )

option( BUILD_${PROJECT_NAME} "Build ${PROJECT_NAME}" TRUE )

set( ${PROJECT_NAME}_VERSION_MAJOR	0 )
set( ${PROJECT_NAME}_VERSION_MINOR	9 )
set( ${PROJECT_NAME}_VERSION_BUILD	4 )

include_directories( ${CMAKE_SOURCE_DIR}/Core/GeneralLib/Src )
include_directories( ${CMAKE_SOURCE_DIR}/Core/ElypseCore/Src )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/Src )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/Data )
include_directories( ${Ogre_INCLUDE_DIRS} )

add_target(
	${PROJECT_NAME}
	bin
	"ElypseCore"
	"ElypseCore;${ElypseCoreLibraries};${BinariesLibraries}"
)

set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Tools" )
set( Build "yes (version ${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_BUILD})" PARENT_SCOPE )
add_target_astyle( ${PROJECT_NAME} ".h;.hpp;.inl;.cpp" )

copy_target_directory( ${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/Data/cfg cfg )
