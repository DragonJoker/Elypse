project( ChatServerPlugin )

set( ${PROJECT_NAME}_VERSION_MAJOR	1 )
set( ${PROJECT_NAME}_VERSION_MINOR	0 )
set( ${PROJECT_NAME}_VERSION_BUILD	0 )

find_package( Boost 1.55 REQUIRED locale )
find_package( MySQL )
find_package( Database )

if ( DATABASE_FOUND )
	#DatabaseEngine Libs
	set( DatabaseLibraries "" )
	foreach( Lib ${Database_LIBRARIES} )
		if( DatabaseLibraries )
			set( DatabaseLibraries "${DatabaseLibraries}|${Lib}" )
		else()
			set( DatabaseLibraries "${Lib}" )
		endif()
	endforeach()

	include_directories(
		${Database_INCLUDE_DIR}
	)

	add_target(
		${PROJECT_NAME}
		dll
		"GeneralLib;ElypseServer"
		"GeneralLib;${BoostLibraries};${DatabaseLibraries};ElypseServer"
	)

	set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Miniline" )
	set( Build "yes (version ${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_BUILD})" PARENT_SCOPE )
	add_target_astyle( ${PROJECT_NAME} ".h;.hpp;.inl;.cpp" )

	copy_dll( ${PROJECT_NAME} ${Database_LIBRARY_DEBUG} Debug )
	copy_dll( ${PROJECT_NAME} ${Database_LIBRARY_RELEASE} Release )
	copy_dll( ${PROJECT_NAME} ${MYSQL_CLIENT_LIBRARY} Debug )
	copy_dll( ${PROJECT_NAME} ${MYSQL_CLIENT_LIBRARY} Release )
	copy_dll( ${PROJECT_NAME} ${MYSQL_CPP_LIBRARY_DYNAMIC_DEBUG} Debug )
	copy_dll( ${PROJECT_NAME} ${MYSQL_CPP_LIBRARY_DYNAMIC_RELEASE} Release )
	copy_dll( ${PROJECT_NAME} ${Boost_LOCALE_LIBRARY_DEBUG} Debug )
	copy_dll( ${PROJECT_NAME} ${Boost_LOCALE_LIBRARY_RELEASE} Release )
	copy_dll( ${PROJECT_NAME} ${Database_LIBRARY_DIR}/DatabasePluginMySqld.lib Debug )
	copy_dll( ${PROJECT_NAME} ${Database_LIBRARY_DIR}/DatabasePluginMySql.lib  Release )
else ()
	set( Build "No (missing DatabaseEngine library" )
endif ()