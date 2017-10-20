project( ElypseFirefox )

set( ${PROJECT_NAME}_VERSION_MAJOR	${VERSION_MAJOR} )
set( ${PROJECT_NAME}_VERSION_MINOR	${VERSION_MINOR} )
set( ${PROJECT_NAME}_VERSION_BUILD	${VERSION_BUILD} )

include_directories(
	${CMAKE_SOURCE_DIR}/Core/ElypseCore/Src
	${CMAKE_CURRENT_SOURCE_DIR}/Src
)

if ( WIN32 )
	find_package( GDI )
	find_package( GDIPlus )
	set( GdiLibraries "" )
	FOREACH( Lib ${GDI_LIBRARY} )
		if ( GdiLibraries )
			set( GdiLibraries "${GdiLibraries}|${Lib}" )
		else ()
			set( GdiLibraries "${Lib}" )
		endif ()
	ENDFOREACH()
	FOREACH( Lib ${GDIPLUS_LIBRARY} )
		if ( GdiLibraries )
			set( GdiLibraries "${GdiLibraries}|${Lib}" )
		else ()
			set( GdiLibraries "${Lib}" )
		endif ()
	ENDFOREACH()
else ()
	set( GDI_FOUND 1 )
	set( GDIPLUS_FOUND 1 )
endif ()

if ( GDI_FOUND AND GDIPLUS_FOUND )
	add_target(
		${PROJECT_NAME}
		dll
		"GeneralLib;ElypseCore"
		"GeneralLib;ElypseCore;${ElypseCoreLibraries};${GdiLibraries}"
	)

	set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Plugins" )
	set( Build "yes (version ${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_BUILD})" PARENT_SCOPE )
	add_target_astyle( ${PROJECT_NAME} ".h;.hpp;.inl;.cpp" )

	parse_subdir_files( Src/Elypse "Elypse" )
	parse_subdir_files( Src/Firefox "Firefox" )
	parse_subdir_files( Src/NpApi "NpApi" )
endif ()