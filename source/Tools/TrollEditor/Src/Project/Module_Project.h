#ifndef ___Module_Project___
#define ___Module_Project___

namespace Troll
{
	enum State
	{
		Normal,
		Selected,
		MouseOver
	};

	enum ObjectType
	{
		tt3DObject,
		ttOverlay,
		ttTemporalObject,
		ttMediaObject
	};

	enum AntiAliasing
	{
		aa0 = 0,
		aa2x = 1,
		aa4x = 2
	};

	enum BackgroundType
	{
		bgColour,
		bgImage
	};

	enum InterpolationMethod
	{
		Linear,
		Square,
		Cubic,
		Sinus,
		Bezier
	};

	enum FileType
	{
		sceneFile,
		loadScriptFile,
		unloadScriptFile,
		dataFile,
		dataFolder
	};

	enum ShadowType
	{
		stNone,
		stTextureAdditive,
		stStencilAdditive
	};

	struct TrollFile;

	class TrollSceneFile;
	class TrollScene;
	class Project;
	class TrollObject;
}

typedef std::vector		<Troll::TrollFile *>					TrollFileArray;
typedef std::map		<unsigned int,	wxString>				wxStringUIntMap;
typedef std::map		<wxString,		Troll::TrollSceneFile *>		SceneFileStrMap;
typedef std::map		<wxString,		Troll::TrollScene *>			SceneMap;
typedef std::map		<wxString,		Troll::TrollObject *>	TrollObjectMap;

#endif
