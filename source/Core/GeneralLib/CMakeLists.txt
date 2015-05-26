project( GeneralLib )

set( GL_VERSION_MAJOR	0 )
set( GL_VERSION_MINOR	9 )
set( GL_VERSION_BUILD	4 )

add_target(
	GeneralLib
	lib
	""
	"${BoostLibraries};${ZZipLibraries}"
)

set_property( TARGET GeneralLib PROPERTY FOLDER "Core" )
set( Build "yes (version ${GL_VERSION_MAJOR}.${GL_VERSION_MINOR}.${GL_VERSION_BUILD})" PARENT_SCOPE )
add_target_astyle( GeneralLib ".h;.hpp;.inl;.cpp" )

install(
	TARGETS	GeneralLib
	ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
)

file( GLOB_RECURSE GeneralLibHeaders
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*.inl
)

install(
	FILES ${GeneralLibHeaders}
	DESTINATION ${CMAKE_INSTALL_PREFIX}/include/GeneralLib
)
