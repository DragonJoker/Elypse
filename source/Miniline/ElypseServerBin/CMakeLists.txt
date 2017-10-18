project( ElypseServerBin )

if (NOT MINGW)
	set( Boost_USE_STATIC_LIBS ON)
	set( BOOST_THREAD_USE_LIB ON)
endif()

set( ${PROJECT_NAME}_VERSION_MAJOR	1 )
set( ${PROJECT_NAME}_VERSION_MINOR	0 )
set( ${PROJECT_NAME}_VERSION_BUILD	0 )

include_directories(
	${CMAKE_SOURCE_DIR}/Miniline/ElypseServer/Src
)

add_target(
	${PROJECT_NAME}
	bin_dos
	"GeneralLib;ElypseServer"
	"GeneralLib;ElypseServer;${CMAKE_DL_LIBS};${BoostLibraries};${X11Libraries}"
)

set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Miniline" )
set( Build "yes (version ${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_BUILD})" PARENT_SCOPE )
add_target_astyle( ${PROJECT_NAME} ".h;.hpp;.inl;.cpp" )

