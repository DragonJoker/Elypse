project( ElypseOcx )

include( ComUtils )

set( ${PROJECT_NAME}_VERSION_MAJOR	${VERSION_MAJOR} )
set( ${PROJECT_NAME}_VERSION_MINOR	${VERSION_MINOR} )
set( ${PROJECT_NAME}_VERSION_BUILD	${VERSION_BUILD} )

find_package( ATL QUIET )
find_package( GDI QUIET )
find_package( GDIPlus QUIET )

if ( ATL_FOUND AND GDI_FOUND AND GDIPLUS_FOUND )
	# Define IDL files
	file( GLOB ${PROJECT_NAME}_IDL_FILES *.idl )
	file( GLOB ${PROJECT_NAME}_DEF_FILES *.def )

	# MIDL compiler
	add_com_interfaces( ${PROJECT_NAME}_SOURCES_IDL_FILES ${${PROJECT_NAME}_IDL_FILES} )
	set( ${PROJECT_NAME}_ADDITIONAL_FILES
		${${PROJECT_NAME}_IDL_FILES}
		${${PROJECT_NAME}_DEF_FILES}
		${${PROJECT_NAME}_SOURCES_IDL_FILES}
	)
	source_group( "IDL Files" FILES ${${PROJECT_NAME}_IDL_FILES} )
	source_group( "IDL Files" FILES ${${PROJECT_NAME}_SOURCES_IDL_FILES} )
	source_group( "IDL Files" FILES ${${PROJECT_NAME}_DEF_FILES} )

	add_target(
		${PROJECT_NAME}
		dll
		${CMAKE_CURRENT_SOURCE_DIR}
		${CMAKE_CURRENT_SOURCE_DIR}
		""
		""
		"ElypseOcxPch.h"
		"ElypseOcxPch.cpp"
		""
		""
		""
		"${${PROJECT_NAME}_ADDITIONAL_FILES}"
	)
	add_library( elypse::${PROJECT_NAME}
		ALIAS
		${PROJECT_NAME}
	)
	target_include_directories( ${PROJECT_NAME}
		PRIVATE
			${CMAKE_CURRENT_SOURCE_DIR}
	)
	target_link_libraries( ${PROJECT_NAME}
		PRIVATE
			elypse::Core
			${ATL_LIBRARIES}
			${GDI_LIBRARY}
			${GDIPLUS_LIBRARY}
	)

	# Copy tlb files into binaries directory.
	add_custom_command( TARGET ${PROJECT_NAME}
		POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.tlb ${PROJECTS_BINARIES_OUTPUT_DIR}/$<CONFIGURATION>/bin/
	)
	
	# install tlb files
	install(
		FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.tlb
		DESTINATION bin
		COMPONENT ${PROJECT_NAME}
	)

	set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Plugins" )
	add_target_astyle( ${PROJECT_NAME} ".h;.hpp;.inl;.cpp" )

	set( Build "yes (version ${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_BUILD})" PARENT_SCOPE )
	set( OPTIONAL_PROJECTS
		${OPTIONAL_PROJECTS}
		${PROJECT_NAME}
		PARENT_SCOPE
	)
else ()
	set( Build "no (missing ATL libraries)" )
endif ()