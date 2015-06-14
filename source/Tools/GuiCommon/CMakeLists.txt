project( GuiCommon )

add_definitions( -D_CRT_SECURE_NO_WARNINGS )

set( ${PROJECT_NAME}_VERSION_MAJOR	1 )
set( ${PROJECT_NAME}_VERSION_MINOR	0 )
set( ${PROJECT_NAME}_VERSION_BUILD	0 )

include_directories(
	${CMAKE_SOURCE_DIR}/Core/ElypseCore/Src
	${CMAKE_CURRENT_SOURCE_DIR}/Src
)

add_target(
	${PROJECT_NAME}
	lib
	"GeneralLib;ElypseCore"
	"GeneralLib;ElypseCore;${ElypseCoreLibraries};${BinariesLibraries}"
	"GuiCommonPch.h"
	"GuiCommonPch.cpp"
)

set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Tools" )
set( Build "yes (version ${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_BUILD})" PARENT_SCOPE )
add_target_astyle( ${PROJECT_NAME} ".h;.hpp;.inl;.cpp" )
