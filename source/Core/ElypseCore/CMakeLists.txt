project( ElypseCore )

set( ${PROJECT_NAME}_VERSION_MAJOR	${VERSION_MAJOR} )
set( ${PROJECT_NAME}_VERSION_MINOR	${VERSION_MINOR} )
set( ${PROJECT_NAME}_VERSION_BUILD	${VERSION_BUILD} )

find_package( OpenCV COMPONENTS highgui core video )

if( OPENCV_FOUND OR OpenCV_FOUND )
	message( STATUS "+ Found OpenCV (version ${OpenCV_VERSION})" )
	include_directories( ${OpenCV_INCLUDE_DIR} )
	include_directories( ${OpenCV_INCLUDE_DIRS} )
	link_directories( ${OpenCV_LINK_DIRECTORIES} )
endif ()

include_directories( ${CMAKE_SOURCE_DIR}/Core/ElypseCore/Src )

#OpenCV libs
set( OpenCVLibraries "")
set( OpenCV_LIBS
	${OpenCV_LIBS}
	${OpenCV_core_LIBRARY}
	${OpenCV_highgui_LIBRARY}
)
foreach(Lib ${OpenCV_LIBS})
	if (OpenCVLibraries)
		set(OpenCVLibraries "${OpenCVLibraries}|${Lib}")
	else()
		set(OpenCVLibraries "${Lib}")
	endif()
endforeach()

add_target(
	${PROJECT_NAME}
	api_dll
	"GeneralLib"
	"GeneralLib;${ElypseCoreLibraries};${OpenCVLibraries}"
	"PrecompiledHeader.h"
	"PrecompiledHeader.cpp"
)

set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Core" )
set( Build "yes (version ${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_BUILD})" PARENT_SCOPE )
add_target_astyle( ${PROJECT_NAME} ".h;.hpp;.inl;.cpp" )

copy_dll( ${PROJECT_NAME} "${Boost_DATE_TIME_LIBRARY_DEBUG}" Debug )
copy_dll( ${PROJECT_NAME} "${Boost_DATE_TIME_LIBRARY_RELEASE}" Release )
copy_dll( ${PROJECT_NAME} "${Boost_FILESYSTEM_LIBRARY_DEBUG}" Debug )
copy_dll( ${PROJECT_NAME} "${Boost_FILESYSTEM_LIBRARY_RELEASE}" Release )
copy_dll( ${PROJECT_NAME} "${Boost_SYSTEM_LIBRARY_DEBUG}" Debug )
copy_dll( ${PROJECT_NAME} "${Boost_SYSTEM_LIBRARY_RELEASE}" Release )
copy_dll( ${PROJECT_NAME} "${Fmod_LIBRARY_DIR}/../fmodex.dll" Debug )
copy_dll( ${PROJECT_NAME} "${Fmod_LIBRARY_DIR}/../fmodex.dll" Release )

if ( WIN32 )
	copy_dll( ${PROJECT_NAME} "${Ogre_Main_LIBRARY_DEBUG}" Debug )
	copy_dll( ${PROJECT_NAME} "${Ogre_Main_LIBRARY_RELEASE}" Release )
	copy_dll( ${PROJECT_NAME} "${Ogre_Overlay_LIBRARY_DEBUG}" Debug )
	copy_dll( ${PROJECT_NAME} "${Ogre_Overlay_LIBRARY_RELEASE}" Release )
	copy_dll( ${PROJECT_NAME} "${Ogre_ROOT_DIR}/bin/Debug/RenderSystem_Direct3D11.dll" Debug "_d" )
	copy_dll( ${PROJECT_NAME} "${Ogre_ROOT_DIR}/bin/Release/RenderSystem_Direct3D11.dll" Release )
	copy_dll( ${PROJECT_NAME} "${Ogre_ROOT_DIR}/bin/Debug/RenderSystem_GL.dll" Debug "_d" )
	copy_dll( ${PROJECT_NAME} "${Ogre_ROOT_DIR}/bin/Release/RenderSystem_GL.dll" Release )
	copy_dll( ${PROJECT_NAME} "${Ogre_ROOT_DIR}/bin/Debug/RenderSystem_GL3Plus.dll" Debug "_d" )
	copy_dll( ${PROJECT_NAME} "${Ogre_ROOT_DIR}/bin/Release/RenderSystem_GL3Plus.dll" Release )
	copy_dll( ${PROJECT_NAME} "${Ogre_ROOT_DIR}/bin/Debug/Plugin_CgProgramManager.dll" Debug "_d" )
	copy_dll( ${PROJECT_NAME} "${Ogre_ROOT_DIR}/bin/Release/Plugin_CgProgramManager.dll" Release )
	copy_dll( ${PROJECT_NAME} "${Ogre_ROOT_DIR}/bin/Debug/Plugin_ParticleFX.dll" Debug "_d" )
	copy_dll( ${PROJECT_NAME} "${Ogre_ROOT_DIR}/bin/Release/Plugin_ParticleFX.dll" Release )
else ()
	copy_dll( ${PROJECT_NAME} "${Ogre_Main_LIBRARY}" Debug )
	copy_dll( ${PROJECT_NAME} "${Ogre_Main_LIBRARY}" Release )
	copy_dll( ${PROJECT_NAME} "${Ogre_Overlay_LIBRARY}" Debug )
	copy_dll( ${PROJECT_NAME} "${Ogre_Overlay_LIBRARY}" Release )
	copy_dll( ${PROJECT_NAME} "${Ogre_RenderSystem_GL_LIBRARY}" Debug )
	copy_dll( ${PROJECT_NAME} "${Ogre_RenderSystem_GL_LIBRARY}" Release )
	copy_dll( ${PROJECT_NAME} "${Ogre_Plugin_ParticleFX_LIBRARY}" Debug )
	copy_dll( ${PROJECT_NAME} "${Ogre_Plugin_ParticleFX_LIBRARY}" Release )
endif ()

if( OPENCV_FOUND OR OpenCV_FOUND )
	# Preparing OpenCV dlls installation
	set( OpenCV_DLLS )
	set( OpenCVd_DLLS )
	get_filename_component( OpenCvLibPath "${OpenCV_LIB_DIR_OPT}" PATH )
	string( REPLACE "." "" Version "${OpenCV_VERSION}" )
	foreach ( LIB ${OpenCV_LIBS} )
		set( OpenCvLibPathName ${OpenCvLibPath}/bin/${OpenCvLibName}${Version}.dll )
		if ( EXISTS ${OpenCvLibPathName} )
			add_custom_command(
				TARGET ${PROJECT_NAME}
				POST_BUILD
				COMMAND if 1==$<CONFIG:Release>
					${CMAKE_COMMAND} -E
						copy_if_different
						${OpenCvLibPathName}
						${PROJECTS_BINARIES_OUTPUT_DIR}/$<CONFIGURATION>/bin
			)
			set( OpenCV_DLLS
				${OpenCV_DLLS}
				${OpenCvLibPathName}
			)
		else ()
			get_filename_component( OpenCvLibName ${LIB} NAME_WE )
			set( OpenCvLibPathName ${OpenCV_CONFIG_PATH}/bin${OpenCV_LIB_DIR_OPT}/${OpenCvLibName}${Version}.dll )
			if ( EXISTS ${OpenCvLibPathName} )
				add_custom_command(
					TARGET ${PROJECT_NAME}
					POST_BUILD
					COMMAND if 1==$<CONFIG:Release>
						${CMAKE_COMMAND} -E
							copy_if_different
							${OpenCvLibPathName}
							${PROJECTS_BINARIES_OUTPUT_DIR}/$<CONFIGURATION>/bin
				)
				set( OpenCV_DLLS
					${OpenCV_DLLS}
					${OpenCvLibPathName}
				)
			else ()
				if ( ${OpenCvLibName} STREQUAL "debug" )
					set( OpenCV_CONFIG "Debug" )
				elseif ( ${OpenCvLibName} STREQUAL "optimized" )
					set( OpenCV_CONFIG "Release" )
				elseif ( OpenCV_CONFIG )
					if ( ${OpenCV_CONFIG} STREQUAL "Debug" )
						set( OpenCvLibPathName ${OpenCvLibPath}/bin/Debug/${OpenCvLibName}.dll )
						if ( EXISTS ${OpenCvLibPathName} )
							add_custom_command(
								TARGET ${PROJECT_NAME}
								POST_BUILD
								COMMAND if 1==$<CONFIG:Debug>
									${CMAKE_COMMAND} -E
										copy_if_different
										${OpenCvLibPathName}
										${PROJECTS_BINARIES_OUTPUT_DIR}/$<CONFIGURATION>/bin
							)
							set( OpenCVd_DLLS
								${OpenCVd_DLLS}
								${OpenCvLibPathName}
							)
						endif ()
					elseif ( ${OpenCV_CONFIG} STREQUAL "Release" )
						set( OpenCvLibPathName ${OpenCvLibPath}/bin/Release/${OpenCvLibName}.dll )
						if ( EXISTS ${OpenCvLibPathName} )
							add_custom_command(
								TARGET ${PROJECT_NAME}
								POST_BUILD
								COMMAND if 1==$<CONFIG:Release>
									${CMAKE_COMMAND} -E
										copy_if_different
										${OpenCvLibPathName}
										${PROJECTS_BINARIES_OUTPUT_DIR}/$<CONFIGURATION>/bin
							)
							set( OpenCV_DLLS
								${OpenCV_DLLS}
								${OpenCvLibPathName}
							)
						endif ()
					endif ()
				endif ()
			endif ()
		endif ()
		set( OpenCvLibPathNamed ${OpenCvLibPath}/bin/${OpenCvLibName}${Version}d.dll )
		if ( EXISTS ${OpenCvLibPathNamed} )
			add_custom_command(
				TARGET ${PROJECT_NAME}
				POST_BUILD
				COMMAND if 1==$<CONFIG:Debug>
					${CMAKE_COMMAND} -E
						copy_if_different
						${OpenCvLibPathNamed}
						${PROJECTS_BINARIES_OUTPUT_DIR}/$<CONFIGURATION>/bin
			)
			set( OpenCVd_DLLS
				${OpenCVd_DLLS}
				${OpenCvLibPathNamed}
			)
		else ()
			set( OpenCvLibPathNamed ${OpenCV_CONFIG_PATH}/bin${OpenCV_LIB_DIR_DBG}/${OpenCvLibName}${Version}d.dll )
			if ( EXISTS ${OpenCvLibPathNamed} )
				add_custom_command(
					TARGET ${PROJECT_NAME}
					POST_BUILD
					COMMAND if 1==$<CONFIG:Debug>
						${CMAKE_COMMAND} -E
							copy_if_different
							${OpenCvLibPathNamed}
							${PROJECTS_BINARIES_OUTPUT_DIR}/$<CONFIGURATION>/bin
				)
				set( OpenCV_DLLS
					${OpenCV_DLLS}
					${OpenCvLibPathNamed}
				)
			endif ()
		endif ()
	endforeach ()
	install(
		FILES ${OpenCV_DLLS}
		DESTINATION bin
		CONFIGURATIONS Release
		COMPONENT ${PROJECT_NAME}
	)
	install(
		FILES ${OpenCVd_DLLS}
		DESTINATION bin
		CONFIGURATIONS Debug
		COMPONENT ${PROJECT_NAME}
	)
endif ()

set( Build "yes (version ${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_BUILD})" PARENT_SCOPE )
