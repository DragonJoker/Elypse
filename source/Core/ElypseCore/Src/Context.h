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

#include <OgreColourValue.h>

namespace Elypse
{
	namespace Main
	{
		struct Context
		{
			SceneManager		*	 sceneManager;
			ScriptEngine		*	 scriptEngine;
			DownloadManager		*	 downloadManager;
			EMGui			*		 gui;
			MirrorManager		*	 mirrorManager;
			PhysicsEngine		*	 physicsEngine;
			CamTexManager		*	 camTexManager;
			UniverseManager		*	 universeManager;
			PostEffectManager		* postEffectManager;
			ObjectMaterialManager	* objectMaterialManager;
			ScriptTimerManager		* timerManager;
			SceneFileParser		*	 sceneFileParser;
			AnimationManager		* animationManager;
			ElypsePlugin			*	 plugin;
			ElypseInstance		*	 emuseInstance;
			SequenceManager		*	 sequenceManager;
			NetworkManager		*	 network;
			String					  lastSceneFile;
			String					  instanceNum;
			String					  lastDataRep;

			MuseFile			*	 currentMuseFile;

			Camera			*		 mainCamera;
			Viewport			*	 mainViewport;
			Real					  frameTime;

			bool					  m_continue;
			bool					  m_breakOne;
			bool					  m_return;

			ScriptNode			*	 m_currentFunction;

			ScriptSection			section;

			String 					groupName;
			String 					filename;
			String 					fullLine;
			String 					baseDirectory;
			Url						baseURL;
			String 					imageDir;
			String 					general_string_a;
			String 					general_string_b;
			String 					general_string_c;

			String					autoPhysicsPreName;
			String					autoPhysicsPostName;

			unsigned int			lineNo;
			int						general_int;

			Real 					currentReal;

			bool 					usingShadow;
			bool 					forceShadows;
			bool					autoPhysics;

			SceneNode			*	 sceneNode;
			Entity			*		 object;
			SubEntity			*	 subObject;
			Light			*		 light;
			Camera			*		 camera;
			EMOverlay			*	 overlay;
			EMOverlayGroup		*	 overlayGroup;

			Mirror			*		 mirror;
			ColourValue				  CurrentBackground;
			Zone			*		 scene;

			EMAnimation			*	 animation;
			AnimatedObject		*	 animatedObject;
			AnimatedObjectGroup		* animatedObjectGroup;

			PhysicsObject		*	 physicsObject;

			Universe			*	 universe;
			ParticleSystem		*	 particleSystem;
			PhysicsMaterial		*	 physicsMaterial;
			PhysicsSimulation		* physicsSimulation;
			BoundingShape		*	 boundingShape;
			Space			*		 physicsSpace;

			SoundPlaylist		*	 soundPlaylist;
			SoundObject			*	 soundObject;
			SoundInstance		*	 soundInstance;

			Portal			*		 portal;
			BillboardSet		*	 billboardSet;

			ClickableShape		*	 clickableShape;
			VideoObject			*	 videoObject;

			Sequence			*	 sequence;
			SequenceTrackType		sequenceTrackType;
			BaseContinuousEvent		* sequenceTrack;
			BaseContinuousV3Event	* sequenceTrack_V3;
			BaseContinuousQEvent	* sequenceTrack_Q;
			BaseContinuousREvent	* sequenceTrack_R;

			PubObject			*	 pubObject;

			void			*		 sequenceTarget;

			std::ifstream			configFile;

			Context()
				: m_continue( false ),
					m_breakOne( false ),
					m_return( false ),
					m_currentFunction( NULL )
			{
			}
		};
	}
}

#endif
