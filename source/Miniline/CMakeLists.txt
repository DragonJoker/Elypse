if (NOT MINGW)
	set( Boost_USE_STATIC_LIBS ON)
	set( BOOST_THREAD_USE_LIB ON)
endif()

if ( Boost_FOUND )
	include_directories( ElypseServer/Src )

	if ( WIN32 )
		add_definitions( -D_WINSOCK_DEPRECATED_NO_WARNINGS )
	endif ()

	add_subdirectory( ElypseServer )
	add_subdirectory( ChatServerPlugin )
	add_subdirectory( ElypseServerBin )
endif ()