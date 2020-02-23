/*
See LICENSE file in root folder
*/
#ifndef ____EMUSE__H___
#define ____EMUSE__H___

#define ___NO_NAMESPACE_OGRE___

#define ELYPSE_NAMESPACE Elypse
#define BEGIN_ELYPSE_NAMESPACE namespace Elypse
#define END_ELYPSE_NAMESPACE

#if defined( _WIN32) || defined( __WIN32__) || defined( WIN32)
#	define ELYPSE_LINUX 0
#	define ELYPSE_MACOS 0
#	define ELYPSE_WINDOWS 1
#	if !defined( WINVER )
#		define WINVER 0x0501
#		define _WIN32_WINNT 0x0501
#		define _WIN32_WINDOWS 0x0410
#	endif
#	if defined( ElypseCore_EXPORTS)
#		define d_dll_export __declspec( dllexport )
#	elif ! defined( ELYPSE_STATIC_LIB)
#		define d_dll_export __declspec( dllimport )
#	else
#		define d_dll_export
#	endif
#elif defined( linux) || defined( __linux) || defined( __linux__)
#	define ELYPSE_LINUX 1
#	define ELYPSE_MACOS 0
#	define ELYPSE_WINDOWS 0
#	define d_dll_export
#elif defined( macintosh) || defined( __APPLE__) || defined( __APPLE_CC__)
#	define ELYPSE_LINUX 0
#	define ELYPSE_MACOS 1
#	define ELYPSE_WINDOWS 0
#	define d_dll_export
#endif

#define ___NO_SCRIPT_COMPILER_DEFINE___
#pragma warning( push )
#pragma warning( disable:4267 )
#pragma warning( disable:4312 )
#pragma warning( disable:4311 )
#include <OgrePrerequisites.h>
#include <OgreArchive.h>
#include <OgreFont.h>
#include <OgreFontManager.h>
#include <OgreOverlay.h>
#include <OgreOverlayManager.h>
#include <OgreStringVector.h>
#pragma warning( pop )
#include <vector>
#include <map>
#include <queue>
#include <set>
#include <memory>
#include <cstdint>

#ifdef GENLIB_USE_ASSERT
#	undef GENLIB_USE_ASSERT
#endif
#define GENLIB_USE_ASSERT 1
#include <Assertions.h>

#define ____GENLIB_NO_MEMORY_DEBUG____
#include <Memory.h>

#include <NonCopyable.h>
#include <MinMax.h>
#include <Macros.h>
#include <STLMapMacros.h>
#include <STLSetMacros.h>
#include <STLMacros.h>
#include <Named.h>
#include <OwnedBy.h>
#include <FunctionTracker.h>

namespace Ogre
{
	class TextAreaOverlayElement;
	class BorderPanelOverlayElement;
	class Exception;
	class SkeletonManager;
	class ResourceGroupListener;
	class StringUtil;
	struct Box;
	class HardwareBuffer;
	class PixelUtil;
	class ImageCodec;
}

using Ogre::String;
using Ogre::Real;
using Ogre::Vector2;
using Ogre::Vector3;
using Ogre::Vector4;
using Ogre::Entity;
using Ogre::OverlayElement;
using Ogre::Plane;
using Ogre::Quaternion;
using Ogre::Overlay;
using Ogre::RenderWindow;
using Ogre::SceneManager;
using Ogre::Camera;
using Ogre::Root;
using Ogre::Log;
using Ogre::StringConverter;
using Ogre::ManualObject;
using Ogre::AxisAlignedBox;
using Ogre::RenderOperation;
using Ogre::TextAreaOverlayElement;
using Ogre::BorderPanelOverlayElement;
using Ogre::Viewport;
using Ogre::SceneNode;
using Ogre::SubEntity;
using Ogre::Light;
using Ogre::ColourValue;
using Ogre::ParticleSystem;
using Ogre::Billboard;
using Ogre::BillboardSet;
using Ogre::Material;
using Ogre::Texture;
using Ogre::Image;
using Ogre::Ray;
using Ogre::MaterialManager;
using Ogre::MeshPtr;
using Ogre::MeshManager;
using Ogre::ResourceGroupManager;
using Ogre::SkeletonManager;
using Ogre::StringVector;
using Ogre::Degree;
using Ogre::Radian;
using Ogre::Node;
using Ogre::Matrix3;
using Ogre::Matrix4;
using Ogre::ResourceGroupListener;
using Ogre::OverlayManager;
using Ogre::Math;
using Ogre::FontPtr;
using Ogre::RenderTarget;
using Ogre::AnimationState;
using Ogre::MovablePlane;
using Ogre::TextureUnitState;
using Ogre::CompositorInstance;
using Ogre::CompositorManager;
using Ogre::MaterialPtr;
using Ogre::FrameListener;
using Ogre::FrameEvent;
using Ogre::StringUtil;
using Ogre::EdgeData;
using Ogre::Exception;
using Ogre::FontManager;
using Ogre::Mesh;
using Ogre::PixelBox;
using Ogre::Pass;
using Ogre::TextureManager;
using Ogre::Box;
using Ogre::HardwareBuffer;
using Ogre::PixelUtil;
using Ogre::Codec;
using Ogre::ImageCodec;

#define Ode

namespace General
{
	namespace Templates
	{
		template< typename T >
		class Manager;
	}
}

namespace General
{
	namespace Files
	{
		class CURLManager;
		class FileManager;
	}
}


namespace Elypse
{
	typedef std::pair <String, String>	StringPair;
	typedef std::pair <bool, Real>		RayResult;

	typedef std::vector <Vector3>		Vector3Array;
	typedef std::vector <Entity *>		EntityArray;
	typedef std::vector <String>		StringArray;
	typedef std::vector <int>			IntArray;
	typedef std::vector <uint32_t>	UIntArray;
	typedef std::vector <StringPair>		StringPairArray;

	typedef std::queue <String>			StringQueue;

	typedef std::map <String, Real>		RealMap;
	typedef std::map <String, int>		IntMap;
	typedef std::map <String, uint32_t>		UIntMap;
	typedef std::map <int, String>		StringIMap;
	typedef std::map <int, String>		StringStringMap;

	typedef std::set<String>			StringSet;

};

extern String EMPTY_STRING;

using namespace Elypse;
using namespace General::Utils;
using namespace General::Theory;
using namespace General::Templates;
using namespace General::MultiThreading;
using namespace General::Files;

/*
#include <StringConverter.h>
#include <ThreadId.h>

#undef d_coucou
#define d_coucou EMUSE_CONSOLE_MESSAGE_RELEASE( "coucou : " + String(__FUNCTION__) + " @line " + ToString( __LINE__) + " / thread : "  + ToString( GENLIB_THREAD_ID()))
*/

#pragma warning(disable:4250)

#endif
