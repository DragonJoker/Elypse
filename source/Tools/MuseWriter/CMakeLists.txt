project( MuseWriter )

set( ${PROJECT_NAME}_VERSION_MAJOR	1 )
set( ${PROJECT_NAME}_VERSION_MINOR	0 )
set( ${PROJECT_NAME}_VERSION_BUILD	0 )

add_target(
	${PROJECT_NAME}
	bin
	""
	""
	"stdafx.h"
	"stdafx.cpp"
)
target_include_directories( ${PROJECT_NAME}
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/Src
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
