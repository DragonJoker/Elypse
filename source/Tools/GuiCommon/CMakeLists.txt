project( GuiCommon )

set( ${PROJECT_NAME}_VERSION_MAJOR	1 )
set( ${PROJECT_NAME}_VERSION_MINOR	0 )
set( ${PROJECT_NAME}_VERSION_BUILD	0 )

set( CMAKE_CURRENT_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include/Tools/${PROJECT_NAME} )

add_target(
	${PROJECT_NAME}
	lib
	${CMAKE_CURRENT_INCLUDE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	""
	""
	"${CMAKE_CURRENT_INCLUDE_DIR}/GuiCommonPch.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/GuiCommonPch.cpp"
)
target_compile_definitions( ${PROJECT_NAME}
	PUBLIC
		${wxWidgets_DEFINITIONS}
)
target_include_directories( ${PROJECT_NAME}
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/Win32
	PUBLIC
		${CMAKE_CURRENT_INCLUDE_DIR}
		${wxWidgets_INCLUDE_DIR}
)
target_link_libraries( ${PROJECT_NAME}
	PUBLIC
		ElypseCore
		${wxWidgets_LIBRARIES}
		${OpenCV_LIBS}
)

set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Tools" )
set( Build "yes (version ${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_BUILD})" PARENT_SCOPE )
add_target_astyle( ${PROJECT_NAME} ".h;.hpp;.inl;.cpp" )
