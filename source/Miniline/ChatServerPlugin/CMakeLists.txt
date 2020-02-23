project( ChatServerPlugin )

set( ${PROJECT_NAME}_VERSION_MAJOR	1 )
set( ${PROJECT_NAME}_VERSION_MINOR	0 )
set( ${PROJECT_NAME}_VERSION_BUILD	0 )

find_package( Boost 1.55 REQUIRED locale system )
find_package( MySQL REQUIRED client )
find_package( Database )

if ( DATABASE_FOUND )
	add_target(
		${PROJECT_NAME}
		dll
		${CMAKE_CURRENT_SOURCE_DIR}
		${CMAKE_CURRENT_SOURCE_DIR}
		""
		""
	)
	target_include_directories( ${PROJECT_NAME}
		PUBLIC
			${Database_INCLUDE_DIR}
	)
	target_link_libraries( ${PROJECT_NAME}
		PUBLIC
			GeneralLib
			ElypseServer
			${Database_LIBRARIES}
	)

	set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Miniline" )
	set( Build "yes (version ${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_BUILD})" PARENT_SCOPE )
	add_target_astyle( ${PROJECT_NAME} ".h;.hpp;.inl;.cpp" )

	copy_dll( ${PROJECT_NAME} ${Database_LIBRARY_DEBUG} Debug )
	copy_dll( ${PROJECT_NAME} ${Database_LIBRARY_RELEASE} Release )
	copy_dll( ${PROJECT_NAME} ${MySQL_client_LIBRARY_DEBUG} Debug )
	copy_dll( ${PROJECT_NAME} ${MySQL_client_LIBRARY_RELEASE} Release )
	copy_dll( ${PROJECT_NAME} ${Boost_LOCALE_LIBRARY_DEBUG} Debug )
	copy_dll( ${PROJECT_NAME} ${Boost_LOCALE_LIBRARY_RELEASE} Release )
	copy_dll( ${PROJECT_NAME} ${Boost_SYSTEM_LIBRARY_DEBUG} Debug )
	copy_dll( ${PROJECT_NAME} ${Boost_SYSTEM_LIBRARY_RELEASE} Release )
	
	if ( WIN32 )
		copy_dll( ${PROJECT_NAME} ${Database_LIBRARY_DIR}/../lib/DatabaseConnector/DatabasePluginMySqld.lib Debug )
		copy_dll( ${PROJECT_NAME} ${Database_LIBRARY_DIR}/../lib/DatabaseConnector/DatabasePluginMySql.lib  Release )
	endif ()
else ()
	set( Build "No (missing DatabaseEngine library" )
endif ()
