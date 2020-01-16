project( ElypseCore )

set( ${PROJECT_NAME}_VERSION_MAJOR	${VERSION_MAJOR} )
set( ${PROJECT_NAME}_VERSION_MINOR	${VERSION_MINOR} )
set( ${PROJECT_NAME}_VERSION_BUILD	${VERSION_BUILD} )

find_package( OpenCV CONFIG REQUIRED highgui core video )

include_directories(  )

add_target(
	${PROJECT_NAME}
	api_dll
	""
	""
	"PrecompiledHeader.h"
	"PrecompiledHeader.cpp"
)
target_include_directories( ${PROJECT_NAME}
	PUBLIC
		${CMAKE_SOURCE_DIR}/Core/ElypseCore/Src
		${Ogre_INCLUDE_DIR}
	PRIVATE
		${Fmod_INCLUDE_DIR}
		${OpenCV_INCLUDE_DIR}
		${OpenCV_INCLUDE_DIRS}
)
target_link_libraries( ${PROJECT_NAME}
	PUBLIC
		GeneralLib
		OgreMain
		OgreOverlay
	PRIVATE
		${Fmod_LIBRARY}
		opencv_core
		opencv_highgui
		opencv_video
		ODE::ODE
		${CMAKE_DL_LIBS}
)

set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Core" )
set( Build "yes (version ${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_BUILD})" PARENT_SCOPE )
add_target_astyle( ${PROJECT_NAME} ".h;.hpp;.inl;.cpp" )

copy_dll( ${PROJECT_NAME}
	${Fmod_LIBRARY_DIR}/fmod.dll
	${Fmod_LIBRARY_DIR}/fmod.dll
)
get_filename_component( OGRE_LIBRARY_DIR_DEBUG ${OGRE_LIBRARY_DIRS} PATH )
set( OGRE_LIBRARY_DIR ${OGRE_LIBRARY_DIRS} )
set( OGRE_LIBRARY_DIR_DEBUG ${OGRE_LIBRARY_DIR_DEBUG}/debug/lib )
copy_dll( ${PROJECT_NAME}
	${OGRE_LIBRARY_DIR_DEBUG}/RenderSystem_Direct3D11_d.lib
	${OGRE_LIBRARY_DIR}/RenderSystem_Direct3D11.lib
)
copy_dll( ${PROJECT_NAME}
	${OGRE_LIBRARY_DIR_DEBUG}/RenderSystem_GL_d.lib
	${OGRE_LIBRARY_DIR}/RenderSystem_GL.lib
)
copy_dll( ${PROJECT_NAME}
	${OGRE_LIBRARY_DIR_DEBUG}/RenderSystem_GL3Plus_d.lib
	${OGRE_LIBRARY_DIR}/RenderSystem_GL3Plus.lib
)
copy_dll( ${PROJECT_NAME}
	${OGRE_LIBRARY_DIR_DEBUG}/Plugin_ParticleFX_d.lib
	${OGRE_LIBRARY_DIR}/Plugin_ParticleFX.lib
)

set( Build "yes (version ${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_BUILD})" PARENT_SCOPE )
