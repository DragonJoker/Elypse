project( ElypseCore )

set( ${PROJECT_NAME}_VERSION_MAJOR	${VERSION_MAJOR} )
set( ${PROJECT_NAME}_VERSION_MINOR	${VERSION_MINOR} )
set( ${PROJECT_NAME}_VERSION_BUILD	${VERSION_BUILD} )

set( CMAKE_CURRENT_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include/Core/${PROJECT_NAME} )

find_package( OpenCV ${EL_CONFIG} REQUIRED highgui core video )

set( ${PROJECT_NAME}_HDR_FILES
)
set( ${PROJECT_NAME}_SRC_FILES
)
source_group( "Header Files" FILES ${${PROJECT_NAME}_HDR_FILES} )
source_group( "Source Files" FILES ${${PROJECT_NAME}_SRC_FILES} )

set( ${PROJECT_NAME}_FOLDER_HDR_FILES
	${CMAKE_CURRENT_INCLUDE_DIR}/AnimatedObject.h
	${CMAKE_CURRENT_INCLUDE_DIR}/AnimatedObjectGroup.h
	${CMAKE_CURRENT_INCLUDE_DIR}/Animation.h
	${CMAKE_CURRENT_INCLUDE_DIR}/AnimationManager.h
	${CMAKE_CURRENT_INCLUDE_DIR}/Interpolators.h
	${CMAKE_CURRENT_INCLUDE_DIR}/Module_Animation.h
)
set( ${PROJECT_NAME}_FOLDER_SRC_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/AnimatedObject.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/AnimatedObjectGroup.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Animation.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/AnimationManager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Interpolators.cpp
)
set( ${PROJECT_NAME}_HDR_FILES
	${${PROJECT_NAME}_HDR_FILES}
	${${PROJECT_NAME}_FOLDER_HDR_FILES}
)
set( ${PROJECT_NAME}_SRC_FILES
	${${PROJECT_NAME}_SRC_FILES}
	${${PROJECT_NAME}_FOLDER_SRC_FILES}
)
source_group( "Header Files\\Animation" FILES ${${PROJECT_NAME}_FOLDER_HDR_FILES} )
source_group( "Source Files\\Animation" FILES ${${PROJECT_NAME}_FOLDER_SRC_FILES} )

set( ${PROJECT_NAME}_FOLDER_HDR_FILES
	${CMAKE_CURRENT_INCLUDE_DIR}/ConfigFile.h
	${CMAKE_CURRENT_INCLUDE_DIR}/DataFile.h
	${CMAKE_CURRENT_INCLUDE_DIR}/DataWriter.h
	${CMAKE_CURRENT_INCLUDE_DIR}/Module_Data.h
	${CMAKE_CURRENT_INCLUDE_DIR}/MuseFile.h
)
set( ${PROJECT_NAME}_FOLDER_SRC_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/ConfigFile.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/DataFile.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/DataWriter.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/MuseFile.cpp
)
set( ${PROJECT_NAME}_HDR_FILES
	${${PROJECT_NAME}_HDR_FILES}
	${${PROJECT_NAME}_FOLDER_HDR_FILES}
)
set( ${PROJECT_NAME}_SRC_FILES
	${${PROJECT_NAME}_SRC_FILES}
	${${PROJECT_NAME}_FOLDER_SRC_FILES}
)
source_group( "Header Files\\Data" FILES ${${PROJECT_NAME}_FOLDER_HDR_FILES} )
source_group( "Source Files\\Data" FILES ${${PROJECT_NAME}_FOLDER_SRC_FILES} )

set( ${PROJECT_NAME}_FOLDER_HDR_FILES
	${CMAKE_CURRENT_INCLUDE_DIR}/DownloadManager.h
	${CMAKE_CURRENT_INCLUDE_DIR}/Module_Download.h
	${CMAKE_CURRENT_INCLUDE_DIR}/MuseDownloader.h
	${CMAKE_CURRENT_INCLUDE_DIR}/MuseFilePack.h
	${CMAKE_CURRENT_INCLUDE_DIR}/MusePack.h
)
set( ${PROJECT_NAME}_FOLDER_SRC_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/DownloadManager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/MuseDownloader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/MuseFilePack.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/MusePack.cpp
)
set( ${PROJECT_NAME}_HDR_FILES
	${${PROJECT_NAME}_HDR_FILES}
	${${PROJECT_NAME}_FOLDER_HDR_FILES}
)
set( ${PROJECT_NAME}_SRC_FILES
	${${PROJECT_NAME}_SRC_FILES}
	${${PROJECT_NAME}_FOLDER_SRC_FILES}
)
source_group( "Header Files\\Download" FILES ${${PROJECT_NAME}_FOLDER_HDR_FILES} )
source_group( "Source Files\\Download" FILES ${${PROJECT_NAME}_FOLDER_SRC_FILES} )

set( ${PROJECT_NAME}_FOLDER_HDR_FILES
	${CMAKE_CURRENT_INCLUDE_DIR}/ClickableBox.h
	${CMAKE_CURRENT_INCLUDE_DIR}/ClickableCircle.h
	${CMAKE_CURRENT_INCLUDE_DIR}/ClickableImage.h
	${CMAKE_CURRENT_INCLUDE_DIR}/ClickableShape.h
	${CMAKE_CURRENT_INCLUDE_DIR}/EMGui.h
	${CMAKE_CURRENT_INCLUDE_DIR}/EMOverlay.h
	${CMAKE_CURRENT_INCLUDE_DIR}/EMOverlayGroup.h
	${CMAKE_CURRENT_INCLUDE_DIR}/MapIterator.h
	${CMAKE_CURRENT_INCLUDE_DIR}/Module_Gui.h
	${CMAKE_CURRENT_INCLUDE_DIR}/RawImage.h
)
set( ${PROJECT_NAME}_FOLDER_SRC_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/ClickableImage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/EMGui.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/EMOverlay.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/EMOverlayGroup.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/RawImage.cpp
)
set( ${PROJECT_NAME}_HDR_FILES
	${${PROJECT_NAME}_HDR_FILES}
	${${PROJECT_NAME}_FOLDER_HDR_FILES}
)
set( ${PROJECT_NAME}_SRC_FILES
	${${PROJECT_NAME}_SRC_FILES}
	${${PROJECT_NAME}_FOLDER_SRC_FILES}
)
source_group( "Header Files\\Gui" FILES ${${PROJECT_NAME}_FOLDER_HDR_FILES} )
source_group( "Source Files\\Gui" FILES ${${PROJECT_NAME}_FOLDER_SRC_FILES} )

set( ${PROJECT_NAME}_FOLDER_HDR_FILES
	${CMAKE_CURRENT_INCLUDE_DIR}/Context.h
	${CMAKE_CURRENT_INCLUDE_DIR}/Elypse.h
	${CMAKE_CURRENT_INCLUDE_DIR}/ElypseController.h
	${CMAKE_CURRENT_INCLUDE_DIR}/ElypseFrameListener.h
	${CMAKE_CURRENT_INCLUDE_DIR}/ElypseInstance.h
	${CMAKE_CURRENT_INCLUDE_DIR}/ElypseLoadingBar.h
	${CMAKE_CURRENT_INCLUDE_DIR}/ElypseLogs.h
	${CMAKE_CURRENT_INCLUDE_DIR}/ElypsePlugin.h
	${CMAKE_CURRENT_INCLUDE_DIR}/ElypseResourceGroupManager.h
	${CMAKE_CURRENT_INCLUDE_DIR}/Module_Main.h
	${CMAKE_CURRENT_INCLUDE_DIR}/OgrePlugins.h
	${CMAKE_CURRENT_INCLUDE_DIR}/Version.h
)
set( ${PROJECT_NAME}_FOLDER_SRC_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/ElypseController.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ElypseFrameListener.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ElypseInstance.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ElypseLoadingBar.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ElypseLogs.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ElypsePlugin.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ElypseResourceManager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/OgrePlugins.cpp
)
set( ${PROJECT_NAME}_HDR_FILES
	${${PROJECT_NAME}_HDR_FILES}
	${${PROJECT_NAME}_FOLDER_HDR_FILES}
)
set( ${PROJECT_NAME}_SRC_FILES
	${${PROJECT_NAME}_SRC_FILES}
	${${PROJECT_NAME}_FOLDER_SRC_FILES}
)
source_group( "Header Files\\Main" FILES ${${PROJECT_NAME}_FOLDER_HDR_FILES} )
source_group( "Source Files\\Main" FILES ${${PROJECT_NAME}_FOLDER_SRC_FILES} )

set( ${PROJECT_NAME}_FOLDER_HDR_FILES
	${CMAKE_CURRENT_INCLUDE_DIR}/Module_Media.h
)
set( ${PROJECT_NAME}_HDR_FILES
	${${PROJECT_NAME}_HDR_FILES}
	${${PROJECT_NAME}_FOLDER_HDR_FILES}
)
source_group( "Header Files\\Media" FILES ${${PROJECT_NAME}_FOLDER_HDR_FILES} )

set( ${PROJECT_NAME}_FOLDER_HDR_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/FmodApi.h
	${CMAKE_CURRENT_SOURCE_DIR}/OpenALApi.h
	${CMAKE_CURRENT_INCLUDE_DIR}/SoundApi.h
	${CMAKE_CURRENT_INCLUDE_DIR}/SoundInstance.h
	${CMAKE_CURRENT_INCLUDE_DIR}/SoundManager.h
	${CMAKE_CURRENT_INCLUDE_DIR}/SoundObject.h
	${CMAKE_CURRENT_INCLUDE_DIR}/SoundPlaylist.h
)
set( ${PROJECT_NAME}_FOLDER_SRC_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/FmodApi.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/OpenALApi.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/SoundInstance.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/SoundManager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/SoundObject.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/SoundPlaylist.cpp
)
set( ${PROJECT_NAME}_HDR_FILES
	${${PROJECT_NAME}_HDR_FILES}
	${${PROJECT_NAME}_FOLDER_HDR_FILES}
)
set( ${PROJECT_NAME}_SRC_FILES
	${${PROJECT_NAME}_SRC_FILES}
	${${PROJECT_NAME}_FOLDER_SRC_FILES}
)
source_group( "Header Files\\Media\\Sound" FILES ${${PROJECT_NAME}_FOLDER_HDR_FILES} )
source_group( "Source Files\\Media\\Sound" FILES ${${PROJECT_NAME}_FOLDER_SRC_FILES} )

set( ${PROJECT_NAME}_FOLDER_HDR_FILES
	${CMAKE_CURRENT_INCLUDE_DIR}/Video_DirectShow.h
	${CMAKE_CURRENT_INCLUDE_DIR}/Video_GStreamer.h
	${CMAKE_CURRENT_INCLUDE_DIR}/VideoFactory.h
	${CMAKE_CURRENT_INCLUDE_DIR}/VideoFactory_DirectShow.h
	${CMAKE_CURRENT_INCLUDE_DIR}/VideoFactory_GStreamer.h
	${CMAKE_CURRENT_INCLUDE_DIR}/VideoImplementation.h
	${CMAKE_CURRENT_INCLUDE_DIR}/VideoInstance.h
	${CMAKE_CURRENT_INCLUDE_DIR}/VideoManager.h
	${CMAKE_CURRENT_INCLUDE_DIR}/VideoObject.h
	${CMAKE_CURRENT_INCLUDE_DIR}/VideoOverlay.h
	${CMAKE_CURRENT_INCLUDE_DIR}/VideoTexture.h
)
set( ${PROJECT_NAME}_FOLDER_SRC_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/Video_DirectShow.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Video_GStreamer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/VideoFactory_DirectShow.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/VideoFactory_GStreamer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/VideoInstance.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/VideoManager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/VideoObject.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/VideoOverlay.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/VideoTexture.cpp
)
set( ${PROJECT_NAME}_HDR_FILES
	${${PROJECT_NAME}_HDR_FILES}
	${${PROJECT_NAME}_FOLDER_HDR_FILES}
)
set( ${PROJECT_NAME}_SRC_FILES
	${${PROJECT_NAME}_SRC_FILES}
	${${PROJECT_NAME}_FOLDER_SRC_FILES}
)
source_group( "Header Files\\Media\\Video" FILES ${${PROJECT_NAME}_FOLDER_HDR_FILES} )
source_group( "Source Files\\Media\\Video" FILES ${${PROJECT_NAME}_FOLDER_SRC_FILES} )

set( ${PROJECT_NAME}_FOLDER_HDR_FILES
	${CMAKE_CURRENT_INCLUDE_DIR}/Module_Network.h
	${CMAKE_CURRENT_INCLUDE_DIR}/NetworkManager.h
)
set( ${PROJECT_NAME}_FOLDER_SRC_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/NetworkManager.cpp
)
set( ${PROJECT_NAME}_HDR_FILES
	${${PROJECT_NAME}_HDR_FILES}
	${${PROJECT_NAME}_FOLDER_HDR_FILES}
)
set( ${PROJECT_NAME}_SRC_FILES
	${${PROJECT_NAME}_SRC_FILES}
	${${PROJECT_NAME}_FOLDER_SRC_FILES}
)
source_group( "Header Files\\Network" FILES ${${PROJECT_NAME}_FOLDER_HDR_FILES} )
source_group( "Source Files\\Network" FILES ${${PROJECT_NAME}_FOLDER_SRC_FILES} )

set( ${PROJECT_NAME}_FOLDER_HDR_FILES
	${CMAKE_CURRENT_INCLUDE_DIR}/BoundingMesh.h
	${CMAKE_CURRENT_INCLUDE_DIR}/BoundingShape.h
	${CMAKE_CURRENT_INCLUDE_DIR}/Mass.h
	${CMAKE_CURRENT_INCLUDE_DIR}/Module_Physics.h
	${CMAKE_CURRENT_INCLUDE_DIR}/PhysicsEngine.h
	${CMAKE_CURRENT_INCLUDE_DIR}/PhysicsMaterial.h
	${CMAKE_CURRENT_INCLUDE_DIR}/PhysicsObject.h
	${CMAKE_CURRENT_INCLUDE_DIR}/PhysicsSimulation.h
	${CMAKE_CURRENT_INCLUDE_DIR}/SimpleBoundingShapes.h
	${CMAKE_CURRENT_INCLUDE_DIR}/Space.h
)
set( ${PROJECT_NAME}_FOLDER_SRC_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/BoundingMesh.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/BoundingShape.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Mass.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/PhysicsEngine.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/PhysicsMaterial.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/PhysicsObject.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/PhysicsSimulation.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/SimpleBoundingShapes.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Space.cpp
)
set( ${PROJECT_NAME}_HDR_FILES
	${${PROJECT_NAME}_HDR_FILES}
	${${PROJECT_NAME}_FOLDER_HDR_FILES}
)
set( ${PROJECT_NAME}_SRC_FILES
	${${PROJECT_NAME}_SRC_FILES}
	${${PROJECT_NAME}_FOLDER_SRC_FILES}
)
source_group( "Header Files\\Physics" FILES ${${PROJECT_NAME}_FOLDER_HDR_FILES} )
source_group( "Source Files\\Physics" FILES ${${PROJECT_NAME}_FOLDER_SRC_FILES} )

set( ${PROJECT_NAME}_FOLDER_HDR_FILES
	${CMAKE_CURRENT_INCLUDE_DIR}/Module_Pub.h
	${CMAKE_CURRENT_INCLUDE_DIR}/PubInstance.h
	${CMAKE_CURRENT_INCLUDE_DIR}/PubManager.h
	${CMAKE_CURRENT_INCLUDE_DIR}/PubObject.h
)
set( ${PROJECT_NAME}_FOLDER_SRC_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/PubInstance.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/PubManager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/PubObject.cpp
)
set( ${PROJECT_NAME}_HDR_FILES
	${${PROJECT_NAME}_HDR_FILES}
	${${PROJECT_NAME}_FOLDER_HDR_FILES}
)
set( ${PROJECT_NAME}_SRC_FILES
	${${PROJECT_NAME}_SRC_FILES}
	${${PROJECT_NAME}_FOLDER_SRC_FILES}
)
source_group( "Header Files\\Pub" FILES ${${PROJECT_NAME}_FOLDER_HDR_FILES} )
source_group( "Source Files\\Pub" FILES ${${PROJECT_NAME}_FOLDER_SRC_FILES} )

set( ${PROJECT_NAME}_FOLDER_HDR_FILES
	${CMAKE_CURRENT_INCLUDE_DIR}/Module_SceneFile.h
	${CMAKE_CURRENT_INCLUDE_DIR}/SceneFileParser.h
	${CMAKE_CURRENT_INCLUDE_DIR}/SceneFileParser_Parsers.h
	${CMAKE_CURRENT_INCLUDE_DIR}/SceneFileSaver.h
)
set( ${PROJECT_NAME}_FOLDER_SRC_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/SceneFileParser.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/SceneFileParser_Parsers.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/SceneFileSaver.cpp
)
set( ${PROJECT_NAME}_HDR_FILES
	${${PROJECT_NAME}_HDR_FILES}
	${${PROJECT_NAME}_FOLDER_HDR_FILES}
)
set( ${PROJECT_NAME}_SRC_FILES
	${${PROJECT_NAME}_SRC_FILES}
	${${PROJECT_NAME}_FOLDER_SRC_FILES}
)
source_group( "Header Files\\SceneFile" FILES ${${PROJECT_NAME}_FOLDER_HDR_FILES} )
source_group( "Source Files\\SceneFile" FILES ${${PROJECT_NAME}_FOLDER_SRC_FILES} )

set( ${PROJECT_NAME}_FOLDER_HDR_FILES
	${CMAKE_CURRENT_INCLUDE_DIR}/Function.h
	${CMAKE_CURRENT_INCLUDE_DIR}/Module_Script.h
	${CMAKE_CURRENT_INCLUDE_DIR}/ScriptBlock.h
	${CMAKE_CURRENT_INCLUDE_DIR}/ScriptCompiler.h
	${CMAKE_CURRENT_INCLUDE_DIR}/ScriptEngine.h
	${CMAKE_CURRENT_INCLUDE_DIR}/ScriptFunctionOperators.h
	${CMAKE_CURRENT_INCLUDE_DIR}/ScriptFunctions.h
	${CMAKE_CURRENT_INCLUDE_DIR}/ScriptNode.h
	${CMAKE_CURRENT_INCLUDE_DIR}/ScriptNodeValue.h
	${CMAKE_CURRENT_INCLUDE_DIR}/ScriptTimer.h
	${CMAKE_CURRENT_INCLUDE_DIR}/ScriptTimerManager.h
	${CMAKE_CURRENT_INCLUDE_DIR}/Structure.h
	${CMAKE_CURRENT_INCLUDE_DIR}/VariableType.h
)
set( ${PROJECT_NAME}_FOLDER_SRC_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/ScriptBlock.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ScriptCompiler.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ScriptCompiler_Compile.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ScriptEngine.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ScriptFunctions.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ScriptFunctions_Gen_Ope.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ScriptFunctions_Ogre.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ScriptNode.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ScriptTimer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ScriptTimerManager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Structure.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/VariableType.cpp
)
set( ${PROJECT_NAME}_HDR_FILES
	${${PROJECT_NAME}_HDR_FILES}
	${${PROJECT_NAME}_FOLDER_HDR_FILES}
)
set( ${PROJECT_NAME}_SRC_FILES
	${${PROJECT_NAME}_SRC_FILES}
	${${PROJECT_NAME}_FOLDER_SRC_FILES}
)
source_group( "Header Files\\Script" FILES ${${PROJECT_NAME}_FOLDER_HDR_FILES} )
source_group( "Source Files\\Script" FILES ${${PROJECT_NAME}_FOLDER_SRC_FILES} )

set( ${PROJECT_NAME}_FOLDER_HDR_FILES
	${CMAKE_CURRENT_INCLUDE_DIR}/BaseEvent.h
	${CMAKE_CURRENT_INCLUDE_DIR}/BaseFrame.h
	${CMAKE_CURRENT_INCLUDE_DIR}/EventFactory.h
	${CMAKE_CURRENT_INCLUDE_DIR}/EventTarget.h
	${CMAKE_CURRENT_INCLUDE_DIR}/Events.h
	${CMAKE_CURRENT_INCLUDE_DIR}/Module_Sequences.h
	${CMAKE_CURRENT_INCLUDE_DIR}/Sequence.h
	${CMAKE_CURRENT_INCLUDE_DIR}/SequenceManager.h
)
set( ${PROJECT_NAME}_FOLDER_SRC_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/EventFactory.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Events.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Sequence.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/SequenceManager.cpp
)
set( ${PROJECT_NAME}_HDR_FILES
	${${PROJECT_NAME}_HDR_FILES}
	${${PROJECT_NAME}_FOLDER_HDR_FILES}
)
set( ${PROJECT_NAME}_SRC_FILES
	${${PROJECT_NAME}_SRC_FILES}
	${${PROJECT_NAME}_FOLDER_SRC_FILES}
)
source_group( "Header Files\\Sequences" FILES ${${PROJECT_NAME}_FOLDER_HDR_FILES} )
source_group( "Source Files\\Sequences" FILES ${${PROJECT_NAME}_FOLDER_SRC_FILES} )

set( ${PROJECT_NAME}_FOLDER_HDR_FILES
	${CMAKE_CURRENT_INCLUDE_DIR}/Module_Simulation.h
	${CMAKE_CURRENT_INCLUDE_DIR}/ParametricLayer.h
	${CMAKE_CURRENT_INCLUDE_DIR}/ParametricSurface.h
	${CMAKE_CURRENT_INCLUDE_DIR}/Surface.h
	${CMAKE_CURRENT_INCLUDE_DIR}/SurfaceManager.h
)
set( ${PROJECT_NAME}_FOLDER_SRC_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/ParametricLayer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ParametricSurface.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Surface.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/SurfaceManager.cpp
)
set( ${PROJECT_NAME}_HDR_FILES
	${${PROJECT_NAME}_HDR_FILES}
	${${PROJECT_NAME}_FOLDER_HDR_FILES}
)
set( ${PROJECT_NAME}_SRC_FILES
	${${PROJECT_NAME}_SRC_FILES}
	${${PROJECT_NAME}_FOLDER_SRC_FILES}
)
source_group( "Header Files\\Simulation" FILES ${${PROJECT_NAME}_FOLDER_HDR_FILES} )
source_group( "Source Files\\Simulation" FILES ${${PROJECT_NAME}_FOLDER_SRC_FILES} )

set( ${PROJECT_NAME}_FOLDER_HDR_FILES
	${CMAKE_CURRENT_INCLUDE_DIR}/CamTex.h
	${CMAKE_CURRENT_INCLUDE_DIR}/MaterialTarget.h
	${CMAKE_CURRENT_INCLUDE_DIR}/Mirror.h
	${CMAKE_CURRENT_INCLUDE_DIR}/MirrorManager.h
	${CMAKE_CURRENT_INCLUDE_DIR}/MirrorMaterial.h
	${CMAKE_CURRENT_INCLUDE_DIR}/Module_SpecialEffects.h
	${CMAKE_CURRENT_INCLUDE_DIR}/ObjectMaterial.h
	${CMAKE_CURRENT_INCLUDE_DIR}/PostEffect.h
	${CMAKE_CURRENT_INCLUDE_DIR}/PostEffect_GaussianBlur.h
	${CMAKE_CURRENT_INCLUDE_DIR}/PostEffect_HDR.h
	${CMAKE_CURRENT_INCLUDE_DIR}/PostEffect_LensFlare.h
	${CMAKE_CURRENT_INCLUDE_DIR}/PostEffect_MotionBlur.h
	${CMAKE_CURRENT_INCLUDE_DIR}/WebImage.h
	${CMAKE_CURRENT_INCLUDE_DIR}/WebImageManager.h
)
set( ${PROJECT_NAME}_FOLDER_SRC_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/CamTex.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/MaterialTarget.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Mirror.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/MirrorManager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/MirrorMaterial.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ObjectMaterial.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/PostEffect.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/PostEffect_GaussianBlur.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/PostEffect_HDR.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/PostEffect_LensFlare.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/PostEffect_MotionBlur.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/WebImage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/WebImageManager.cpp
)
set( ${PROJECT_NAME}_HDR_FILES
	${${PROJECT_NAME}_HDR_FILES}
	${${PROJECT_NAME}_FOLDER_HDR_FILES}
)
set( ${PROJECT_NAME}_SRC_FILES
	${${PROJECT_NAME}_SRC_FILES}
	${${PROJECT_NAME}_FOLDER_SRC_FILES}
)
source_group( "Header Files\\SpecialEffects" FILES ${${PROJECT_NAME}_FOLDER_HDR_FILES} )
source_group( "Source Files\\SpecialEffects" FILES ${${PROJECT_NAME}_FOLDER_SRC_FILES} )

set( ${PROJECT_NAME}_FOLDER_HDR_FILES
	${CMAKE_CURRENT_INCLUDE_DIR}/Module_Zone.h
	${CMAKE_CURRENT_INCLUDE_DIR}/Portal.h
	${CMAKE_CURRENT_INCLUDE_DIR}/Universe.h
	${CMAKE_CURRENT_INCLUDE_DIR}/Zone.h
	${CMAKE_CURRENT_INCLUDE_DIR}/ZoneObject.h
)
set( ${PROJECT_NAME}_FOLDER_SRC_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/Portal.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Universe.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Zone.cpp
)
set( ${PROJECT_NAME}_HDR_FILES
	${${PROJECT_NAME}_HDR_FILES}
	${${PROJECT_NAME}_FOLDER_HDR_FILES}
)
set( ${PROJECT_NAME}_SRC_FILES
	${${PROJECT_NAME}_SRC_FILES}
	${${PROJECT_NAME}_FOLDER_SRC_FILES}
)
source_group( "Header Files\\Zone" FILES ${${PROJECT_NAME}_FOLDER_HDR_FILES} )
source_group( "Source Files\\Zone" FILES ${${PROJECT_NAME}_FOLDER_SRC_FILES} )

add_target_min(
	${PROJECT_NAME}
	api_dll
	${CMAKE_CURRENT_INCLUDE_DIR}/PrecompiledHeader.h
	${CMAKE_CURRENT_SOURCE_DIR}/PrecompiledHeader.cpp
)
add_library( elypse::Core
	ALIAS
	${PROJECT_NAME}
)
target_include_directories( ${PROJECT_NAME}
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_INCLUDE_DIR}>
		$<INSTALL_INTERFACE:include>
		${Ogre_INCLUDE_DIR}
	PRIVATE
		${OpenCV_INCLUDE_DIR}
		${OpenCV_INCLUDE_DIRS}
		${CMAKE_SOURCE_DIR}/Core/${PROJECT_NAME}/Win32
)
target_link_libraries( ${PROJECT_NAME}
	PUBLIC
		elypse::General
		OgreMain
		OgreOverlay
		ODE::ODE
	PRIVATE
		opencv_core
		opencv_highgui
		opencv_video
		${CMAKE_DL_LIBS}
)
if ( BUILD_USE_FMOD )
	target_link_libraries( ${PROJECT_NAME}
		PRIVATE
			Fmod::Fmod
	)
	target_compile_definitions( ${PROJECT_NAME}
		PRIVATE
			ELYPSE_USE_FMOD
	)
else ()
	target_link_libraries( ${PROJECT_NAME}
		PRIVATE
			OpenAL::OpenAL
			SndFile::sndfile
			mpg123::mpg123
	)
	target_compile_definitions( ${PROJECT_NAME}
		PRIVATE
			ELYPSE_USE_OPENAL
	)
endif ()
target_install_headers( ${PROJECT_NAME} ${CMAKE_CURRENT_INCLUDE_DIR} )
set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Core" )
add_target_astyle( ${PROJECT_NAME} ".h;.hpp;.inl;.cpp" )
target_add_doc( ${PROJECT_NAME} "" "*.h *.hpp *.dox" )

# Copy otherwise unreferenced dlls to bin folder
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
copy_dll( ${PROJECT_NAME}
	${OGRE_LIBRARY_DIR_DEBUG}/Codec_STBI_d.lib
	${OGRE_LIBRARY_DIR}/Codec_STBI.lib
)
if ( BUILD_USE_FMOD AND WIN32 )
	copy_dll( ${PROJECT_NAME}
		${Fmod_LIBRARY_DIR}/fmod.dll
		${Fmod_LIBRARY_DIR}/fmod.dll
	)
endif ()
copy_target_directory( ${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/Data/cfg cfg )

set( Build "yes (version ${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_BUILD})" PARENT_SCOPE )
