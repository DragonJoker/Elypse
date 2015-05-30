if (NOT MINGW)
	set( Boost_USE_STATIC_LIBS ON)
	set( BOOST_THREAD_USE_LIB ON)
endif()

if( NOT WIN32 )
	find_package( X11 )
endif()

if ( Boost_FOUND )
	include_directories( ElypseServer/Src )

	if ( WIN32 )
		add_definitions( -D_WINSOCK_DEPRECATED_NO_WARNINGS )
	endif ()
	
	#X11 Libs
	set( x11Libraries "" )
	foreach( Lib ${X11_LIBRARIES} )
		if( x11Libraries )
			set( x11Libraries "${x11Libraries}|${Lib}" )
		else()
			set( x11Libraries "${Lib}" )
		endif()
	endforeach()

	add_subdirectory( ElypseServer )
	add_subdirectory( ChatServerPlugin )
	add_subdirectory( ElypseServerBin )
endif ()
