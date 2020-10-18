project( ElypseServerBin )

if (NOT MINGW)
	set( Boost_USE_STATIC_LIBS ON)
	set( BOOST_THREAD_USE_LIB ON)
endif()

set( ${PROJECT_NAME}_VERSION_MAJOR	1 )
set( ${PROJECT_NAME}_VERSION_MINOR	0 )
set( ${PROJECT_NAME}_VERSION_BUILD	0 )

add_target(
	${PROJECT_NAME}
	bin_dos
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	""
	""
)
target_link_libraries( ${PROJECT_NAME}
	PRIVATE
		elypse::Server
		${CMAKE_DL_LIBS}
)

set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Miniline" )
add_target_astyle( ${PROJECT_NAME} ".h;.hpp;.inl;.cpp" )

set( Build "yes (version ${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_BUILD})" PARENT_SCOPE )
set( OPTIONAL_PROJECTS
	${OPTIONAL_PROJECTS}
	${PROJECT_NAME}
	PARENT_SCOPE
)
