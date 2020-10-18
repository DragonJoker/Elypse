project( ElypseGuiCommon )

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
	${CMAKE_CURRENT_INCLUDE_DIR}/GuiCommonPch.h
	${CMAKE_CURRENT_SOURCE_DIR}/GuiCommonPch.cpp
)
add_library( elypse::GuiCommon
	ALIAS
	${PROJECT_NAME}
)
target_compile_definitions( ${PROJECT_NAME}
	PUBLIC
		${wxWidgets_DEFINITIONS}
)
target_include_directories( ${PROJECT_NAME}
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/Win32
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_INCLUDE_DIR}>
		$<INSTALL_INTERFACE:include>
		${wxWidgets_INCLUDE_DIR}
)
target_link_libraries( ${PROJECT_NAME}
	PUBLIC
		elypse::Core
		${wxWidgets_LIBRARIES}
		${OpenCV_LIBS}
)

target_install_headers( ${PROJECT_NAME} ${CMAKE_CURRENT_INCLUDE_DIR} )
set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Tools" )
add_target_astyle( ${PROJECT_NAME} ".h;.hpp;.inl;.cpp" )

set( Build "yes (version ${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_BUILD})" PARENT_SCOPE )
set( OPTIONAL_PROJECTS
	${OPTIONAL_PROJECTS}
	${PROJECT_NAME}
	PARENT_SCOPE
)
