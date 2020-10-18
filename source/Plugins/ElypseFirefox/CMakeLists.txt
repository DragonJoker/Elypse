project( ElypseFirefox )

set( ${PROJECT_NAME}_VERSION_MAJOR	${VERSION_MAJOR} )
set( ${PROJECT_NAME}_VERSION_MINOR	${VERSION_MINOR} )
set( ${PROJECT_NAME}_VERSION_BUILD	${VERSION_BUILD} )

if ( WIN32 )
	find_package( GDI QUIET )
	find_package( GDIPlus QUIET )
else ()
	set( GDI_FOUND ON )
	set( GDIPLUS_FOUND ON )
endif ()

if ( GDI_FOUND AND GDIPLUS_FOUND )
	add_target(
		${PROJECT_NAME}
		dll
		${CMAKE_CURRENT_SOURCE_DIR}
		${CMAKE_CURRENT_SOURCE_DIR}
		""
		""
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
			${GDI_LIBRARY}
			${GDIPLUS_LIBRARY}
	)

	set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Plugins" )
	add_target_astyle( ${PROJECT_NAME} ".h;.hpp;.inl;.cpp" )

	parse_subdir_files( Elypse "Elypse" )
	parse_subdir_files( Firefox "Firefox" )
	parse_subdir_files( NpApi "NpApi" )

	set( Build "yes (version ${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_BUILD})" PARENT_SCOPE )
	set( OPTIONAL_PROJECTS
		${OPTIONAL_PROJECTS}
		${PROJECT_NAME}
		PARENT_SCOPE
	)
endif ()