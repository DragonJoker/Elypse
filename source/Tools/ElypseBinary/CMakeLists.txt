project( ElypseBinary )

set( ${PROJECT_NAME}_VERSION_MAJOR	0 )
set( ${PROJECT_NAME}_VERSION_MINOR	9 )
set( ${PROJECT_NAME}_VERSION_BUILD	4 )

add_target(
	${PROJECT_NAME}
	bin
	${CMAKE_CURRENT_SOURCE_DIR}/Src
	${CMAKE_CURRENT_SOURCE_DIR}/Src
	""
	""
	"ElypseBinaryPch.h"
	"ElypseBinaryPch.cpp"
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

copy_target_directory( ${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/Data/cfg cfg )
