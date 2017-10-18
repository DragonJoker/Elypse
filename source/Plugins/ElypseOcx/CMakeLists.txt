project( ElypseOcx )

include( ComUtils )

set( ${PROJECT_NAME}_VERSION_MAJOR	${VERSION_MAJOR} )
set( ${PROJECT_NAME}_VERSION_MINOR	${VERSION_MINOR} )
set( ${PROJECT_NAME}_VERSION_BUILD	${VERSION_BUILD} )

include_directories(
	${CMAKE_SOURCE_DIR}/Core/ElypseCore/Src
	${CMAKE_CURRENT_SOURCE_DIR}/Src
)

find_package( ATL )
find_package( GDI )
find_package( GDIPlus )

if ( ATL_FOUND AND GDI_FOUND AND GDIPLUS_FOUND )
	message( STATUS "+ Found ATL" )
	message( STATUS "+ Found GDI" )
	message( STATUS "+ Found GDIPlus" )
	
	#ATL Libs
	set( AtlLibraries "" )
	FOREACH( Lib ${ATL_LIBRARIES} )
		if ( AtlLibraries )
			set( AtlLibraries "${AtlLibraries}|${Lib}" )
		else ()
			set( AtlLibraries "${Lib}" )
		endif ()
	ENDFOREACH()
	
	#GDI Libs
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

	# Define IDL files
	file( GLOB ${PROJECT_NAME}_IDL_FILES Src/*.idl )
	file( GLOB ${PROJECT_NAME}_DEF_FILES Src/*.def )

	# MIDL compiler
	add_com_interfaces( ${PROJECT_NAME}_SOURCES_IDL_FILES ${${PROJECT_NAME}_IDL_FILES} )
	set( ${PROJECT_NAME}_ADDITIONAL_FILES
		${${PROJECT_NAME}_IDL_FILES}
		${${PROJECT_NAME}_DEF_FILES}
		${${PROJECT_NAME}_SOURCES_IDL_FILES}
	)
	source_group( "IDL Files" FILES ${${PROJECT_NAME}_IDL_FILES} )
	source_group( "IDL Files" FILES ${${PROJECT_NAME}_SOURCES_IDL_FILES} )
	source_group( "IDL Files" FILES ${${PROJECT_NAME}_DEF_FILES} )

	add_target(
		${PROJECT_NAME}
		dll
		"GeneralLib;ElypseCore"
		"GeneralLib;ElypseCore;${ElypseCoreLibraries};${AtlLibraries};${GdiLibraries}"
		"ElypseOcxPch.h"
		"ElypseOcxPch.cpp"
		""
		""
		""
		"${${PROJECT_NAME}_ADDITIONAL_FILES}"
	)
	
	# Copy tlb files into binaries directory.
	add_custom_command( TARGET ${PROJECT_NAME}
		POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.tlb ${PROJECTS_BINARIES_OUTPUT_DIR}/$<CONFIGURATION>/bin/
	)
	
	# install tlb files
	install(
		FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.tlb
		DESTINATION bin
		COMPONENT ${PROJECT_NAME}
	)

	set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Plugins" )
	add_target_astyle( ${PROJECT_NAME} ".h;.hpp;.inl;.cpp" )

	set( Build "yes (version ${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_BUILD})" PARENT_SCOPE )
else ()
	set( Build "no (missing ATL libraries)" )
endif ()