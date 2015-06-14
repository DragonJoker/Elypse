project( ElypseBinary )

add_definitions( -D_CRT_SECURE_NO_WARNINGS )

set( ${PROJECT_NAME}_VERSION_MAJOR	0 )
set( ${PROJECT_NAME}_VERSION_MINOR	9 )
set( ${PROJECT_NAME}_VERSION_BUILD	4 )

include_directories(
	${CMAKE_SOURCE_DIR}/Core/ElypseCore/Src
	${CMAKE_SOURCE_DIR}/Tools/GuiCommon/Src
	${CMAKE_CURRENT_SOURCE_DIR}/Src
)

add_target(
	${PROJECT_NAME}
	bin
	"GeneralLib;ElypseCore;GuiCommon"
	"GeneralLib;ElypseCore;GuiCommon;${ElypseCoreLibraries};${BinariesLibraries}"
	"ElypseBinaryPch.h"
	"ElypseBinaryPch.cpp"
)

set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Tools" )
set( Build "yes (version ${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_BUILD})" PARENT_SCOPE )
add_target_astyle( ${PROJECT_NAME} ".h;.hpp;.inl;.cpp" )

copy_target_directory( ${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/Data/cfg cfg )
