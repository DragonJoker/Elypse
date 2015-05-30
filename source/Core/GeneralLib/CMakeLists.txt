project( GeneralLib )

set( ${PROJECT_NAME}_VERSION_MAJOR	0 )
set( ${PROJECT_NAME}_VERSION_MINOR	9 )
set( ${PROJECT_NAME}_VERSION_BUILD	4 )

add_target(
	${PROJECT_NAME}
	lib
	""
	"${BoostLibraries};${ZZipLibraries}"
)

set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Core" )
set( Build "yes (version ${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_BUILD})" PARENT_SCOPE )
add_target_astyle( ${PROJECT_NAME} ".h;.hpp;.inl;.cpp" )

install_dll( ${PROJECT_NAME} ${ZLIB_LIBRARY} Debug )
install_dll( ${PROJECT_NAME} ${ZLIB_LIBRARY} Release )
install_dll( ${PROJECT_NAME} ${LIBZIP_LIBRARY} Debug )
install_dll( ${PROJECT_NAME} ${LIBZIP_LIBRARY} Release )

copy_dll( ${PROJECT_NAME} ${ZLIB_LIBRARY} Debug )
copy_dll( ${PROJECT_NAME} ${ZLIB_LIBRARY} Release )
copy_dll( ${PROJECT_NAME} ${LIBZIP_LIBRARY} Debug )
copy_dll( ${PROJECT_NAME} ${LIBZIP_LIBRARY} Release )
