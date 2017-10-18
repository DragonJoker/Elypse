/*
This source file is part of ElypsePlayer (https://sourceforge.net/projects/elypse/)

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
the program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
*/
#ifndef ___EMUSE_CONTEXT_H___
#define ___EMUSE_CONTEXT_H___

#include "Module_Script.h"
#include "Module_Physics.h"
#include "Module_Gui.h"
#include "Module_Media.h"
#include "Module_Data.h"
#include "Module_Pub.h"
#include "Module_SpecialEffects.h"
#include "Module_Animation.h"
#include "Module_Zone.h"
#include "Module_Download.h"
#include "Module_SceneFile.h"
#include "Module_Main.h"
#include "SequenceManager.h"
#include "Module_Network.h"

#include "BaseEvent.h"

#include <OgreColourValue.h>

namespace Elypse
{
	namespace Main
	{
		struct Context
		{
			SceneManager * sceneManager{ nullptr };
			ScriptEngine * scriptEngine{ nullptr };
			DownloadManager * downloadManager{ nullptr };
			EMGui * gui{ nullptr };
			MirrorManager * mirrorManager{ nullptr };
			PhysicsEngine * physicsEngine{ nullptr };
			CamTexManager * camTexManager{ nullptr };
			UniverseManager * universeManager{ nullptr };
			PostEffectManager * postEffectManager{ nullptr };
			ObjectMaterialManager * objectMaterialManager{ nullptr };
			ScriptTimerManager * timerManager{ nullptr };
			SceneFileParser * sceneFileParser{ nullptr };
			AnimationManager * animationManager{ nullptr };
			ElypsePlugin * plugin{ nullptr };
			ElypseInstance * emuseInstance{ nullptr };
			SequenceManager * sequenceManager{ nullptr };
			Network::NetworkManager * network{ nullptr };
			String lastSceneFile;
			String instanceNum;
			String lastDataRep;

			MuseFile * currentMuseFile{ nullptr };

			Camera * mainCamera{ nullptr };
			Viewport * mainViewport{ nullptr };
			Real frameTime{ 0.0 };

			bool m_continue{ false };
			bool m_breakOne{ false };
			bool m_return{ false };

			ScriptNode * m_currentFunction{ nullptr };

			ScriptSection section;

			String groupName;
			String filename;
			String fullLine;
			String baseDirectory;
			Url baseURL;
			String imageDir;
			String general_string_a;
			String general_string_b;
			String general_string_c;

			String autoPhysicsPreName;
			String autoPhysicsPostName;

			uint32_t lineNo{ 0 };
			int general_int{ 0 };

			Real currentReal{ 0.0 };

			bool usingShadow{ false };
			bool forceShadows{ false };
			bool autoPhysics{ false };

			SceneNode * sceneNode{ nullptr };
			Entity * object{ nullptr };
			SubEntity * subObject{ nullptr };
			Light * light{ nullptr };
			Camera * camera{ nullptr };
			EMOverlay * overlay{ nullptr };
			EMOverlayGroup * overlayGroup{ nullptr };

			std::shared_ptr< Mirror > mirror;
			ColourValue CurrentBackground;
			Zone * scene{ nullptr };

			EMAnimation * animation{ nullptr };
			AnimatedObject * animatedObject{ nullptr };
			std::shared_ptr< AnimatedObjectGroup > animatedObjectGroup;

			PhysicsObject * physicsObject{ nullptr };

			std::shared_ptr< Universe > universe;
			ParticleSystem * particleSystem{ nullptr };
			PhysicsMaterial * physicsMaterial{ nullptr };
			PhysicsSimulation * physicsSimulation{ nullptr };
			BoundingShape * boundingShape{ nullptr };
			Space * physicsSpace{ nullptr };

			SoundPlaylist * soundPlaylist{ nullptr };
			SoundObject * soundObject{ nullptr };
			SoundInstance * soundInstance{ nullptr };

			Portal * portal{ nullptr };
			BillboardSet * billboardSet{ nullptr };

			ClickableShape * clickableShape{ nullptr };
			VideoObject * videoObject{ nullptr };

			std::shared_ptr< Sequence > sequence;
			SequenceTrackType sequenceTrackType;
			BaseContinuousEvent * sequenceTrack{ nullptr };
			BaseContinuousV3Event * sequenceTrack_V3{ nullptr };
			BaseContinuousQEvent * sequenceTrack_Q{ nullptr };
			BaseContinuousREvent * sequenceTrack_R{ nullptr };

			PubObject * pubObject{ nullptr };

			std::unique_ptr< EventTargetBase > sequenceTarget;

			std::ifstream configFile;
		};
	}
}

#endif
