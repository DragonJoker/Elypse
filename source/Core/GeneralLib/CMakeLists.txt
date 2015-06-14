project( GeneralLib )

set( ${PROJECT_NAME}_VERSION_MAJOR	${VERSION_MAJOR} )
set( ${PROJECT_NAME}_VERSION_MINOR	${VERSION_MINOR} )
set( ${PROJECT_NAME}_VERSION_BUILD	${VERSION_BUILD} )

include_directories( ${CMAKE_SOURCE_DIR}/Core/GeneralLib/Src )

add_target(
	${PROJECT_NAME}
	lib
	""
	"${BoostLibraries};${LibZipLibraries}"
)

set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Core" )
set( Build "yes (version ${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_BUILD})" PARENT_SCOPE )
add_target_astyle( ${PROJECT_NAME} ".h;.hpp;.inl;.cpp" )

copy_dll( ${PROJECT_NAME} ${ZLIB_LIBRARY} Debug )
copy_dll( ${PROJECT_NAME} ${ZLIB_LIBRARY} Release )
copy_dll( ${PROJECT_NAME} ${LIBZIP_LIBRARY} Debug )
copy_dll( ${PROJECT_NAME} ${LIBZIP_LIBRARY} Release )
if ( WIN32 )
	get_filename_component( _Path ${FREETYPE_LIBRARY} PATH )
	get_filename_component( _Path ${_Path} PATH )
	copy_dll( ${PROJECT_NAME} ${_Path}/bin/freetype6.dll Debug )
	copy_dll( ${PROJECT_NAME} ${_Path}/bin/freetype6.dll Release )
else ()
	copy_dll( ${PROJECT_NAME} ${FREETYPE_LIBRARY} Debug )
	copy_dll( ${PROJECT_NAME} ${FREETYPE_LIBRARY} Release )
endif ()

set( Build "yes (version ${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_BUILD})" PARENT_SCOPE )
