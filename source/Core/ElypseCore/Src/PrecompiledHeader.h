#pragma once
#ifndef ___PRECOMPILED_HEADER_H___
#define ___PRECOMPILED_HEADER_H___

#pragma warning( disable:4251 )
#pragma warning( disable:4275 )

// ODE using single precision floats
#define dSINGLE

#define WINVER 0x0501
#define _WIN32_WINNT 0x0501
#define _WIN32_WINDOWS 0x0501

#define BOOST_REGEX_NO_LIB
#define BOOST_DATE_TIME_NO_LIB
#define BOOST_NO_TYPEID
#define BOOST_ASIO_DISABLE_BUFFER_DEBUGGING
#define BOOST_ASIO_NO_DEFAULT_LINKED_LIBS

#pragma warning( push )
#pragma warning( disable:4267 )
#pragma warning( disable:4312 )
#pragma warning( disable:4311 )
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/condition.hpp>
#include <boost/version.hpp>
#pragma warning( pop )

#define ___NO_SCRIPT_COMPILER_DEFINE___
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <set>
#include <list>
#include <ctime>

#pragma warning( push )
#pragma warning( disable:4312 )
#pragma warning( disable:4311 )

#include <OgrePrerequisites.h>

#include "EMuse.h"

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
#include <OgreStringConverter.h>
#include <OgreSubEntity.h>
#include <OgreSubMesh.h>
#include <OgreTexture.h>
#include <OgreTextureManager.h>
#include <OgreTextAreaOverlayElement.h>
#include <OgreVector3.h>
#include <OgreViewport.h>

#pragma warning( pop )

#endif
