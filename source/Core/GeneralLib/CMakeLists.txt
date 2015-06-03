project( GeneralLib )

set( ${PROJECT_NAME}_VERSION_MAJOR	0 )
set( ${PROJECT_NAME}_VERSION_MINOR	9 )
set( ${PROJECT_NAME}_VERSION_BUILD	4 )

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

find_package( Freetype )

get_filename_component( FreeTypePath ${FREETYPE_LIBRARY} PATH)
get_filename_component( FreeTypePath ${FreeTypePath} PATH)
if ( EXISTS ${FreeTypePath}/bin/freetype6.dll )
	add_custom_command(
		TARGET ${PROJECT_NAME}
		POST_BUILD
		COMMAND
			${CMAKE_COMMAND} -E
				copy_if_different
				${FreeTypePath}/bin/freetype6.dll
				${PROJECTS_BINARIES_OUTPUT_DIR}/$<CONFIG>/bin
	)
	install(
		FILES ${FreeTypePath}/bin/freetype6.dll
		DESTINATION bin
		COMPONENT ${PROJECT_NAME}
	)
endif ()

set( Build "yes (version ${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_BUILD})" PARENT_SCOPE )