/*
See LICENSE file in root folder
*/
#ifndef ___PRECOMPILED_HEADER_H___
#define ___PRECOMPILED_HEADER_H___

#pragma warning( disable:4251 )
#pragma warning( disable:4275 )

#define WINVER 0x0501
#define _WIN32_WINNT 0x0501
#define _WIN32_WINDOWS 0x0501

#define ____GENLIB_NO_MEMORY_DEBUG____

#define BOOST_REGEX_NO_LIB
#define BOOST_DATE_TIME_NO_LIB
#define BOOST_NO_TYPEID
#define BOOST_ASIO_DISABLE_BUFFER_DEBUGGING
#define BOOST_ASIO_NO_DEFAULT_LINKED_LIBS

#pragma warning( push )
#pragma warning( disable:4996 )

#pragma warning( push )
#pragma warning( disable:4267 )
#pragma warning( disable:4312 )
#pragma warning( disable:4311 )
#include <boost/asio.hpp>
#if defined( _WIN32 )
#	if defined( _MSC_VER )
#		if _MSC_VER >= 1700
#			include <thread>
#			include <mutex>
#		else
#			include <boost/thread/condition.hpp>
#			include <boost/thread/mutex.hpp>
#			include <boost/thread/recursive_mutex.hpp>
#			include <boost/thread/thread.hpp>
#		endif
#	else
#		include <boost/thread/condition.hpp>
#		include <boost/thread/mutex.hpp>
#		include <boost/thread/recursive_mutex.hpp>
#		include <boost/thread/thread.hpp>
#	endif
#else
#	include <thread>
#	include <mutex>
#endif
#include <boost/filesystem.hpp>
#pragma warning( pop )

#include <Elypse.h>

#include <cstdarg>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <set>
#include <list>
#include <stdio.h>
#include <ctime>
#include <queue>

#pragma warning( push )
#pragma warning( disable:4267 )
#pragma warning( disable:4312 )
#pragma warning( disable:4311 )
#include <OgrePrerequisites.h>
#include <OgreAnimation.h>
#include <OgreAnimationState.h>
#include <OgreAxisAlignedBox.h>
#include <OgreBorderPanelOverlayElement.h>
#include <OgreCamera.h>
#include <OgreColourValue.h>
#include <OgreDataStream.h>
#include <OgreEdgeListBuilder.h>
#include <OgreEntity.h>
#include <OgreException.h>
#include <OgreHardwarePixelBuffer.h>
#include <OgreLight.h>
#include <OgreLog.h>
#include <OgreLogManager.h>
#include <OgreManualObject.h>
#include <OgreMaterial.h>
#include <OgreMaterialManager.h>
#include <OgreMesh.h>
#include <OgreMeshManager.h>
#include <OgreMovablePlane.h>
#include <OgreOverlay.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OgreParticleSystem.h>
#include <OgreParticleSystemManager.h>
#include <OgreRay.h>
#include <OgreRenderSystem.h>
#include <OgreRenderTarget.h>
#include <OgreRenderTexture.h>
#include <OgreRenderWindow.h>
#include <OgreResourceGroupManager.h>
#include <OgreRoot.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreSingleton.h>
#include <OgreString.h>
#include <OgreStringConverter.h>
#include <OgreSubEntity.h>
#include <OgreSubMesh.h>
#include <OgreTexture.h>
#include <OgreTextureManager.h>
#include <OgreTextAreaOverlayElement.h>
#include <OgreVector.h>
#include <OgreViewport.h>
#pragma warning( pop )

#include <Exception.h>
#include <File.h>
#include <Memory.h>
#include <Mutex.h>
#include <OwnedBy.h>
#include <Singleton.h>
#include <STLMacros.h>
#include <STLMapMacros.h>
#include <Utils.h>

#include <AnimatedObjectGroup.h>
#include <Animation.h>
#include <AnimationManager.h>
#include <BaseEvent.h>
#include <BoundingMesh.h>
#include <ConfigFile.h>
#include <Context.h>
#include <DownloadManager.h>
#include <ElypseController.h>
#include <ElypseFrameListener.h>
#include <EMGui.h>
#include <ElypseInstance.h>
#include <ElypseLogs.h>
#include <EMOverlay.h>
#include <EMOverlayGroup.h>
#include <ElypseLoadingBar.h>
#include <ElypsePlugin.h>
#include <Events.h>
#include <MirrorManager.h>
#include <MuseDownloader.h>
#include <NetworkManager.h>
#include <PhysicsEngine.h>
#include <PhysicsSimulation.h>
#include <PostEffect.h>
#include <SceneFileParser.h>
#include <ScriptCompiler.h>
#include <ScriptEngine.h>
#include <ScriptFunctions.h>
#include <ScriptNode.h>
#include <ScriptTimerManager.h>
#include <Sequence.h>
#include <SequenceManager.h>
#include <SlottedObjectPool.h>
#include <SoundInstance.h>
#include <SoundObject.h>
#include <StringUtils.h>
#include <SurfaceManager.h>
#include <Universe.h>
#include <VariableType.h>
#include <VideoManager.h>
#include <VideoObject.h>
#include <VideoOverlay.h>
#include <Zone.h>

#include <wx/wx.h>
#include <wx/aui/auibook.h>
#include <wx/button.h>
#include <wx/bmpbuttn.h>
#include <wx/caret.h>
#include <wx/checkbox.h>
#include <wx/checklst.h>
#include <wx/colordlg.h>
#include <wx/colour.h>
#include <wx/combobox.h>
#include <wx/dialog.h>
#include <wx/defs.h>
#include <wx/dir.h>
#include <wx/dnd.h>
#include <wx/dynlib.h>
#include <wx/event.h>
#include <wx/filedlg.h>
#include <wx/filename.h>
#include <wx/icon.h>
#include <wx/image.h>
#include <wx/imaglist.h>
#include <wx/listbox.h>
#include <wx/listctrl.h>
#include <wx/log.h>
#include <wx/math.h>
#include <wx/frame.h>
#include <wx/notebook.h>
#include <wx/numdlg.h>
#include <wx/msgdlg.h>
#include <wx/panel.h>
#include <wx/popupwin.h>
#include <wx/scrolwin.h>
#include <wx/spinctrl.h>
#include <wx/splitter.h>
#include <wx/srchctrl.h>
#include <wx/stattext.h>
#include <wx/stc/stc.h>
#include <wx/stdpaths.h>
#include <wx/string.h>
#include <wx/textctrl.h>
#include <wx/textdlg.h>
#include <wx/treebase.h>
#include <wx/treectrl.h>
#include <wx/txtstrm.h>
#include <wx/wfstream.h>

#include <zlib.h>
#include <MD5.h>
#include <zip.h>

#if !defined( NDEBUG )
#	if defined( VLD_AVAILABLE )
#include <vld.h>      // redefines the new() operator 
#	elif defined( __WXMSW__ )
#include <wx/msw/msvcrt.h>      // redefines the new() operator 
#	endif
#endif

#pragma warning( pop )

#endif
