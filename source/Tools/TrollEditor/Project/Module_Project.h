/*
See LICENSE file in root folder
*/
#ifndef ___Module_Project___
#define ___Module_Project___

#include "TrollEditorPrerequisites.h"

namespace Troll
{
	namespace ProjectComponents
	{
		enum State
		{
			Normal,
			Selected,
			MouseOver,
			StateCount
		};

		enum ObjectType
		{
			tt3DObject,
			ttOverlay,
			ttTemporalObject,
			ttMediaObject,
			ObjectTypeCount
		};

		enum AntiAliasing
		{
			aa0 = 0,
			aa2x = 1,
			aa4x = 2,
			AntialiasingCount
		};

		enum BackgroundType
		{
			bgColour,
			bgImage,
			BackgroundTypeCount
		};

		enum InterpolationMethod
		{
			Linear,
			Square,
			Cubic,
			Sinus,
			Bezier,
			InterpolationMethodCount
		};

		enum FileType
		{
			sceneFile,
			loadScriptFile,
			unloadScriptFile,
			dataFile,
			dataFolder,
			FileTypeCount
		};

		enum ShadowType
		{
			stNone,
			stTextureAdditive,
			stStencilAdditive,
			ShadowTypeCount
		};

		class Displayable;
		class Object;
		class Project;
		class Scene;
		struct File;
		class SceneFile;
		class Object;

		using ScenePtr = std::shared_ptr< Scene >;
		typedef std::vector< File > FileArray;
		typedef std::map< uint32_t, wxString > wxStringUIntMap;
		typedef std::map< wxString, SceneFile * > SceneFileStrMap;
		typedef std::vector< ScenePtr > SceneArray;
		typedef std::map< wxString, ScenePtr > SceneMap;
		typedef std::map< wxString, Object * > ObjectMap;

		typedef std::shared_ptr< Project > ProjectSPtr;
		typedef std::weak_ptr< Project > ProjectWPtr;
	}
}

#endif
