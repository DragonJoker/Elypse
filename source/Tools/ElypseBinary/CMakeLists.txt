project( ElypseBinary )

set( ${PROJECT_NAME}_VERSION_MAJOR	0 )
set( ${PROJECT_NAME}_VERSION_MINOR	9 )
set( ${PROJECT_NAME}_VERSION_BUILD	4 )

add_target(
	${PROJECT_NAME}
	bin
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	""
	""
	${CMAKE_CURRENT_SOURCE_DIR}/ElypseBinaryPch.h
	${CMAKE_CURRENT_SOURCE_DIR}/ElypseBinaryPch.cpp
)
target_include_directories( ${PROJECT_NAME}
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}
		${CMAKE_CURRENT_SOURCE_DIR}/Win32
)
target_link_libraries( ${PROJECT_NAME}
	PRIVATE
		ElypseGuiCommon
)
set_target_properties(${PROJECT_NAME}
	PROPERTIES
		VS_DEBUGGER_WORKING_DIRECTORY "$(OutDir)"
		FOLDER "Tools"
)

add_target_astyle( ${PROJECT_NAME} ".h;.hpp;.inl;.cpp" )

set( Build "yes (version ${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_BUILD})" PARENT_SCOPE )
set( OPTIONAL_PROJECTS
	${OPTIONAL_PROJECTS}
	${PROJECT_NAME}
	PARENT_SCOPE
)
