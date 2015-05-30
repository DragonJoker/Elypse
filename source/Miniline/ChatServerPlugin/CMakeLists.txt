project( ChatServerPlugin )

if (NOT MINGW)
	set( Boost_USE_STATIC_LIBS ON)
	set( BOOST_THREAD_USE_LIB ON)
endif()

set( ${PROJECT_NAME}_VERSION_MAJOR	1 )
set( ${PROJECT_NAME}_VERSION_MINOR	0 )
set( ${PROJECT_NAME}_VERSION_BUILD	0 )

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
		api_dll
		"ElypseServer"
		"${BoostLibraries};${DatabaseLibraries};ElypseServer"
	)

	set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Miniline" )
	set( Build "yes (version ${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_BUILD})" PARENT_SCOPE )
	add_target_astyle( ${PROJECT_NAME} ".h;.hpp;.inl;.cpp" )

	install_dll( ${PROJECT_NAME} ${Database_LIBRARY_DEBUG} Debug )
	install_dll( ${PROJECT_NAME} ${Database_LIBRARY_RELEASE} Release )

	copy_dll( ${PROJECT_NAME} ${Database_LIBRARY_DEBUG} Debug )
	copy_dll( ${PROJECT_NAME} ${Database_LIBRARY_RELEASE} Release )
else ()
	set( Build "No (missing DatabaseEngine library" )
endif ()